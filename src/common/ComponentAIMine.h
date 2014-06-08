#pragma once
#include "component.h"


#include "Clock.h"


class ComponentAIMine :	public Component
{
public:
	ComponentAIMine(float explosionTime);
	virtual ~ComponentAIMine(void);
	
	virtual void onMessage(const Message& message);
	virtual void update();

private:
	void countdown(GameObject* target);
	float explosionTime;
	bool activated;
	Clock clock;
	
};

