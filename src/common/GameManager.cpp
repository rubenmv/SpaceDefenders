/*
GameManager.cpp
Manejador de estados del juego
*/
#include <iostream>
#include <string>

// Managers y herramientas
#include "GameManager.h"
#include "ResourceManager.h"
#include "EventManager.h"
#include "DebugTools.h"
#include "MapManager.h"
#include "AIEngine.h"
#include "AIFlocking.h"
#include "CollisionManager.h"
#include "GameObjectManager.h"
#include "AudioManager.h"
#include "PhysicsManager.h"
// Motores
#include "GraphicsEngine.h"
#include "GraphicsEngineIrrlicht.h"
#include "GraphicsEngineTAG.h"
#include "NetworkEngine.h"
// States
#include "State.h"
#include "StateIntro.h"
#include "StateServerLobby.h"
// Otros
#include "GameSettings.h"
#include "tests/StateTestOnline.h"
#include "tests/StateTestServer.h"

GameManager* GameManager::pInstance = NULL;

GameManager* GameManager::getInstance()
{
	if(pInstance == NULL)
	{
		pInstance = new GameManager();
	}

	return pInstance;
}

GameManager::GameManager()
{
	running = false;
	restart = false;
	server = false;
	currentState = NULL;
	debugMode = false;
	updateTime=-1;
}

GameManager::~GameManager()
{
	updateTime=0;
}

void GameManager::init()
{
	std::cout << "\nIniciando el GameManager\n\n";

	this->running = true;
	this->restart = false;

	// Engines y managers siempre activos
	networkEngine = new NetworkEngine();
	gameObjectManager = new GameObjectManager();

	if(isServer())
	{
		eventManager = NULL;
		graphicsEngine = NULL;
		debugTools = NULL;
		resourceManager = NULL;
		audioManager = NULL;
		changeState(StateServerLobby::getInstance());
		//changeState(StateTestServer::getInstance());
	} else
	{
		gameSettings = new GameSettings();
		gameSettings->loadSettings();
		// EventManager
		eventManager = new EventManager();

		// Motor de graficos
		if(gameSettings->isIrrlicht())
		{
			graphicsEngine = new GraphicsEngineIrrlicht();
		}
		else
		{
			resourceManager = new ResourceManager();
			graphicsEngine = new GraphicsEngineTAG();
		}

		bool successful = graphicsEngine->createWindow(eventManager, gameSettings);

		if(!successful)
		{
			exit(-1);
		}

		// AudioManager
		audioManager = new AudioManager();
		// Esto hace que el EventManager guarde punteros a los distintos
		// managers de los que hace uso constantemente
		eventManager->init();

		//changeState(StateTestOnline::getInstance());
		changeState(StateIntro::getInstance());

	}

	clock = Clock();
	deltaTime = 0;
}

// Cambia el estado e inicializa
void GameManager::changeState(State* newState)
{
	if(currentState != NULL)
	{
		std::cout << " :: Cerrando :: " << currentState->getName() << std::endl;
	}

	if(currentState != NULL)
	{
		currentState->cleanup();
		// Todos los estado por encima de ingame son tests ingame
		// Paso de imgame a menu
		if ((newState->getName() < State::StateName::IN_GAME &&
			currentState->getName() >= State::StateName::IN_GAME) ||
			currentState->getName() == State::StateName::INTRO ) 
		{
			// Esto primero, para que las componentes se desregistren
			networkEngine->close();
			gameObjectManager->clear();
			//delete gameObjectManager; // Esto ya hace el clear()
			//gameObjectManager = NULL;

			delete aiEngine;
			aiEngine=NULL;

			delete collisionManager;
			collisionManager=NULL;
			delete physicsManager;
			physicsManager = NULL;
			delete debugTools;
			debugTools=NULL;
		}
		// Limpia la pantalla
		if(!isServer())
		{
			graphicsEngine->clear();
		}
	}
	// currentState ahora es el nuevo
	currentState = newState;
	if(currentState != NULL)
	{
		std::cout << " :: Iniciando :: " << currentState->getName() << std::endl;

		// Todos los estado por encima de ingame son tests ingame
		// Paso a ingame
		if (currentState->getName() >= State::StateName::IN_GAME || 
			currentState->getName() == State::StateName::INTRO  )
		{
			//gameObjectManager = new GameObjectManager();
			collisionManager = new CollisionManager();
			if(!isServer() && gameSettings->usePhysics())
			{
				initPhysicsManager();
			}
			else
			{
				physicsManager = NULL;
			}
			aiEngine = new AIEngine();
			//aiFlocking = new AIFlocking();
			if(!isServer())
			{
				debugTools = new DebugTools();
			}
		}
		debugMode = false;
		currentState->init();
		if(audioManager != NULL)
		{
			audioManager->onStateChange(currentState->getName());
		}
		
	}
}

bool GameManager::isRunning()
{
	if(isServer())
	{
		return running;
	} else
	{
		return graphicsEngine->isRunning();
	}
}

// Arranca el bucle del manejador del juego
void GameManager::run()
{
	while (isRunning())
	{
		update();
	}
}

// Actualiza el juego
void GameManager::update()
{
	updateClockGM.start();

	deltaTime =   clock.restart()/1000.0;
	if(debugMode)
	{
		deltaTime = 0.016;
	}
	currentState->update(this);

	if(!isServer())
	{
		eventManager->cleanButtonsDowns();
		graphicsEngine->draw();
	}else
	{
		//Duerme el server hasta que pasa el frame
		int sleepTime = 8 - clock.getElapsedTime();
		if(sleepTime > 0)
		{
			Sleep(sleepTime);
		}
	//	std::cout<<"SleepTime: "<<sleepTime<<"\n";
	}

	updateTime=updateClockGM.getElapsedTime();
}

// Cierra el juego
void GameManager::quit()
{
	if(!isServer())
	{
		graphicsEngine->quit();
	} else
	{
		running = false;
	}

}

// Limpia
void GameManager::cleanup()
{
	running = false;
	/*
	if(!isServer())
	{
		gameSettings->saveSettings();
	}*/

	currentState->cleanup();
	currentState = NULL;
	// Hay que asegurarse de eliminar los managers de ingame
	// en el caso de que se reincie el juego desde una partida
	if (gameObjectManager != NULL)
	{
		gameObjectManager->clear();
		delete gameObjectManager;
		gameObjectManager = NULL;
	}

	if(networkEngine != NULL)
	{
		delete networkEngine;
		networkEngine = NULL;
	}

	if (aiFlocking != NULL)
	{
		delete aiFlocking;
		aiFlocking=NULL;
	}
	if (mapManager != NULL)
	{
		delete mapManager;
		mapManager=NULL;
	}
	if (physicsManager != NULL)
	{
		delete physicsManager;
		physicsManager=NULL;
	}
	if (collisionManager != NULL)
	{
		delete collisionManager;
		collisionManager=NULL;
	}
	if (audioManager != NULL)
	{
		delete audioManager;
		audioManager = NULL;
	}
	if (debugTools != NULL)
	{
		delete debugTools;
		debugTools=NULL;
	}
	if (resourceManager != NULL)
	{
		delete resourceManager;
		resourceManager = NULL;
	}

	delete networkEngine;
	networkEngine = NULL;
	delete eventManager;
	eventManager = NULL;
	delete gameSettings;
	gameSettings = NULL;
	if (!isServer())
	{
		delete graphicsEngine;
		graphicsEngine = NULL;
	}
}

unsigned int GameManager::getUpdateTime()
{
	return updateTime;
}

// Indica si el estado actual es de menu o no
bool GameManager::isMenuState()
{
	// Todos los estados por debajo de este son de menus
	return currentState->getName() < State::StateName::IN_GAME;
}

void GameManager::initMapManager()
{
	delete mapManager;
	mapManager=NULL;

	// El MapManager muestra una pantalla de carga
		mapManager = new MapManager();
}

void GameManager::initPhysicsManager()
{
	delete physicsManager;
	physicsManager=NULL;

	physicsManager = new PhysicsManager();
}
