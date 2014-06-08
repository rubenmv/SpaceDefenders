#pragma once
#include "Component.h"

#include "Clock.h"

class ComponentMinesGenerator :	public Component
{
public:
	ComponentMinesGenerator(float newMineTime);
	virtual ~ComponentMinesGenerator(void);

	virtual void onMessage(const Message& message);
	virtual void update();

protected:

	float newMineTime;
	Clock clock;
};

