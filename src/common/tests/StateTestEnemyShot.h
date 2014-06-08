#pragma once

#include "../State.h"
#include "../CollisionManager.h"
class GameManager;
class GameObjectManager;
class MapManager;

class StateTestEnemyShot : public State
{


public:
	static StateTestEnemyShot* getInstance();
	virtual ~StateTestEnemyShot();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateTestEnemyShot();
	StateTestEnemyShot(const StateTestEnemyShot& orig);
	StateTestEnemyShot &operator= (const StateTestEnemyShot &);
	static StateTestEnemyShot* pInstance;

	MapManager* mapManager;

	//Deberia ir en el GameObjectManager
	GameObjectManager* gameObjectManager;
	CollisionManager* collisionManager;
};

