#pragma once
#include "component.h"

class EventManager;
class GraphicsEngine;
class Clock;

/*	Recoge el clic izquierdo para situar un nuevo objetivo
*/
class ComponentInputTarget : public Component
{
public:
	ComponentInputTarget(void);
	virtual ~ComponentInputTarget(void);

	virtual void update();
	virtual void onMessage(const Message& message);
private:
	void onTargetChange(Vector2d target);

	EventManager* eventManager;
	GraphicsEngine* graphicsEngine;
	bool mousePressed;
};

