#pragma once
#include "Component.h"
#include "Clock.h"

class ComponentAutoDestroy : public Component
{
public:
	ComponentAutoDestroy(float autoDestroyTime);
	virtual ~ComponentAutoDestroy(void){};

	void update();

	void onMessage(const Message& message){};

private:
	Clock clock;
	float autoDestroyTime;
};

