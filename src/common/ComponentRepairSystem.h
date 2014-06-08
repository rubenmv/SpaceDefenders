#pragma once
#include "Component.h"

class GameObect;

class ComponentRepairSystem : public Component
{
public:
	ComponentRepairSystem(void);
	~ComponentRepairSystem(void);

	virtual void update();
	virtual void onMessage(const Message& message);
	void onNewVision(GameObject * gameObject);
	void onLastVision( GameObject* gameObject );
private:

	float speed;
	float acceleration;
	GameObject *player;
};

