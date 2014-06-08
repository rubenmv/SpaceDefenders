#pragma once
#include "Component.h"

class GameObject;


class ComponentTargetAim :	public Component
{
public:
	ComponentTargetAim(void);
	virtual ~ComponentTargetAim(void);

	virtual void update();
	virtual void onMessage(const Message& message);

private:
	GameObject* gameObjectTarget;
	bool shooting;
	bool aiming;
	void onNewAim(GameObject* newTarget);
	void stopAim();
};

