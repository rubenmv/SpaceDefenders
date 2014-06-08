#include "StateTestOnline.h"

#include <iostream>
#include <vector>
#include <sstream>
#include "../GraphicsEngine.h"
#include "../GameManager.h"
#include "../GameObject.h"
#include "../GameObjectManager.h"
#include "../NetworkEngine.h"
#include "../CollisionManager.h"

// Singleton
StateTestOnline* StateTestOnline::pInstance = NULL;

StateTestOnline* StateTestOnline::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateTestOnline();

	return pInstance;
}

StateTestOnline::StateTestOnline()
{

	name = State::StateName::TEST_BROWSER;
}

StateTestOnline::~StateTestOnline()
{
}

void StateTestOnline::init()
{
	gameManager = GameManager::getInstance();

	gameObjectManager = gameManager->getGameObjectManager();
	collisionManager = gameManager->getCollisionManager();
	networkEngine = gameManager->getNetworkEngine();

	graphicsEngine = gameManager->getGraphicsEngine();
	debugTools = gameManager->getDebugTools();

	mapManager = gameManager->getMapManager();  
	
	//graphicsEngine->createMenuServerBrowser();
	//graphicsEngine->createMenuClientLobby();

	networkEngine->startClient();
	//networkEngine->launchServer();	

	networkEngine->connectToServer("127.0.0.1");
	networkEngine->waitUntilConnected(1000);

	//networkEngine->sendToggleCharacterToServer(1);
	//networkEngine->sendToggleReadyToServer();
	//networkEngine->sendStartGameToServer();
	

	/*Clock timeout;

	while(timeout.getElapsedTime() < 100)
	{
		networkEngine->update();
	}
	graphicsEngine->clear();
	debugTools->init();
	*/
	

	GameObject* mainPlayer = gameObjectManager->createFighter();

	mainPlayer->position = Vector2d(548,-329);

	graphicsEngine->initWorld();
	graphicsEngine->createDebugMap(mapManager->getCollisionMap());
	//networkEngine->setMain(mainPlayer);
	networkEngine->update();
}

void StateTestOnline::cleanup()
{
	// Limpia los objetos del juego
	mapManager = NULL;
}

void StateTestOnline::update(GameManager* gameManager)
{

	gameObjectManager->update();
	collisionManager->update();
	debugTools->update();
	networkEngine->update();

	//std::wstringstream string;

	//string << "Ping: " << networkEngine->getLastPing() << "ms\n";

	//graphicsEngine->setDebugText(string.str().c_str());

}
