#pragma once
#include "Component.h"

class EventManager;
class GraphicsEngine;
class Clock;
class GameManager;

class ComponentInputDebugPathfinding : public Component
{
public:
	ComponentInputDebugPathfinding(void);
	~ComponentInputDebugPathfinding(void);

	virtual void update();
	virtual void onMessage(Message message);

private:
	EventManager* eventManager;
	GraphicsEngine* graphicsEngine;
};

