/*
	GameManager.h
	Manejador de estados del juego
*/

#pragma once

#include "Clock.h"

class State;
class GraphicsEngine;
class NetworkEngine;
class EventManager;
class GameObjectManager;
class CollisionManager;
class MapManager;
class AIEngine;
class AIFlocking;
class DebugTools;
class GameSettings;
class ResourceManager;
class AudioManager;
class PhysicsManager;

class GameManager
{
public:
	static GameManager* getInstance();
	virtual~GameManager();

	void init();
	void cleanup();
	void run();
	void changeState(State* newState);
	void update();
	bool isRunning(); // Depende de graphicsEngine
	void quit();
	
	bool isServer()
	{
		return server;
	}

	bool isRestart()
	{
		return restart;
	}

	void setRestart(bool restart)
	{
		this->restart = restart;
	}

	//Tiene que ser llamado antes de init()
	void setIsServer(bool server)
	{
		this->server = server;
	}

	GraphicsEngine* getGraphicsEngine() 
	{
		return graphicsEngine;
	}

	ResourceManager* getResourceManager()
	{
		return resourceManager;
	}

	NetworkEngine* getNetworkEngine() 
	{
		return networkEngine;
	}
	EventManager* getEventManager() 
	{
		return eventManager;
	}

	GameObjectManager* getGameObjectManager()
	{
		return gameObjectManager;
	}

	CollisionManager* getCollisionManager()
	{
		return collisionManager;
	}

	MapManager* getMapManager()
	{
		return mapManager;
	}

	AudioManager* getAudioManager()
	{
		return audioManager;
	}

	AIEngine* getAIEngine()
	{
		return aiEngine;
	}

	AIFlocking* getAIFlocking()
	{
		return aiFlocking;
	}

	DebugTools* getDebugTools()
	{
		return debugTools;
	}

	GameSettings* getGameSettings()
	{
		return gameSettings;
	}
	PhysicsManager* getPhysicsManager()
	{
		return physicsManager;
	}
	double getDeltaTime()
	{
		if(deltaTime > 1) //Es para si se ha parado la ejecucion que no se vaya todo a la mierda por un deltaTime enorme 
		{
			return 1/60.0;
		} else
		{
			return deltaTime;
		}
	}

	void enableDebugMode()
	{
		debugMode = true;
	}
	unsigned int getUpdateTime();

	// Indica si el estado actual es menu o no (ingame)
	bool isMenuState();
	// Inicializa el mapa en stateintro
	void initMapManager();
	// Inicializa el mundo de Bullet
	void initPhysicsManager();
private:
	// Singleton
	GameManager();
	GameManager(const GameManager& orig);
	GameManager &operator= (const GameManager &);

	static GameManager* pInstance;

	State* currentState;
	// Indica si el gameManager esta arrancado
	bool running;
	// Indica si el juego se debe abrir de nuevo al intentar salir (ej. reiniciar para aplicar settings)
	bool restart;

	bool server;
	bool debugMode;
	Clock clock;
	double deltaTime;

	GraphicsEngine* graphicsEngine;
	ResourceManager* resourceManager;
	GameSettings* gameSettings;
	NetworkEngine* networkEngine;
	EventManager* eventManager;
	GameObjectManager* gameObjectManager;
	CollisionManager* collisionManager;
	AudioManager* audioManager;
	DebugTools* debugTools;
	MapManager* mapManager;
	AIEngine* aiEngine;
	AIFlocking* aiFlocking;
	PhysicsManager* physicsManager;

	Clock updateClockGM;
	int updateTime;
};
