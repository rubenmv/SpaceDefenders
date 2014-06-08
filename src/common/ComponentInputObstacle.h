#pragma once
#include "Component.h"

class EventManager;
class GraphicsEngine;
class Clock;
class GameManager;

class ComponentInputObstacle : public Component
{
public:
	ComponentInputObstacle(void);
	~ComponentInputObstacle(void);

	virtual void update();
	virtual void onMessage(const Message& message);

private:
	EventManager* eventManager;
	GraphicsEngine* graphicsEngine;
	bool mousePressed;
};

