#pragma once

#include "State.h"

class EventManager;

class StateMenuTests : public State
{
public:
	static StateMenuTests* getInstance(int page);
	virtual ~StateMenuTests();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateMenuTests(int page);
	StateMenuTests(const StateMenuTests& orig);
	StateMenuTests &operator= (const StateMenuTests &);
	static StateMenuTests* pInstance;

	EventManager* eventManager;
	GraphicsEngine* graphicsEngine;

	void goToPage(int page);
	int page;
};

