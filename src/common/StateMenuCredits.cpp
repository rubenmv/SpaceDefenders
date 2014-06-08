#include <iostream>
#include <fstream>
#include "StateMenuCredits.h"
#include "StateServerBrowser.h"
#include "GraphicsEngine.h"
#include "EventManager.h"
#include "NetworkEngine.h"
#include "GUIManager.h"

// Singleton
StateMenuCredits* StateMenuCredits::pInstance = NULL;

StateMenuCredits* StateMenuCredits::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateMenuCredits();

	return pInstance;
}

StateMenuCredits::StateMenuCredits()
{
	name = State::StateName::MENU_CREDITS;
}

StateMenuCredits::~StateMenuCredits()
{
	// En el cleanup
}

void StateMenuCredits::init()
{
	gameManager = GameManager::getInstance();
	graphicsEngine = gameManager->getGraphicsEngine();
	networkEngine = gameManager->getNetworkEngine();
	eventManager = gameManager->getEventManager();
	// Menus
	graphicsEngine->createMenuCredits();
	graphicsEngine->setMenuSettings();
}

void StateMenuCredits::cleanup()
{
	graphicsEngine = NULL;
	eventManager = NULL;
	gameManager = NULL;
}

void StateMenuCredits::update(GameManager* gameManager)
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
