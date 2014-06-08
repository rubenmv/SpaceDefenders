#include <iostream>
#include <fstream>
#include "StateMenuTests.h"
#include "GUIManager.h"
#include "StateInGame.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "EventManager.h"
#include "StateMenuMain.h"

using namespace GUI;

// Singleton
StateMenuTests* StateMenuTests::pInstance = NULL;

StateMenuTests* StateMenuTests::getInstance(int page)
{
	if(pInstance == NULL)
	{
		pInstance = new StateMenuTests(page);
	}
	else
	{
		pInstance->page += page;
		if(pInstance->page >1)
			pInstance->page=0;
	}

	return pInstance;
}

StateMenuTests::StateMenuTests(int page)
{
	name = State::StateName::MENU_TESTS;

	this->page = page;
}

StateMenuTests::~StateMenuTests()
{
}

// Navega a una pagina con tests
void StateMenuTests::goToPage(int page)
{
	std::ifstream fichero("../data/server.dat");

	graphicsEngine->initMenuScreen();

	this->page = page;

	switch (page)
	{
	case 0:
		graphicsEngine->createMenuButton(0, L"BACK",BUTTON_ID::BACK);
		graphicsEngine->createMenuButton(1, L"GAME OBJECT",BUTTON_ID::GAME_OBJECT);
		graphicsEngine->createMenuButton(2, L"ONLINE",BUTTON_ID::ONLINE);
		graphicsEngine->createMenuButton(3, L"PATHFINDING",BUTTON_ID::PATHFINDING);
		break;
	default:
		break;
	}



	graphicsEngine->setMenuSettings();

	if(fichero)
		graphicsEngine->addReconnectButton();
	else
		graphicsEngine->addServerBrowserButton();
}

void StateMenuTests::init()
{
	gameManager = GameManager::getInstance();

	graphicsEngine = gameManager->getGraphicsEngine();

	eventManager = gameManager->getEventManager();

	goToPage(this->page);


}

void StateMenuTests::cleanup()
{
}

void StateMenuTests::update(GameManager* gameManager)
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
