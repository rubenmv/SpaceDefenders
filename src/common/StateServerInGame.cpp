#include "StateServerInGame.h"
#include <iostream>

#include "GameObjectManager.h"
#include "NetworkEngine.h"
#include "raknet/ReplicaManager3.h"

#include <vector>
#include "GameObject.h"
#include "CollisionManager.h"
#include "AIEngine.h"

// Singleton
StateServerInGame* StateServerInGame::pInstance = NULL;

StateServerInGame* StateServerInGame::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateServerInGame();

	return pInstance;
}

StateServerInGame::StateServerInGame()
{
	name = State::StateName::TEST_SERVER;
}

StateServerInGame::~StateServerInGame()
{

}

void StateServerInGame::init()
{
	gameManager = GameManager::getInstance();
	networkEngine = gameManager->getNetworkEngine();
	gameObjectManager = gameManager->getGameObjectManager();
	networkEngine->setLobbyOn(false);

	aiEngine =  gameManager->getAIEngine();
	
	gameObjectManager->createWholeBase(Vector2d(180,-760));
}

void StateServerInGame::cleanup()
{
	networkEngine->close();
}

void StateServerInGame::update(GameManager* gameManager)
{
	networkEngine->update();
	gameManager->getCollisionManager()->update();
	gameObjectManager->update();
	aiEngine->update();

}

