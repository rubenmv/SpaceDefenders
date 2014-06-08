#include "StateClientInGame.h"

#include <iostream>
#include <vector>
#include <sstream>
#include "../GraphicsEngine.h"
#include "../GameManager.h"
#include "../GameObject.h"
#include "../GameObjectManager.h"
#include "../NetworkEngine.h"
#include "../CollisionManager.h"
#include "../PhysicsManager.h"
#include "../StateMenuMain.h"

// Singleton
StateClientInGame* StateClientInGame::pInstance = NULL;

StateClientInGame* StateClientInGame::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateClientInGame();

	return pInstance;
}

StateClientInGame::StateClientInGame()
{
	name = State::StateName::TEST_BROWSER;
}

StateClientInGame::~StateClientInGame()
{
}

void StateClientInGame::init()
{
	gameManager = GameManager::getInstance();

	gameObjectManager = gameManager->getGameObjectManager();
	collisionManager = gameManager->getCollisionManager();
	networkEngine = gameManager->getNetworkEngine();
	physicsManager = gameManager->getPhysicsManager();
	graphicsEngine = gameManager->getGraphicsEngine();
	debugTools = gameManager->getDebugTools();

	networkEngine->giveMeCharacter();
	
	mapManager = gameManager->getMapManager();	
	gameManager->getGraphicsEngine()->createDebugMap(mapManager->getCollisionMap());
	gameObjectManager->createMap();


	Clock timeoutClock;
	GameObject* mainPlayer = NULL;
	//El servidor si detecta que nos hemos reconectado,
	//nos enviara el jugador que teniamos antes.
	while(mainPlayer == NULL && timeoutClock.getElapsedTime() < 300)
	{
		networkEngine->update();
		mainPlayer = networkEngine->getMain();
	}

	if(mainPlayer == NULL) 
	{
		//El servidor no ha mandado ningun player por lo que no es una reconexion
		//Por lo que hay que coger el tipo de nave elegida en el lobby
		int character = networkEngine->getCharacter();

		switch (character)
		{
		case 1:
			mainPlayer = gameObjectManager->createBomber();
			break;
		case 2:
			mainPlayer = gameObjectManager->createEngineer();
			break;
		case 3:
			mainPlayer = gameObjectManager->createBattleship();
			break;
		case 4:
			mainPlayer = gameObjectManager->createFighter();
			break;
		default:
			//No se ha pasado por lobby, el problema ha sido que el servidor no le ha dado tiempo a responder
			//Se vuelve a pedir al server y si sigue sin responder se manda al jugador al menu
			timeoutClock.restart();
			while(mainPlayer == NULL && timeoutClock.getElapsedTime() < 300)
			{
				networkEngine->update();
				mainPlayer = networkEngine->getMain();
			}

			if(mainPlayer == NULL)
			{
				networkEngine->close();
				gameManager->changeState(StateMenuMain::getInstance());
			}
		}

		networkEngine->setMain(mainPlayer);

	}
	mainPlayer->position = Vector2d(548 + Math::random(-20,20), -329 + Math::random(-20,20));
	gameManager->getGraphicsEngine()->initWorld();
}

void StateClientInGame::cleanup()
{
	// Limpia los objetos del juego
}

void StateClientInGame::update(GameManager* gameManager)
{
	if(physicsManager != NULL)	physicsManager->update();
	gameObjectManager->update();
	collisionManager->update();
	debugTools->update();

	std::wstringstream string;
	networkEngine->update();

	//string << "Ping: " << networkEngine->getLastPing() << "ms\n";

	//graphicsEngine->setDebugText(string.str().c_str());

}
