#pragma once

#include "../State.h"
#include "../Clock.h"

class NetworkEngine;
class GameObjectManager;
class AIEngine;

class StateTestServer : public State
{
public:
	static StateTestServer* getInstance();
	virtual ~StateTestServer();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateTestServer();
	StateTestServer(const StateTestServer& orig);
	StateTestServer &operator= (const StateTestServer &);
	static StateTestServer* pInstance;

	NetworkEngine* networkEngine;
	GameObjectManager* gameObjectManager;

	Clock clock;
	AIEngine* aiEngine;
};

