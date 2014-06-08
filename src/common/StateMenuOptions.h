#pragma once

#include "State.h"

class EventManager;

class StateMenuOptions : public State
{
public:
	static StateMenuOptions* getInstance();
	virtual ~StateMenuOptions();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateMenuOptions();
	StateMenuOptions(const StateMenuOptions& orig);
	StateMenuOptions &operator= (const StateMenuOptions &);
	static StateMenuOptions* pInstance;

	EventManager* eventManager;
	GraphicsEngine* graphicsEngine;
	GameSettings* gameSettings;

	bool fullscreen;
	bool vsync;
	bool irrlicht;
	bool physics;
	bool aliasing;
	bool music;
};

