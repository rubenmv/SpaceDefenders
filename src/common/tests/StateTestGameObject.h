#pragma once

#include "../State.h"
#include "../CollisionManager.h"
class GameManager;
class GameObjectManager;


class StateTestGameObject : public State
{
public:
	static StateTestGameObject* getInstance();
	virtual ~StateTestGameObject();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateTestGameObject();
	StateTestGameObject(const StateTestGameObject& orig);
	StateTestGameObject &operator= (const StateTestGameObject &);
	static StateTestGameObject* pInstance;

	//Deberia ir en el GameObjectManager
	GameObjectManager* gameObjectManager;
	CollisionManager* collisionManager;
};

