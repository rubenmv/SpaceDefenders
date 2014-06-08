#pragma once
#include "component.h"
#include "Clock.h"
class GameObject;

class ComponentCreatingWall :	public Component
{
public:
	ComponentCreatingWall(GameObject* creator1, GameObject* creator2);
	~ComponentCreatingWall(void);

	void update();
	void onMessage(const Message& message);

private:
	GameObject* creator1;
	GameObject* creator2;
	Vector2d start;
	Vector2d end;
	Clock timeBuild;
};

