#pragma once
#include "component.h"

class GameObject;
class NodeMesh;

class ComponentEnergyWall :	public Component
{
public:
	ComponentEnergyWall(Vector2d start,Vector2d end);
	~ComponentEnergyWall(void);

	void update();
	void onMessage(const Message& message);
	void removeBresenham();
	void bresenham();
private:
	GameObject* wallBegin;
	GameObject* wallEnd;
	NodeMesh* energy;
	bool energyActivated;
};

