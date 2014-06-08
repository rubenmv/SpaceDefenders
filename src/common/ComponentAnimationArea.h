#pragma once
#include "component.h"
class ComponentAnimationArea :
	public Component
{
public:
	ComponentAnimationArea(int type, Vector2d position);
	~ComponentAnimationArea(void);

	void update();
	void onMessage(const Message& message);
private:
	float radius;
	float radiusMax;
	float time;
	Vector2d position;
	int type;
};

