#pragma once
#include "component.h"

class EventManager;
class GraphicsEngine;
class Clock;

/*	Recoge los controles de movimiento de camara libre
*/
class ComponentInputCamera : public Component
{
public:
	ComponentInputCamera(void);
	virtual ~ComponentInputCamera(void);

	virtual void update();
	virtual void onMessage(const Message& message);
private:
	EventManager* eventManager;
	float speed;
};

