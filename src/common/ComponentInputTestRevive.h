#pragma once
#include "Component.h"


class EventManager;
class GameObject;


class ComponentInput : public Component
{
public:
	ComponentInput(float speed);
	virtual ~ComponentInput(void);

	virtual void update();
	virtual void onMessage(Message message){};

private:
	EventManager* eventManager;
};

