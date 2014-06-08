#pragma once

#include "../State.h"
class GameManager;
class GameObject;
class GameObjectManager;
class NetworkEngine;
class CollisionManager;

class StateClientInGame : public State
{
public:
	static StateClientInGame* getInstance();
	virtual ~StateClientInGame();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateClientInGame();
	StateClientInGame(const StateClientInGame& orig);
	StateClientInGame &operator= (const StateClientInGame &);
	static StateClientInGame* pInstance;

	std::string ipServer;

	GameObjectManager* gameObjectManager;
	CollisionManager* collisionManager;
	NetworkEngine* networkEngine;
	GraphicsEngine* graphicsEngine;
	DebugTools* debugTools;
	MapManager* mapManager;
	PhysicsManager* physicsManager;

};

