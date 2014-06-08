#pragma once

#include "State.h"

class EventManager;
class GraphicsEngine;
class NetworkEngine;

class StateMenuCredits : public State
{
public:
	static StateMenuCredits* getInstance();
	virtual ~StateMenuCredits();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateMenuCredits();
	StateMenuCredits(const StateMenuCredits& orig);
	StateMenuCredits &operator= (const StateMenuCredits &);
	static StateMenuCredits* pInstance;

	EventManager* eventManager;
	GraphicsEngine* graphicsEngine;
	NetworkEngine* networkEngine;
};

