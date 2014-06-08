#pragma once

#include "State.h"

class EventManager;
class GraphicsEngine;
class NetworkEngine;

class StateMenuCreateGame : public State
{
public:
	static StateMenuCreateGame* getInstance();
	virtual ~StateMenuCreateGame();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateMenuCreateGame();
	StateMenuCreateGame(const StateMenuCreateGame& orig);
	StateMenuCreateGame &operator= (const StateMenuCreateGame &);
	static StateMenuCreateGame* pInstance;

	EventManager* eventManager;
	GraphicsEngine* graphicsEngine;
	NetworkEngine* networkEngine;
};

