#include <iostream>
#include <fstream>
#include "StateMenuOptions.h"
#include "StateInGame.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "EventManager.h"

#include "GUIManager.h" // IDs de botones
using namespace GUI;

// Singleton
StateMenuOptions* StateMenuOptions::pInstance = NULL;

StateMenuOptions* StateMenuOptions::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateMenuOptions();

	return pInstance;
}

StateMenuOptions::StateMenuOptions()
{
	name = State::StateName::MENU_OPTIONS;
}

StateMenuOptions::~StateMenuOptions()
{
}

void StateMenuOptions::init()
{
	gameManager = GameManager::getInstance();

	graphicsEngine = gameManager->getGraphicsEngine();
	eventManager = gameManager->getEventManager();
	gameSettings = gameManager->getGameSettings();

	graphicsEngine->createMenuSettings();
	graphicsEngine->setMenuSettings();

	fullscreen = gameSettings->getFullScreen();
	vsync = gameSettings->getSync();
	irrlicht = gameSettings->isIrrlicht();
	physics = gameSettings->usePhysics();
	aliasing = gameSettings->useAliasing();
	music = gameSettings->getVolumen();
}

void StateMenuOptions::cleanup()
{
	
}

void StateMenuOptions::update(GameManager* gameManager)
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

	switch(eventManager->getButtonDown())
	{
		case BUTTON_ID::RESOLUTION_NEXT:
			gameManager->getGameSettings()->nextResolution();
			graphicsEngine->updateResolution();
			break;
		case BUTTON_ID::RESOLUTION_BEFORE:
			gameManager->getGameSettings()->backResolution();
			graphicsEngine->updateResolution();
			break;
		case BUTTON_ID::V_SYNC:
			gameManager->getGameSettings()->changeVSync();
			break;
		case BUTTON_ID::FULLSCREEN:
			gameManager->getGameSettings()->changeFullScreen();
			break;
		case BUTTON_ID::IRRLICHT:
			gameManager->getGameSettings()->changeIrrlicht();
			break;
		case BUTTON_ID::PHYSICS:
			gameManager->getGameSettings()->changePhysics();
			break;
		case BUTTON_ID::ANTIALIASING:
			gameManager->getGameSettings()->changeAliasing();
			break;
		case BUTTON_ID::MUSIC:
			gameManager->getGameSettings()->changeVolume();
			break;
	}

	if (fullscreen != graphicsEngine->isFullScreenChecked())
	{
		gameManager->getGameSettings()->changeFullScreen();
		fullscreen = !fullscreen;
	}

	if (vsync != graphicsEngine->isVSyncChecked())
	{
		gameManager->getGameSettings()->changeVSync();
		vsync = !vsync;
	}

	if (irrlicht != graphicsEngine->isIrrlichtChecked())
	{
		gameManager->getGameSettings()->changeIrrlicht();
		irrlicht = !irrlicht;
	}

	if (physics != graphicsEngine->isPhysicsChecked())
	{
		gameManager->getGameSettings()->changePhysics();
		physics = !physics;
	}

	if (aliasing != graphicsEngine->isAliasingChecked())
	{
		gameManager->getGameSettings()->changeAliasing();
		aliasing = !aliasing;
	}

	if (music != graphicsEngine->isMusicChecked())
	{
		gameManager->getGameSettings()->changeVolume();
		music = !music;
	}
}
