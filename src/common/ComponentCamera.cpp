#include "ComponentCamera.h"
#include "NodeCamera.h"
#include "GameObject.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "Math.h"

ComponentCamera::ComponentCamera(Vector3d relativePosition)
{
	this->relativePosition = relativePosition;
	node = GameManager::getInstance()->getGraphicsEngine()->addNodeCamera(relativePosition);
	eventManager = GameManager::getInstance()->getEventManager();

	startYPosition = relativePosition.y;

	speed = 0;
}

ComponentCamera::~ComponentCamera(void)
{
	delete node;
	node = NULL;
	eventManager = NULL;
}
void ComponentCamera::awake()
{
}
void ComponentCamera::update()
{
	shake();

	// ZOOM/ALTURA de la camara
/*	if(eventManager->isKeyPressed(KEY::KEY_HOME) || eventManager->isKeyPressed(KEY::KEY_KEY_P))
	{
		relativePosition.y = startYPosition;
	}
	if(eventManager->mouseState.wheelUp || eventManager->isKeyPressed(KEY::KEY_PRIOR) || eventManager->isKeyPressed(KEY::KEY_KEY_O))
	{
		speed += ACCELERATION;
	}
	else if(eventManager->mouseState.wheelDown || eventManager->isKeyPressed(KEY::KEY_NEXT) || eventManager->isKeyPressed(KEY::KEY_KEY_L))
	{
		speed -= ACCELERATION;
	}

	if (speed > MAX_SPEED)
	{
		speed = MAX_SPEED;
	}
	if (speed < -MAX_SPEED)
	{
		speed = -MAX_SPEED;
	}

	speed *= FRICTION;

	if (Math::abs(speed) < Math::abs(0.1))
	{
		speed = 0;
	}

	relativePosition.y += speed;

	if (relativePosition.y > MAX_ZOOM)
	{
		relativePosition.y = MAX_ZOOM;
	}
	else if (relativePosition.y < MIN_ZOOM)
	{
		relativePosition.y = MIN_ZOOM;
	}
	*/

	Vector3d playerPosition(parent->position.x, 0, parent->position.y);
	node->setPosition(playerPosition + relativePosition + offset);
	node->setTargetPosition(playerPosition + offset);
}

void ComponentCamera::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::NEW_VISION:
		onNewVision(message.gameObject);
		break;
	case Message::LOST_VISION:
		onLostVision(message.gameObject);
		break;
	case Message::ADD_EXPERIENCE:
		shakeRate = 0.8;
		break;
	case Message::INCOMING_DAMAGE:
		shakeRate = 0.4;
		break;
	}
	
}

void ComponentCamera::onNewVision(GameObject *gameObject )
{
	Message message;
	message.type = Message::ON_CAMERA;
	gameObject->broadcastMessage(message);
}

void ComponentCamera::onLostVision( GameObject* gameObject )
{
	Message message;
	message.type = Message::OUT_OF_CAMERA;
	gameObject->broadcastMessage(message);
}

void ComponentCamera::shake()
{
	/*if(eventManager->isKeyDown(KEY::KEY_KEY_Z))
	{
		shakeRate = 0.8;
	}*/

	offset.x = shakeRate * generateRandom();
	offset.y = shakeRate * generateRandom();
	offset.z = shakeRate * generateRandom();

	shakeRate *= 0.8;

	if(shakeRate < 0.001)
	{
		shakeRate = 0;
	}
}

float ComponentCamera::generateRandom()
{
	return Math::random(100)/100.0 - 0.5;
}
