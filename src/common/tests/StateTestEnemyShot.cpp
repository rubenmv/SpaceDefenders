
#include <iostream>
#include <vector>
#include "StateTestEnemyShot.h"
#include "../GraphicsEngine.h"
#include "../GameManager.h"
#include "../GameObject.h"
#include "../GameObjectManager.h"
#include "../Component.h"
#include "../ComponentGraphics.h"
#include "../ComponentCamera.h"
#include "../DebugTools.h"


// Singleton
StateTestEnemyShot* StateTestEnemyShot::pInstance = NULL;

StateTestEnemyShot* StateTestEnemyShot::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateTestEnemyShot();

	return pInstance;
}

StateTestEnemyShot::StateTestEnemyShot()
{
	name = State::TEST_SHOT;
}

StateTestEnemyShot::~StateTestEnemyShot()
{
	// Limpia los objetos del juego
	mapManager = NULL;
	gameObjectManager->clear();
}

void StateTestEnemyShot::init()
{
	gameManager = GameManager::getInstance();

	gameObjectManager = gameManager->getGameObjectManager();
	collisionManager = gameManager->getCollisionManager();



	gameManager->getGraphicsEngine()->initWorld();

	mapManager = gameManager->getMapManager();
	gameManager->getGraphicsEngine()->createDebugMap(mapManager->getCollisionMap());

	//gameManager->getDebugTools()->init();


	
	GameObject* player = gameObjectManager->createEngineer();
	player->position = Vector2d(768,-135);

	gameObjectManager->createFuryEnemies();

	//GameObject* camera = gameObjectManager->createFreeCamera();
	//camera->position = Vector2d(0,0);
	//gameObjectManager->createMine();
	//gameObjectManager->createMine();
	//gameObjectManager->createMinesGenerator();
	
	/*GameObject* turret =gameObjectManager->createTurret();
	turret->position = Vector2d(375,-300);

	GameObject* turret1 =gameObjectManager->createTurret();
	turret1->position = Vector2d(237,-70);
	
	GameObject* turret2 =gameObjectManager->createTurret();
	turret2->position = Vector2d(319,-150);
	GameObject* turret3 =gameObjectManager->createTurret();
	turret3->position = Vector2d(74,-404);
	GameObject* turret4 =gameObjectManager->createTurret();
	turret4->position = Vector2d(395,-403);*/
	
	//gameObjectManager->createEnemyBomber();
	//gameObjectManager->createEnemyBasic();
	GameObject* basic = gameObjectManager->createEnemyBasic();
	basic->position = Vector2d(350,-90);
	
	/*GameObject* defender = gameObjectManager->createEnemyStill();
	defender->position = Vector2d(315,-222);
	GameObject* defender1 = gameObjectManager->createEnemyStill();
	defender1->position = Vector2d(320,-222);
	GameObject* defender2 = gameObjectManager->createEnemyStill();
	defender2->position = Vector2d(325,-222);
	GameObject* defender3 = gameObjectManager->createEnemyStill();
	defender3->position = Vector2d(330,-222);
	GameObject* defender4 = gameObjectManager->createEnemyStill();
	defender4->position = Vector2d(335,-222);
	GameObject* defender5 = gameObjectManager->createEnemyStill();
	defender5->position = Vector2d(340,-222);
	GameObject* defender6 = gameObjectManager->createEnemyStill();
	defender6->position = Vector2d(345,-222);
	GameObject* defender7 = gameObjectManager->createEnemyStill();
	defender7->position = Vector2d(350,-222);
	*/
	//gameObjectManager->createEnemyStill();
	//gameObjectManager->createEnemyBomber();
	//gameObjectManager->createEnemyCombat();
	//gameObjectManager->createEnemyBasic();
	//gameObjectManager->createEnemyCombat();
}

void StateTestEnemyShot::cleanup()
{
}

void StateTestEnemyShot::update(GameManager* gameManager)
{
	gameObjectManager->update();
	collisionManager->update();
	gameManager->getDebugTools()->update();
}
