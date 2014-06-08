#pragma once

#include "Component.h"


class ComponentBehaviour : public Component
{
public:
	ComponentBehaviour();
	~ComponentBehaviour();
	virtual void update();

	virtual void onMessage(const Message& message);
	void calculateCloseWallForce();
	void calculateWallForce();

private:

	bool aiming;
};

