#include "StateTestClient.h"

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
StateTestClient* StateTestClient::pInstance = NULL;

StateTestClient* StateTestClient::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateTestClient();

	return pInstance;
}

StateTestClient::StateTestClient()
{
	name = State::StateName::TEST_CLIENT;
}

StateTestClient::~StateTestClient()
{
}

void StateTestClient::init()
{
	/*gameManager = GameManager::getInstance();

	gameObjectManager = gameManager->getGameObjectManager();
	collisionManager = gameManager->getCollisionManager();
	networkEngine = gameManager->getNetworkEngine();


	//bool success = networkEngine->startClientAndAutoconnect();

	graphicsEngine = gameManager->getGraphicsEngine();
	mainPlayer = gameObjectManager->createPlayer();
	if(success)
	{
		graphicsEngine->setDebugText(L"Conectado");
		networkEngine->Reference(mainPlayer);
		networkEngine->setLobbyOn(false);

	} else
	{
		graphicsEngine->setDebugText(L"No se ha encontrado ningun server");
	}

	gameManager->getGraphicsEngine()->createLight();*/

}

void StateTestClient::cleanup()
{
	networkEngine->close();
	gameObjectManager->clear();
}

void StateTestClient::update(GameManager* gameManager)
{
	networkEngine->update();
	gameObjectManager->update();
	collisionManager->update();

	std::wstringstream string;

	/*string << "Ping: " << networkEngine->getLastPing() << "ms\n";

	graphicsEngine->setDebugText(string.str().c_str());*/

}
