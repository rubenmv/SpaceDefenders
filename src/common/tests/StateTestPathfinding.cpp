#include <iostream>
#include "StateTestPathfinding.h"
#include "../GameManager.h"
#include "../GameObject.h"
#include "../GameObjectManager.h"
#include "../GraphicsEngine.h"
#include "../DebugTools.h"
#include "../EventManager.h"
#include "../AIFlocking.h"
// Singleton
StateTestPathfinding* StateTestPathfinding::pInstance = NULL;

StateTestPathfinding* StateTestPathfinding::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateTestPathfinding();

	return pInstance;
}

StateTestPathfinding::StateTestPathfinding()
{
	name = State::StateName::TEST_PATHFINDING;
}

StateTestPathfinding::~StateTestPathfinding()
{
	// Usar el metodo de cleanup para limpiar
}

void StateTestPathfinding::cleanup()
{
	// Limpia los objetos del juego
	gameObjectManager = NULL;
	mapManager = NULL;
}

void StateTestPathfinding::init()
{
	gameManager = GameManager::getInstance();

	mapManager = gameManager->getMapManager();

	std::vector<GameObject*> mainPlayers;
	std::vector<GameObject*> enemyPlayers;

	gameObjectManager = gameManager->getGameObjectManager();

	gameManager->getGraphicsEngine()->initWorld();
	gameManager->getGraphicsEngine()->createDebugMap(mapManager->getCollisionMap());

	aiFlocking = gameManager->getAIFlocking();

	debugTools = gameManager->getDebugTools();

	//gameObjectManager->createTurret();
	mainPlayers.push_back(gameObjectManager->createPlayer());
	/*enemyPlayers.push_back(gameObjectManager->createEnemyPathfinder());
	enemyPlayers.push_back(gameObjectManager->createEnemySteering());
	enemyPlayers.push_back(gameObjectManager->createEnemySteering());
	enemyPlayers.push_back(gameObjectManager->createEnemySteering());
	enemyPlayers.push_back(gameObjectManager->createEnemySteering());
	enemyPlayers.push_back(gameObjectManager->createEnemySteering());
	enemyPlayers.push_back(gameObjectManager->createEnemySteering());
	enemyPlayers.push_back(gameObjectManager->createEnemySteering());
	enemyPlayers.push_back(gameObjectManager->createEnemySteering());*/

	for(unsigned int i = 0; i < enemyPlayers.size(); i++)
	{
		aiFlocking->addEnemy(enemyPlayers[i]);
	}

	for(unsigned int i = 0; i < mainPlayers.size(); i++)
	{
		mainPlayers[i]->position = Vector2d(160,-160);
		aiFlocking->addPlayer(mainPlayers[i]);
	}

	//gameObjectManager->createTarget();

	gameManager->getGraphicsEngine()->createMenuPathfinding();
}



void StateTestPathfinding::update(GameManager* gameManager)
{
	// Input
	switch(gameManager->getEventManager()->getButtonDown())
	{
		case 350:// Boton reiniciar mapa
			// Eliminamos del mapa que guarda la IA
			mapManager->initMap();
			// Eliminamos los cubos
			debugTools->removeObstacles();
		break;
	}


	aiFlocking->update();
	gameObjectManager->update();
	gameManager->getDebugTools()->update();
}
