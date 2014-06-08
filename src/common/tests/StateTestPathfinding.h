#pragma once

#include "../State.h"
#include "../MapManager.h"

class GameManager;
class GameObjectManager;

class StateTestPathfinding : public State
{
public:
	static StateTestPathfinding* getInstance();
	virtual ~StateTestPathfinding();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateTestPathfinding();
	StateTestPathfinding(const StateTestPathfinding& orig);
	StateTestPathfinding &operator= (const StateTestPathfinding &);
	static StateTestPathfinding* pInstance;

	GameObjectManager* gameObjectManager;
	MapManager* mapManager;
	DebugTools* debugTools;
	AIFlocking* aiFlocking;
};

