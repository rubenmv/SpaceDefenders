#include "StateTestWaves.h"
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
#include "../PhysicsManager.h"
// Singleton
StateTestWaves* StateTestWaves::pInstance = NULL;
const int StateTestWaves::ENEMY_MULTIPLIER=2;
const int StateTestWaves::INITIAL_ENEMIES=4;
const int StateTestWaves::DEFEND_TIME=3000; //3 segundos


StateTestWaves* StateTestWaves::getInstance(int character)
{
	if(pInstance == NULL)
		pInstance = new StateTestWaves(character);

	return pInstance;
}


StateTestWaves::StateTestWaves(int character)
{
	name = State::StateName::TEST_WAVES;
	this->character = character;
}



StateTestWaves::~StateTestWaves()
{
	mapManager = NULL;
}


void StateTestWaves::init()
{
	gameManager = GameManager::getInstance();

	gameObjectManager = gameManager->getGameObjectManager();
	collisionManager = gameManager->getCollisionManager();
	
	physicsManager = gameManager->getPhysicsManager();
	
	mapManager = gameManager->getMapManager();
	gameManager->getGraphicsEngine()->createDebugMap(mapManager->getCollisionMap());

	
	//gameManager->getDebugTools()->init();

	aiEngine =  gameManager->getAIEngine();

	gameObjectManager->createMap();
	gameObjectManager->createWholeBase(Vector2d(180,-760));

	GameObject* player = NULL;
	
	switch (character)
	{
	case 0:
		player = gameObjectManager->createBattleship();
		break;
	case 1:
		player = gameObjectManager->createFighter();
		break;
	case 2:
		player = gameObjectManager->createEngineer();
		break;
	case 3:
		player = gameObjectManager->createBomber();
		break;
	}

	player->position = Vector2d(548,-329);
	/*GameObject* basic = gameObjectManager->createEnemyBasic();	
	basic->position = Vector2d(548,-329);*/
//	GameObject* bomber = gameObjectManager->createEnemyBomber();	
//	bomber->position = Vector2d(548,-329);
//	gameObjectManager->createFuryEnemies();	

	gameManager->getGraphicsEngine()->initWorld();

}


void StateTestWaves::cleanup()
{
	pInstance = NULL;
}

void StateTestWaves::update(GameManager* gameManager)
{
	/*
	std::wstringstream string; //Para escribir por pantalla
	//Mostramos por pantalla
	string << "Oleada #"<<aiEngine->getLevelRound()<<"\n";
	string << "Enemigos vivos: "<< aiEngine->getNumEnemiesWave()<<"\n";
	string << "Enemigos totales creados: "<<aiEngine->getTotalEnemiesRoundInGame()<<"\n";
	string << "Enemigos totales de la ronda: "<<aiEngine->getTotalEnemiesRound()<<"\n";
	
	string << "\nPresiona la tecla K para elimnar a todos los enemigos\n";
	gameManager->getGraphicsEngine()->setDebugText(string.str().c_str()); //Insertamos el texto
	
	*/
	if(physicsManager != NULL)	physicsManager->update();
	gameObjectManager->update();
	collisionManager->update();
	//gameManager->getDebugTools()->update();
	aiEngine->update();

	/*
	if(gameManager->getEventManager()->isKeyDown(KEY::KEY_KEY_K))
	{
		gameObjectManager->killAllEnemies();		
	}
	*/

	/*if(gameManager->getEventManager()->isKeyDown(KEY::KEY_KEY_Z))
	{
		gameObjectManager->createBasicDebris(Vector2d(2,2),GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());		
	}

	if(gameManager->getEventManager()->isKeyDown(KEY::KEY_KEY_X))
	{
		gameObjectManager->createGenericDebris("",Vector2d(0,0),GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());	
	}*/
}


void StateTestWaves::createWaveEnemies(int numberOfEnemies)
{
	for(int i=0; i<numberOfEnemies; i++)
	{
		GameObject* basic = gameObjectManager->createEnemyBasic();
		basic->position = Vector2d(762,-135+i/10.0);
		
	}
	stateOfWave=1;//pongo el estado a 1;	
}

void StateTestWaves::createPositionPath()
{
	/////A
	//GameObject* positionPath = gameObjectManager->createEnemyStill();
	//positionPath->position = Vector2d(762,-135);
	/////B

	
	GameObject* positionPath = gameObjectManager->createEnemyStill();
	positionPath->position = Vector2d(781,-112);
	positionPath = gameObjectManager->createEnemyStill();
	positionPath->position = Vector2d(832,-59);
	positionPath = gameObjectManager->createEnemyStill();
	positionPath->position = Vector2d(487,-88);
	positionPath = gameObjectManager->createEnemyStill();
	positionPath->position = Vector2d(613,-290);
	positionPath = gameObjectManager->createEnemyStill();
	positionPath->position = Vector2d(806, -460);
	/////C
	positionPath = gameObjectManager->createEnemyStill();
	positionPath->position = Vector2d(241,-226);
	positionPath = gameObjectManager->createEnemyStill();
	positionPath->position = Vector2d(385,-373);
	positionPath = gameObjectManager->createEnemyStill();
	positionPath->position = Vector2d(562,-564);
	positionPath = gameObjectManager->createEnemyStill();
	positionPath->position = Vector2d(700, -777);

	/////D
	positionPath = gameObjectManager->createEnemyStill();
	positionPath->position = Vector2d(120,-465);
	positionPath = gameObjectManager->createEnemyStill();
	positionPath->position = Vector2d(363,-599);
	positionPath = gameObjectManager->createEnemyStill();
	positionPath->position = Vector2d(441,-798);
	/////E
	positionPath = gameObjectManager->createEnemyStill();
	positionPath->position = Vector2d(142,-777);


}

