#include "ComponentInventory.h"
#include "NodeMesh.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "GameManager.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "MapManager.h"


ComponentInventory::ComponentInventory(const char* meshFilename, unsigned int price, const char* meshFilenameStatic)
{
	GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	node = graphicsEngine->addNodeMesh(meshFilename);
	color = DebugColor::RED;
	
	//Cargamos la malla
	if(meshFilenameStatic != "")
	{
		nodeStatic = graphicsEngine->addNodeMesh(meshFilenameStatic);
	}
	else
	{
		nodeStatic = NULL;
	}
	//Si es un muro creamos un array de objetos del maximo numero de muros que hay en la fila
	node->setColor(color);
	if(meshFilenameStatic != "" && nodeStatic != NULL)
	{
		nodeStatic->setColor(color);
	}
	
	this->price = price;
	this->canBuild = true;
}


ComponentInventory::~ComponentInventory(void)
{
	delete node;
	node=NULL;

	delete nodeStatic;
	nodeStatic=NULL;
}


void ComponentInventory::viewing()
{
	//Si no ve obstacules se puede construir, si los ve no
	MapManager *aiAux = GameManager::getInstance()->getMapManager();
	if(obstacles.empty())
	{
		canBuild = true;
	}
	else
	{
		canBuild = false;
	}


	GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	node->setPosition(graphicsEngine->getMousePositionOnGround());
	parent->position = graphicsEngine->getMousePositionOnGround();
	if(aiAux->checkMap(aiAux->getFrame(parent->position).y,aiAux->getFrame(parent->position).x) == 'W' || parent->position.getSqrDistanceFrom(Vector2d(900,0)) < 122500)
	{
		canBuild = false;
	}
	//Si tiene parte estatica se pinta
	if(nodeStatic != NULL)
	{
		nodeStatic->setPosition(graphicsEngine->getMousePositionOnGround());
	}

	//Mira a ver si se pulsa algun boton para destruir el actual inventory y que no puedan haber muchos a la vez
	EventManager *eventManager = GameManager::getInstance()->getEventManager();
	if(eventManager->isKeyDown(KEY::KEY_KEY_1) || eventManager->isKeyDown(KEY::KEY_KEY_2) || 
		eventManager->isKeyDown(KEY::KEY_KEY_3) || eventManager->isKeyDown(KEY::KEY_KEY_4) ||
		eventManager->isKeyDown(KEY::KEY_KEY_5) || eventManager->isKeyDown(KEY::KEY_KEY_6) ||
		eventManager->isKeyDown(KEY::KEY_KEY_7) || eventManager->isKeyDown(KEY::KEY_KEY_8) || eventManager->mouseState.rightButtonDown)
	{
		parent->kill();
	}

	//Si se puede construir se le pone un color sino se le pone otro
	if(canBuild)
	{
		color = DebugColor::GREEN;
	}
	else
	{
		color = DebugColor::RED;
	}

	node->setColor(color);

	
	if(nodeStatic != NULL)
	{
		nodeStatic->setColor(color);
	}
	
	//Si pulsas el raton se mira que no estes encima del inventario y si se puede construir manda
	//un mensaje al jugador para ver si tiene dinero suficiente y se destruye el objeto
	if(GameManager::getInstance()->getEventManager()->mouseState.leftButtonDown)
	{
		GameManager::getInstance()->getEventManager()->mouseState.leftButtonDown = false;
		if(canBuild)
		{
			Message message;
			message.type = Message::TRY_BUY;
			message.value = price;
			message.gameObject = parent;
			GameManager::getInstance()->getGameObjectManager()->getMainPlayer()->broadcastMessage(message);
		} else
		{
			GameManager::getInstance()->getGraphicsEngine()->setWarningMessage(GameConstants::CANT_BUILD_MESSAGE);
		}
	}
}

void ComponentInventory::update()
{
	viewing();
}

void ComponentInventory::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::SOLD:
		//El jugador tenia dinero, debemos construir
		create();
		break;
	case Message::NOT_SOLD:
		//El jugador no tenia dinero, debemos destruir
		//destroy();
		break;
	case Message::NEW_VISION:
		//Mira si hay obstaculos en su posicion
		obstacles.push_back(message.gameObject);
		break;
	case Message::LOST_VISION:
		//Quita los obstaculos que ya no ve
		std::vector<GameObject*>::iterator it;
		if(!obstacles.empty())
		{
			for (it=obstacles.begin(); it!=obstacles.end(); ++it)
			{	
				if( *it == message.gameObject)
				{
					obstacles.erase(it);
					break;
				}
			}
		}
		break;
	}
}

void ComponentInventory::create()
{
	GameObject* element = NULL;
	MapManager *aiAux = GameManager::getInstance()->getMapManager();
	Vector2d pos = aiAux->getFrame(parent->position);
	pos = Vector2d(pos.y,pos.x);

	if(aiAux->checkMap(pos.x,pos.y) != 'W')
	{
		switch (parent->getName())
		{
		case GameObject::N_INVENTORY_TURRET_PROJECTILE:
			//Crea un objeto para empezar a construirse
			if(GameManager::getInstance()->getNetworkEngine()->isStarted())
			{
				GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_BUILDING_TURRET_PROJECTILE,parent->position);

			}
			else
			{
				element = GameManager::getInstance()->getGameObjectManager()->createBuildingTurret(parent->position);
			}
			delete node;
			node = NULL;
			delete nodeStatic;
			nodeStatic = NULL;
			break;
		case GameObject::N_INVENTORY_MINE:
			//Crea un objeto para empezar a construirse
			if(GameManager::getInstance()->getNetworkEngine()->isStarted())
			{
				GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_BUILDING_MINE,parent->position);

			}
			else
			{
				element = GameManager::getInstance()->getGameObjectManager()->createBuildingMine(parent->position);
			}
			break;
		case GameObject::N_MINE2:
			//Crea un objeto para empezar a construirse
			if(GameManager::getInstance()->getNetworkEngine()->isStarted())
			{
				GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_MINE2,parent->position);

			}
			else
			{
				element = GameManager::getInstance()->getGameObjectManager()->createBuildingMine2(parent->position);
			}
			break;
		case GameObject::N_MINE3:
			//Crea un objeto para empezar a construirse
			if(GameManager::getInstance()->getNetworkEngine()->isStarted())
			{
				GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_MINE3,parent->position);

			}
			else
			{
				element = GameManager::getInstance()->getGameObjectManager()->createBuildingMine3(parent->position);
			}
			break;
		case GameObject::N_INVENTORY_TURRET_BULLET:
			//Crea un objeto para empezar a construirse
			if(GameManager::getInstance()->getNetworkEngine()->isStarted())
			{
				GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_BUILDING_TURRET_BULLET,parent->position);
			}
			else
			{
				element = GameManager::getInstance()->getGameObjectManager()->createBuildingTurret2(parent->position);
			}
			delete node;
			node = NULL;
			delete nodeStatic;
			nodeStatic = NULL;
			break;
		case GameObject::N_INVENTORY_TURRET_2_CANON:
			//Crea un objeto para empezar a construirse
			if(GameManager::getInstance()->getNetworkEngine()->isStarted())
			{
				GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_BUILDING_TURRET_2_CANON,parent->position);
			}
			else
			{
				element = GameManager::getInstance()->getGameObjectManager()->createBuildingTurret2Canon(parent->position);
			}
			delete node;
			node = NULL;
			delete nodeStatic;
			nodeStatic = NULL;
			break;
		case GameObject::N_INVENTORY_TURRET_BIG_CANON:
			//Crea un objeto para empezar a construirse
			if(GameManager::getInstance()->getNetworkEngine()->isStarted())
			{
				GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_BUILDING_TURRET_BIG_CANON,parent->position);
			}
			else
			{
				element = GameManager::getInstance()->getGameObjectManager()->createBuildingTurretBigCanon(parent->position);
			}
			delete node;
			node = NULL;
			delete nodeStatic;
			nodeStatic = NULL;
			break;
		case GameObject::N_INVENTORY_TURRET_1_CANON:
			//Crea un objeto para empezar a construirse
			if(GameManager::getInstance()->getNetworkEngine()->isStarted())
			{
				GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_BUILDING_TURRET_1_CANON,parent->position);
			}
			else
			{
				element = GameManager::getInstance()->getGameObjectManager()->createBuildingTurret1Canon(parent->position);
			}
			delete node;
			node = NULL;
			delete nodeStatic;
			nodeStatic = NULL;
			break;
		case GameObject::N_INVENTORY_TURRET_GUN:
			//Crea un objeto para empezar a construirse
			if(GameManager::getInstance()->getNetworkEngine()->isStarted())
			{
				GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_BUILDING_TURRET_GUN,parent->position);
			}
			else
			{
				element = GameManager::getInstance()->getGameObjectManager()->createBuildingTurretGun(parent->position);
			}
			delete node;
			node = NULL;
			delete nodeStatic;
			nodeStatic = NULL;
			break;
		}
		if(element != NULL)
		{
			element->setPosition(parent->position.x,parent->position.y);
			element->rotation = parent->rotation;
		}

	}

	parent->kill();
}

void ComponentInventory::destroy()
{
	parent->kill();
}
