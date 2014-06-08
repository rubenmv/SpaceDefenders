#pragma once

#include "State.h"

class EventManager;

class StateMenuMain : public State
{
public:
	static StateMenuMain* getInstance();
	virtual ~StateMenuMain();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateMenuMain();
	StateMenuMain(const StateMenuMain& orig);
	StateMenuMain &operator= (const StateMenuMain &);
	static StateMenuMain* pInstance;

	EventManager* eventManager;
	GraphicsEngine* graphicsEngine;

};

