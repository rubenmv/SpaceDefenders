#pragma once

#include "../State.h"

class GameManager;
class GameObjectManager;

class StateTestRevive :	public State
{
public:
	static StateTestRevive* getInstance();
	virtual ~StateTestRevive();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateTestRevive();
	StateTestRevive(const StateTestRevive& orig);
	StateTestRevive &operator= (const StateTestRevive &);
	static StateTestRevive* pInstance;

	GameObjectManager* gameObjectManager;
	DebugTools* debugTools;
};

