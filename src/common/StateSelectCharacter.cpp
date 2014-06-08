#include "StateSelectCharacter.h"
#include "GraphicsEngine.h"
#include "GUIManager.h"
#include "GameObjectManager.h"
#include "EventManager.h"
#include "StateServerBrowser.h"
#include "Tests/StateTestWaves.h"
#include "StateMenuMain.h"

using namespace GUI;

// Singleton
StateSelectCharacter* StateSelectCharacter::pInstance = NULL;

StateSelectCharacter* StateSelectCharacter::getInstance()
{
	//static StateSelectCharacter Instance;
	if(pInstance == NULL)
		pInstance = new StateSelectCharacter();
	
	return pInstance;
	//return &Instance;
}

StateSelectCharacter::StateSelectCharacter()
{
	name = State::StateName::MENU_CREATE_GAME;
	character = -1;
	selected=false;
}

StateSelectCharacter::~StateSelectCharacter()
{

}

void StateSelectCharacter::init()
{
	gameManager = GameManager::getInstance();

	GraphicsEngine* graphicsEngine = gameManager->getGraphicsEngine();
	eventManager = gameManager->getEventManager();
	graphicsEngine->createMenuSelectCharacter();
	graphicsEngine->setMenuSettings();
	selected = false;
}

void StateSelectCharacter::cleanup()
{
	//Da problemas si alguien se cuela en el servidor antes de tiempo
	
	//gameManager->getGameObjectManager()->clear();
	//gameManager->getGraphicsEngine()->clear();
}

void StateSelectCharacter::update(GameManager* gameManager)
{
	// Aqui empieza el update

	switch(eventManager->getButtonDown())
	{
	case BUTTON_ID::DISCONNECT://Boton Desconectar
		GameManager::getInstance()->changeState(StateMenuMain::getInstance());
		return;
		break;
	case BUTTON_ID::BATTLESHIP:
		character = 0;
		selected=true;
		break;
	case BUTTON_ID::FIGHTER:
		character = 1;
		selected=true;
		break;
	case BUTTON_ID::ENGINEER:
		character = 2;
		selected=true;
		break;
	case BUTTON_ID::BOMBER:
		character = 3;
		selected=true;
		break;

	}

	if(selected) GameManager::getInstance()->changeState(StateTestWaves::getInstance(character));
}
