#include "StateServerLobby.h"
#include <iostream>

#include "StateServerInGame.h"
#include "NetworkEngine.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"

// Singleton
StateServerLobby* StateServerLobby::pInstance = NULL;

StateServerLobby* StateServerLobby::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateServerLobby();

	return pInstance;
}

StateServerLobby::StateServerLobby()
{
	name = State::StateName::SERVER_LOBBY;
	std::cout << "Server lobby" << std::endl;
}

StateServerLobby::~StateServerLobby()
{
}

void StateServerLobby::init()
{
	gameManager = GameManager::getInstance();
	networkEngine = gameManager->getNetworkEngine();
	gameManager->initMapManager();

	networkEngine->startServer();
	networkEngine->setLobbyOn(true);
}

void StateServerLobby::cleanup()
{

}

void StateServerLobby::update(GameManager* gameManager)
{
	//gameManager->changeState(StateTestServer::getInstance());
	networkEngine->update();
}

