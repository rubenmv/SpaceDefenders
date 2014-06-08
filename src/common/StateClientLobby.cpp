#include "StateClientLobby.h"
#include "GraphicsEngine.h"
#include "GUIManager.h"
#include "GameObjectManager.h"
#include "NetworkEngine.h"
#include "EventManager.h"
#include "StateServerBrowser.h"
#include "Tests/StateClientInGame.h"

using namespace GUI;

// Singleton
StateClientLobby* StateClientLobby::pInstance = NULL;

StateClientLobby* StateClientLobby::getInstance()
{
	//static StateClientLobby Instance;
	if(pInstance == NULL)
		pInstance = new StateClientLobby();
	
	return pInstance;
	//return &Instance;
}

StateClientLobby::StateClientLobby()
{
	name = State::StateName::MENU_CLIENT_LOBBY;
	ready = false;
	character = -1;
	selected=false;
}

StateClientLobby::~StateClientLobby()
{

}

void StateClientLobby::init()
{
	gameManager = GameManager::getInstance();

	GraphicsEngine* graphicsEngine = gameManager->getGraphicsEngine();
	networkEngine = gameManager->getNetworkEngine();
	eventManager = gameManager->getEventManager();
	graphicsEngine->createMenuClientLobby();
	graphicsEngine->setMenuSettings();
	ready = false;
}

void StateClientLobby::cleanup()
{
	//Da problemas si alguien se cuela en el servidor antes de tiempo
	
	//gameManager->getGameObjectManager()->clear();
	//gameManager->getGraphicsEngine()->clear();
}

void StateClientLobby::update(GameManager* gameManager)
{
	// Aqui empieza el update

	switch(eventManager->getButtonDown())
	{
	case BUTTON_ID::DISCONNECT://Boton Desconectar
		ready=false;
		networkEngine->sendToggleReadyToServer();
		networkEngine->setLobbyOn(false);
		networkEngine->close();
		GameManager::getInstance()->changeState(StateServerBrowser::getInstance());
		return;
		break;
	case BUTTON_ID::START://Boton Comenzar
		networkEngine->sendStartGameToServer();
		//GameManager::getInstance()->changeState(StateTestBrowser::getInstance()); //Cambia de estado el networkEngine
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

	GraphicsEngine* graphicsEngine = gameManager->getGraphicsEngine();
	if(ready)
	{
		if(!graphicsEngine->isChecked())
		{
			ready = false;
			networkEngine->sendToggleReadyToServer();
		}
	}else{
		if(graphicsEngine->isChecked())
		{
			ready = true;
			networkEngine->sendToggleReadyToServer();
		}
	}
	
	if(selected)
	{
		networkEngine->sendToggleCharacterToServer(character);
		selected=false;
	}

	networkEngine->update();

}
