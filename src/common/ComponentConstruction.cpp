#include "ComponentConstruction.h"
#include "NodeMesh.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "GameManager.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "MapManager.h"


ComponentConstruction::ComponentConstruction(const char* meshFilename, const char* meshFilenameStatic)
{
	if(!GameManager::getInstance()->isServer())
	{
		GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
		node = graphicsEngine->addNodeMesh(meshFilename);

		//Una malla o 2 dependiendo de si tiene varias partes
		if(meshFilenameStatic != "")
		{
			nodeStatic = graphicsEngine->addNodeMesh(meshFilenameStatic);
		}
		else
		{
			nodeStatic = NULL;
		}
	}

	else
	{
		node = NULL;
		nodeStatic = NULL;
	}
}


ComponentConstruction::~ComponentConstruction(void)
{
	delete node;
	node = NULL;
	delete nodeStatic;
	nodeStatic=NULL;
}

void ComponentConstruction::building()
{
	//Si ha pasado el tiempo se crea
	if(timeBuild.getElapsedTime() > 6000)
	{
		create();
	}
	else if(timeBuild.getElapsedTime() > 3000) //Si pasa la mitad del tiempo se empieza a crear la parte principal
	{
		if(nodeStatic != NULL)
		{
			nodeStatic->setPosition(parent->position);
		}

		if(node != NULL)
		{
			node->setPosition(parent->position);
		}
	}
	else //Si aun no ha llegado al tiempo minimo se emppieza a construir la parte secundaria si tiene
	{
		if(nodeStatic != NULL)
		{
			nodeStatic->setPosition(parent->position);
		}

		if(node != NULL)
		{
			node->setPosition(GameManager::getInstance()->getMapManager()->getFrameCenter(300,300));
		}
	}
	if(timeBuild.getElapsedTime() % 5 == 0) //Cada cierto tiempo crea chispas
	{
		(GameManager::getInstance()->getGameObjectManager()->createSpark())->position = parent->position + Vector2d(Math::random(-3,3),Math::random(-3,3));

		//Le mando un -3 para que le sume 3 a la vida actual de la construccion
		Message message;
		message.type = Message::INCOMING_DAMAGE;
		message.value = -3;
		parent->broadcastMessage(message);
	}
}

void ComponentConstruction::update()
{
	building();
}

void ComponentConstruction::onMessage(const Message& message)
{

}

void ComponentConstruction::create()
{
	GameObject* element = NULL;
	MapManager *aiAux = GameManager::getInstance()->getMapManager();

	switch (parent->getTag())
	{
	case GameObject::BUILDING_TURRET_PROJECTILE:
		if(GameManager::getInstance()->getNetworkEngine()->isStarted())
		{
			if(GameManager::getInstance()->isServer())
			{
				element = GameManager::getInstance()->getGameObjectManager()->createTurret();
			}
		}
		else
		{
			element = GameManager::getInstance()->getGameObjectManager()->createTurret();
		}
		delete node;
		node = NULL;
		delete nodeStatic;
		nodeStatic = NULL;
		break;
	case GameObject::BUILDING_TURRET_BULLET:
		if(GameManager::getInstance()->getNetworkEngine()->isStarted())
		{
			if(GameManager::getInstance()->isServer())
			{
				element = GameManager::getInstance()->getGameObjectManager()->createTurret2();
			}
		}
		else
		{
			element = GameManager::getInstance()->getGameObjectManager()->createTurret2();
		}
		delete node;
		node = NULL;
		delete nodeStatic;
		nodeStatic = NULL;
		break;
	case GameObject::BUILDING_TURRET_2_CANON:
		if(GameManager::getInstance()->getNetworkEngine()->isStarted())
		{
			if(GameManager::getInstance()->isServer())
			{
				element = GameManager::getInstance()->getGameObjectManager()->createTurret2Canon();
			}
		}
		else
		{
			element = GameManager::getInstance()->getGameObjectManager()->createTurret2Canon();
		}
		delete node;
		node = NULL;
		delete nodeStatic;
		nodeStatic = NULL;
		break;
	case GameObject::BUILDING_TURRET_BIG_CANON:
		if(GameManager::getInstance()->getNetworkEngine()->isStarted())
		{
			if(GameManager::getInstance()->isServer())
			{
				element = GameManager::getInstance()->getGameObjectManager()->createTurretBigCanon();
			}
		}
		else
		{
			element = GameManager::getInstance()->getGameObjectManager()->createTurretBigCanon();
		}
		delete node;
		node = NULL;
		delete nodeStatic;
		nodeStatic = NULL;
		break;
	case GameObject::BUILDING_TURRET_1_CANON:
		if(GameManager::getInstance()->getNetworkEngine()->isStarted())
		{
			if(GameManager::getInstance()->isServer())
			{
				element = GameManager::getInstance()->getGameObjectManager()->createTurret1Canon();
			}
		}
		else
		{
			element = GameManager::getInstance()->getGameObjectManager()->createTurret1Canon();
		}
		delete node;
		node = NULL;
		delete nodeStatic;
		nodeStatic = NULL;
		break;
	case GameObject::BUILDING_TURRET_GUN:
		if(GameManager::getInstance()->getNetworkEngine()->isStarted())
		{
			if(GameManager::getInstance()->isServer())
			{
				element = GameManager::getInstance()->getGameObjectManager()->createTurretGun();
			}
		}
		else
		{
			element = GameManager::getInstance()->getGameObjectManager()->createTurretGun();
		}
		delete node;
		node = NULL;
		delete nodeStatic;
		nodeStatic = NULL;
		break;

	case GameObject::BUILDING_MINE2:
		if(GameManager::getInstance()->getNetworkEngine()->isStarted())
		{
			if(GameManager::getInstance()->isServer())
			{
				element = GameManager::getInstance()->getGameObjectManager()->createMine2();
			}
		}
		else
		{
			element = GameManager::getInstance()->getGameObjectManager()->createMine2();
		}
		delete node;
		node = NULL;
		break;
	case GameObject::BUILDING_MINE3:
		if(GameManager::getInstance()->getNetworkEngine()->isStarted())
		{
			if(GameManager::getInstance()->isServer())
			{
				element = GameManager::getInstance()->getGameObjectManager()->createMine3();
			}
		}
		else
		{
			element = GameManager::getInstance()->getGameObjectManager()->createMine3();
		}
		delete node;
		node = NULL;
		break;
	case GameObject::BUILDING_MINE:
		if(GameManager::getInstance()->getNetworkEngine()->isStarted())
		{
			if(GameManager::getInstance()->isServer())
			{
				element = GameManager::getInstance()->getGameObjectManager()->createMine();
			}
		}
		else
		{
			element = GameManager::getInstance()->getGameObjectManager()->createMine();
		}
		delete node;
		node = NULL;
		break;
	case GameObject::BUILDING_WALL:
		if(GameManager::getInstance()->getNetworkEngine()->isStarted())
		{
			if(GameManager::getInstance()->isServer())
			{
				//element = GameManager::getInstance()->getGameObjectManager()->createWall();
			}
		}
		else
		{
			element = GameManager::getInstance()->getGameObjectManager()->createWall();
		}
		delete node;
		node = NULL;
		break;
	}

	if(element != NULL)
	{
		element->position = parent->position;
		element->rotation = parent->rotation;
	}

	parent->kill();
}
