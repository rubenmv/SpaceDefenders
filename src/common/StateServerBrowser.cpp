#include "StateServerBrowser.h"
#include <iostream>
#include "GUIManager.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "EventManager.h"
#include "StateMenuMain.h"
#include "StateMenuCreateGame.h"
#include "tests/StateClientInGame.h"

using namespace GUI;

// Singleton
StateServerBrowser* StateServerBrowser::pInstance = NULL;

StateServerBrowser* StateServerBrowser::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateServerBrowser();

	return pInstance;
}

StateServerBrowser::StateServerBrowser()
{
	name = State::StateName::MENU_SERVER_BROWSER;
}

StateServerBrowser::~StateServerBrowser()
{

}

void StateServerBrowser::init()
{
	gameManager = GameManager::getInstance();	
	GraphicsEngine* graphicsEngine = gameManager->getGraphicsEngine();	
	NetworkEngine* networkEngine = gameManager->getNetworkEngine();
	networkEngine->startClient();
	bool reconnected = networkEngine->tryToReconnect();

	if(reconnected)
	{
		gameManager->changeState(StateClientInGame::getInstance());
	} else
	{		
		graphicsEngine->createMenuServerBrowser();
		graphicsEngine->setMenuSettings();
		eventManager = GameManager::getInstance()->getEventManager();
		updateListBox();
	}	
}

void StateServerBrowser::cleanup()
{

}

void StateServerBrowser::update(GameManager* gameManager)
{
	// Aqui empieza el update
	NetworkEngine* networkEngine = gameManager->getNetworkEngine();
	networkEngine->update();

	switch(eventManager->getButtonDown())
	{
		case BUTTON_ID::REFRESH://Boton Actualizar
					updateListBox();
					break;
		case BUTTON_ID::JOIN://Boton Unirse	
					joinServer();
				break;
		case BUTTON_ID::CREATE_GAME:// Boton crear partida
			GameManager::getInstance()->changeState(StateMenuCreateGame::getInstance());
			break;
		case BUTTON_ID::CLOSE_SERVER:// Boton cerrar servidor
			networkEngine->closeServer();
			GameManager::getInstance()->changeState(StateMenuMain::getInstance());
			break;
	}
}

void StateServerBrowser::updateListBox()
{
	GraphicsEngine* graphicsEngine = gameManager->getGraphicsEngine();	
	graphicsEngine->clearServerTable();

	NetworkEngine* networkEngine = gameManager->getNetworkEngine();
	networkEngine->searchLanServers();
}

void StateServerBrowser::joinServer()
{
	NetworkEngine* networkEngine = gameManager->getNetworkEngine();
	GraphicsEngine* graphicsEngine = gameManager->getGraphicsEngine();	
	if(graphicsEngine->getSelectedTableServers() != 65535) //Devuelve 65535 si no hay nada seleccionado
	{
		//Aunque salga lleno en la ultima busqueda de servidores puede que en realidad no este lleno
		//Es mejor que intente conectarse y ya fallara si esta lleno de verdad

		/*if(networkEngine->getServerBrowser()->isServerFull(graphicsEngine->getSelectedTableServers()))
		{
			std::cout<<"ERROR: El servidor esta lleno\n";
			return;
		}*/
		networkEngine->joinServer(graphicsEngine->getSelectedTableServers());
		
	} else 
	{
		std::cout<<"ERROR: No hay un servidor seleccionado\n";
	}
}