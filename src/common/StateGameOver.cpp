#include "StateGameOver.h"
#include <iostream>
#include <fstream>
#include "StateInGame.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "EventManager.h"
#include "clock.h"
#include "AIEngine.h"
// Singleton
StateGameOver* StateGameOver::pInstance = NULL;

StateGameOver* StateGameOver::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateGameOver();


	return pInstance;
}

StateGameOver::StateGameOver()
{
	name = State::StateName::GAME_OVER;
}

StateGameOver::~StateGameOver()
{
}

void StateGameOver::init()
{
	
	gameManager = GameManager::getInstance();
	AIEngine* aiEngine = gameManager->getAIEngine();

	if(aiEngine != NULL)
	{
		levelRound = aiEngine->getLevelRound();
		wastedTime = aiEngine->getGameTime();
		wastedTime.pause();
		enemiesKilled = aiEngine->getTotalEnemiesKilled();
	}

	graphicsEngine = gameManager->getGraphicsEngine();

	eventManager = gameManager->getEventManager();

	gameManager->getNetworkEngine()->removeServerFile();

	graphicsEngine->createMenuGameOver(levelRound,wastedTime,enemiesKilled);
	graphicsEngine->setMenuSettings();
}

void StateGameOver::cleanup()
{
	graphicsEngine = NULL;
	eventManager = NULL;
	gameManager = NULL;
}

void StateGameOver::update(GameManager* gameManager)
{

}
