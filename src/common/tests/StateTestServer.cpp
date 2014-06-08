#include "StateTestServer.h"
#include <iostream>
#include <vector>

#include "../GameObjectManager.h"
#include "../NetworkEngine.h"
#include "raknet/ReplicaManager3.h"

#include "../GameObject.h"
#include "../CollisionManager.h"
#include "../AIEngine.h"

// Singleton
StateTestServer* StateTestServer::pInstance = NULL;

StateTestServer* StateTestServer::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateTestServer();

	return pInstance;
}

StateTestServer::StateTestServer()
{
	name = State::StateName::TEST_SERVER;
}

StateTestServer::~StateTestServer()
{

}

void StateTestServer::init()
{
	gameManager = GameManager::getInstance();
	networkEngine = gameManager->getNetworkEngine();
	gameObjectManager = gameManager->getGameObjectManager();

	networkEngine->startServer();
	networkEngine->setLobbyOn(false);
	
	aiEngine =  gameManager->getAIEngine();

	GameObject* turret = gameManager->getGameObjectManager()->createTurret();
	turret->position = Vector2d(466,-349);

	turret = gameManager->getGameObjectManager()->createTurret();
	turret->position = Vector2d(537,-373);


	turret = gameManager->getGameObjectManager()->createTurret();
	turret->position = Vector2d(400,-150);

	turret = gameManager->getGameObjectManager()->createTurret();
	turret->position = Vector2d(430,-140);


	turret = gameManager->getGameObjectManager()->createTurret();
	turret->position = Vector2d(780,-450);

	turret = gameManager->getGameObjectManager()->createTurret();
	turret->position = Vector2d(750,-470);

	
}

void StateTestServer::cleanup()
{
	networkEngine->close();
}

void StateTestServer::update(GameManager* gameManager)
{
	networkEngine->update();
	gameObjectManager->update();
	
	gameManager->getCollisionManager()->update();
	aiEngine->update();

}

