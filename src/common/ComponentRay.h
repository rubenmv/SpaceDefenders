#pragma once
#include "component.h"
#include "Clock.h"
class ComponentRay :
	public Component
{
public:
	ComponentRay(GameObject* shooter);
	~ComponentRay(void);

	void update();
	void onMessage(const Message& message);
private:
	Vector2d upright;
	Vector2d upleft;
	Vector2d downright;
	Vector2d downleft;
	GameObject* shooter;
	Clock canHurt;
};

