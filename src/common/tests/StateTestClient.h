#pragma once

#include "../State.h"
class GameManager;
class GameObject;
class GameObjectManager;
class NetworkEngine;
class CollisionManagerr;

class StateTestClient : public State
{
public:
	static StateTestClient* getInstance();
	virtual ~StateTestClient();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateTestClient();
	StateTestClient(const StateTestClient& orig);
	StateTestClient &operator= (const StateTestClient &);
	static StateTestClient* pInstance;

	GameObject* mainPlayer;
	GameObjectManager* gameObjectManager;
	CollisionManager* collisionManager;
	NetworkEngine* networkEngine;
	GraphicsEngine* graphicsEngine;
};

