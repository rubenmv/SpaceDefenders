#pragma once

#include "State.h"
#include "Clock.h"

class NetworkEngine;
class GameObjectManager;
class AIEngine;

class StateServerInGame : public State
{
public:
	static StateServerInGame* getInstance();
	virtual ~StateServerInGame();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateServerInGame();
	StateServerInGame(const StateServerInGame& orig);
	StateServerInGame &operator= (const StateServerInGame &);
	static StateServerInGame* pInstance;

	NetworkEngine* networkEngine;
	GameObjectManager* gameObjectManager;
	AIEngine* aiEngine;

	Clock clock;
};

