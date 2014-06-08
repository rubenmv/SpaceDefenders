#pragma once

#include "Component.h"

#include "Clock.h"

class NodeSound;

class ComponentSound :public Component
{
public:
	ComponentSound(float rate, const char* soundPath);
	~ComponentSound();

	virtual void update();
	virtual void onMessage(const Message& message);
private:
	float rate;
	bool playing;
	Clock delay;

	NodeSound* sound;
};

