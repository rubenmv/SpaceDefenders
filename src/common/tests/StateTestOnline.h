
#pragma once

#include "../State.h"
class GameManager;
class GameObject;
class GameObjectManager;
class NetworkEngine;
class CollisionManager;

class StateTestOnline : public State
{
public:
	static StateTestOnline* getInstance();
	virtual ~StateTestOnline();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateTestOnline();
	StateTestOnline(const StateTestOnline& orig);
	StateTestOnline &operator= (const StateTestOnline &);
	static StateTestOnline* pInstance;

	std::string ipServer;

	GameObject* mainPlayer;
	GameObjectManager* gameObjectManager;
	CollisionManager* collisionManager;
	NetworkEngine* networkEngine;
	GraphicsEngine* graphicsEngine;
	DebugTools* debugTools;
	MapManager* mapManager;

};

