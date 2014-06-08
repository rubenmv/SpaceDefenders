#include "StateTestAIFury.h"
#include <iostream>
#include <sstream>
#include <vector>
#include "../GraphicsEngine.h"
#include "../GameManager.h"
#include "../GameObject.h"
#include "../GameObjectManager.h"
#include "../EventManager.h"
#include "../Component.h"
#include "../ComponentGraphics.h"
#include "../ComponentCamera.h"
#include "../DebugTools.h"
#include "../AIEngine.h"


// Singleton
StateTestAIFury* StateTestAIFury::pInstance = NULL;

StateTestAIFury::~StateTestAIFury(void)
{
	mapManager = NULL;
}


StateTestAIFury* StateTestAIFury::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateTestAIFury();

	return pInstance;
}


StateTestAIFury::StateTestAIFury()
{
	name = State::StateName::TEST_WAVES;
}

void StateTestAIFury::init()
{
	gameManager = GameManager::getInstance();

	gameObjectManager = gameManager->getGameObjectManager();
	collisionManager = gameManager->getCollisionManager();
	
	mapManager = gameManager->getMapManager();
	gameManager->getGraphicsEngine()->createDebugMap(mapManager->getCollisionMap());
//	physicsManager = gameManager->getPhysicsManager();
	mapMesh = gameManager->getGraphicsEngine()->addNodeMesh("../assets/models/map/map.obj");
	position = Vector2d(0, -800);
	rotation = 0;
	mapMesh->setPosition(position);
	mapMesh->setRotation(rotation);
	mapMesh->setScale(Vector3d(1, 1, 1));

	aiEngine =  gameManager->getAIEngine();

	gameObjectManager->createWholeBase(Vector2d(144,-770));
	GameObject* player = gameObjectManager->createFighter();
	player->position = Vector2d(548,-329);
	
	//GameObject* bomber = gameObjectManager->createEnemyBomber();
	gameManager->getGraphicsEngine()->initWorld();
}


void StateTestAIFury::cleanup()
{
}

void StateTestAIFury::update(GameManager* gameManager)
{

	/*if(gameManager->getEventManager()->isKeyDown(KEY::KEY_KEY_7))
	{
		gameObjectManager->createEnemyBasic();	
	}*/
	if(gameManager->getEventManager()->isKeyDown(KEY::KEY_KEY_8))
	{
		gameObjectManager->createFuryEnemies();
	}
	if(gameManager->getEventManager()->isKeyDown(KEY::KEY_KEY_9))
	{
		GameObject* bomber = gameObjectManager->createEnemyBomber();	
	}
	if(gameManager->getEventManager()->isKeyDown(KEY::KEY_KEY_0))
	{
		//crear oleada
	}
	gameObjectManager->update();
	collisionManager->update();
	gameManager->getDebugTools()->update();

	if(gameManager->getEventManager()->isKeyPressed(KEY::KEY_KEY_U))
	{
		position.y += 1;
	}
	if(gameManager->getEventManager()->isKeyPressed(KEY::KEY_KEY_J))
	{
		position.y -= 1;
	}
	if(gameManager->getEventManager()->isKeyPressed(KEY::KEY_KEY_H))
	{
		position.x += 1;
	}
	if(gameManager->getEventManager()->isKeyPressed(KEY::KEY_KEY_K))
	{
		position.x -= 1;
	}
	// Rotacion
	if(gameManager->getEventManager()->isKeyPressed(KEY::KEY_KEY_Y))
	{
		rotation += 1;
	}
	if(gameManager->getEventManager()->isKeyPressed(KEY::KEY_KEY_I))
	{
		rotation -= 1;
	}

	mapMesh->setRotation( rotation );
	mapMesh->setPosition( position );
}

