#include <iostream>
#include "StateTestRevive.h"
#include "../GameManager.h"
#include "../GameObject.h"
#include "../GameObjectManager.h"
#include "../GraphicsEngine.h"
#include "../DebugTools.h"
#include "../EventManager.h"
#include "../AIFlocking.h"
// Singleton
StateTestRevive* StateTestRevive::pInstance = NULL;

StateTestRevive* StateTestRevive::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateTestRevive();

	return pInstance;
}

StateTestRevive::StateTestRevive()
{
	name = std::string("Test de revivir jugador");
}

StateTestRevive::~StateTestRevive()
{
}

void StateTestRevive::init()
{
	gameManager = GameManager::getInstance();

	std::vector<GameObject*> mainPlayers;
	std::vector<GameObject*> enemyPlayers;
	gameObjectManager = gameManager->getGameObjectManager();
	debugTools = gameManager->getDebugTools();
	debugTools->init();

	gameManager->getGraphicsEngine()->createLight();

	gameManager->getGraphicsEngine()->createLight();
	gameManager->getDebugTools()->init();

}

void StateTestRevive::cleanup()
{
}

void StateTestRevive::update(GameManager* gameManager)
{
	gameObjectManager->update();
	gameManager->getDebugTools()->update();
}
