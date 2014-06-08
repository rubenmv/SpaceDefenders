#include <iostream>
#include <fstream>
#include "StateMenuMain.h"
#include "StateInGame.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "EventManager.h"

// Singleton
StateMenuMain* StateMenuMain::pInstance = NULL;

StateMenuMain* StateMenuMain::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateMenuMain();

	return pInstance;
}

StateMenuMain::StateMenuMain()
{
	name = State::StateName::MENU_MAIN;
}

StateMenuMain::~StateMenuMain()
{
}

void StateMenuMain::init()
{
	gameManager = GameManager::getInstance();

	graphicsEngine = gameManager->getGraphicsEngine();

	eventManager = gameManager->getEventManager();

	std::ifstream fichero("../data/server.dat");
	graphicsEngine->createMenuMain();
	graphicsEngine->setMenuSettings();

	if(fichero)
	{
		graphicsEngine->addReconnectButton();
	}
	else
	{
		graphicsEngine->addServerBrowserButton();
	}

}

void StateMenuMain::cleanup()
{
	graphicsEngine = NULL;
	eventManager = NULL;
	gameManager = NULL;
}

void StateMenuMain::update(GameManager* gameManager)
{
	if(eventManager->isKeyDown(KEY::KEY_DOWN))
	{
		graphicsEngine->setNextMenuFocus();
	}

	if(eventManager->isKeyDown(KEY::KEY_UP))
	{
		graphicsEngine->setNextMenuFocus(true);
	}

	if(eventManager->isKeyDown(KEY::KEY_LEFT))
	{
		graphicsEngine->setNextMenuFocus();
		graphicsEngine->setNextMenuFocus();
		graphicsEngine->setNextMenuFocus();
	}
	if(eventManager->isKeyDown(KEY::KEY_RIGHT))
	{
		graphicsEngine->setNextMenuFocus(true);
		graphicsEngine->setNextMenuFocus(true);
		graphicsEngine->setNextMenuFocus(true);
	}
}
