#include <iostream>
#include <fstream>
#include "StateMenuCreateGame.h"
#include "StateServerBrowser.h"
#include "GraphicsEngine.h"
#include "EventManager.h"
#include "NetworkEngine.h"
#include "GUIManager.h"

// Singleton
StateMenuCreateGame* StateMenuCreateGame::pInstance = NULL;

StateMenuCreateGame* StateMenuCreateGame::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateMenuCreateGame();

	return pInstance;
}

StateMenuCreateGame::StateMenuCreateGame()
{
	name = State::StateName::MENU_CREATE_GAME;
}

StateMenuCreateGame::~StateMenuCreateGame()
{
	// En el cleanup
}

void StateMenuCreateGame::init()
{
	gameManager = GameManager::getInstance();
	graphicsEngine = gameManager->getGraphicsEngine();
	networkEngine = gameManager->getNetworkEngine();
	eventManager = gameManager->getEventManager();
	// Menus
	graphicsEngine->createMenuCreateGame();
	graphicsEngine->setMenuSettings();
}

void StateMenuCreateGame::cleanup()
{
	graphicsEngine = NULL;
	eventManager = NULL;
	gameManager = NULL;
}

void StateMenuCreateGame::update(GameManager* gameManager)
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

	// Crea el juego
	if (eventManager->getButtonDown() == GUI::BUTTON_ID::LAUNCH_SERVER)
	{
		networkEngine->launchServer();
		// Devuelve al serverBrowser que le conectara en cuanto este disponible el servidor
		GameManager::getInstance()->changeState(StateServerBrowser::getInstance());
	}
}
