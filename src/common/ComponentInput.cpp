#include "ComponentInput.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "CollisionManager.h"
#include "ComponentDebris.h"

ComponentInput::ComponentInput(float speed)
{
	eventManager = GameManager::getInstance()->getEventManager();
	this->speed = speed;
	shooting = false;
	down = false;
	debugMode = false;
}


ComponentInput::~ComponentInput(void)
{
	eventManager = NULL;
}

void ComponentInput::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::DEATH:
		handleDeath();
		break;
	case Message::REPAIRED:
		repairShip();
		break;
	}
}
		
void ComponentInput::update()
{
	//Debug Mode -> Te permite moverte mientras estas down y asi no te disparan
	handleDebugMode();

	if (!down || debugMode)
	{
		handleMovementInput();
		handleSkillsInput();
		applyWallForces();
		handleShootingInput();
	}
}
//Creamos el sistema de reparacion y le pasamos el jugador para que pueda repararlo cuando termine
void ComponentInput::handleDeath()
{
	GameManager::getInstance()->getGameObjectManager()->killInventories();
	down = true;
	stopShooting(); //Si cae cuando esta disparando, ya no hace caso al input y se quedaba disparando
		
	GameObject * repairSystem = GameManager::getInstance()->getGameObjectManager()->createRepairSystem();
		
	repairSystem->position = parent->position;

	Message message; 

	message.type = Message::NEW_TARGET;
	message.gameObject = parent;

	repairSystem->broadcastMessage(message);
}

void ComponentInput::repairShip()
{
	down = false;
	debugMode = false;
}

void ComponentInput::stopShooting()
{
	Message message;
	message.type = Message::STOP_SHOOTING;
	parent->broadcastMessage(message);
}

void ComponentInput::startShooting()
{
	Message message;
	message.type = Message::START_SHOOTING;
	message.gameObject = NULL;
	parent->broadcastMessage(message);
}

void ComponentInput::handleDebugMode()
{
	//Si se cambia la tecla, hay que cambiarla tambien en ComponentRepairSystem.cpp
/*	if(eventManager->isKeyDown(KEY::KEY_KEY_R))
	{
		if(down) //El repairSystem se encarga de resucitarlo al darle a la R
		{
			debugMode = false;
		} else
		{
			//Le quitamos toda la vida y activamos el debugMode
			//Si no tiene el componente de vida no puede morir y no funcionara
			Message message;
			message.type = Message::INCOMING_DAMAGE;
			message.value = 99999999;
			parent->broadcastMessage(message);

			debugMode = true;
		}
	}*/
}

void ComponentInput::handleMovementInput()
{
	int forwardInput = 0;
	int rightInput = 0;

	if(eventManager->isKeyPressed(KEY::KEY_KEY_W))
	{
		forwardInput += 1; //El += esta puesto para que si esta W y S apretado se anulen y valga 0
	}

	if(eventManager->isKeyPressed(KEY::KEY_KEY_S))
	{
		forwardInput -= 1;
	}

	if(eventManager->isKeyPressed(KEY::KEY_KEY_D))
	{
		rightInput += 1;
	}

	if(eventManager->isKeyPressed(KEY::KEY_KEY_A))
	{
		rightInput -= 1;
	}

	GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	Vector2d target = graphicsEngine->getMousePositionOnGround();

	Vector2d direction = target - parent->position;
	
	direction.normalize();

	float desiredRotation = direction.getAngle();

	float newRotation = parent->rotation - desiredRotation;

	newRotation = Math::warpAngle(newRotation);

	if(Math::abs(newRotation) < 0.001)
	{
		parent->rotation = desiredRotation;
	} else
	{
		parent->rotation -= newRotation * GameManager::getInstance()->getDeltaTime() * 10;
	}

	parent->rotation = Math::warpAngle(parent->rotation);

	Vector2d newDirection = Vector2d::getVector2dByAngle(parent->rotation);

	//parent->position += direction * speed * forwardInput * GameManager::getInstance()->getDeltaTime();

	//Movimiento hacia adelante y hacia atras
	parent->acceleration += direction * speed * forwardInput;

	//Movimiento lateral

	Vector2d right(direction.y, -direction.x); //Vector direccion girado 90º hacia la derecha
	
	float decrease = 0.5; //Indica el porcentaje de la velocidad a la que puede ir en lateral

	parent->acceleration += right * speed * decrease * rightInput;
}

void ComponentInput::handleSkillsInput()
{
	for(int i = 1; i <= 8; i++)
	{
		int key = KEY::KEY_KEY_0 + i;
		if(eventManager->isKeyDown((KEY::EKEY_CODE) key))
		{
			Message message;
			message.type = Message::USE_SKILL;
			message.value = i;
			parent->broadcastMessage(message);
		}
	}
}

void ComponentInput::applyWallForces()
{
		Vector2d ejectionForce = GameManager::getInstance()->getMapManager()->getWallEjectionForce(parent->position);

		Vector2d ejectionDirection = ejectionForce;
		ejectionDirection.normalize();

		Vector2d velocityDirection = parent->velocity;
		velocityDirection.normalize();

		float intensity = velocityDirection.dot(ejectionDirection); //Devuelve 1 si la nave se aleja de la pared,-1 si va directa hacia ella, 0 si va en paralelo

		//Queremos que cuanto mas vaya hacia la pared mas fuerte sea la fuerza que lo expulse
		intensity = intensity < 0 ? 1.5 : 0 ; //Como queremos que lo expulse solo cuando vaya hacia la pared solo hay intensidad si el dot ha dado negativo
		
		parent->acceleration += ejectionForce * ejectionForce.getLength()/10 * intensity;
}

void ComponentInput::handleShootingInput()
{
	if(eventManager->mouseState.leftButtonDown)
	{
		startShooting();
	}

	if(eventManager->mouseState.leftButtonUp)
	{
		stopShooting();
	}
}