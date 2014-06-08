#pragma once
#include "Component.h"
#include "Vector3d.h"
#include "Vector2d.h"

class NodeText;
class GraphicsEngine;

class ComponentDebugGameObject : public Component
{
public:
	ComponentDebugGameObject(void);
	virtual ~ComponentDebugGameObject(void);
	
	virtual void update();
	virtual void onMessage(const Message& message);
private:
	bool showPosition;
	Vector2d relativeTextPosition;
	GraphicsEngine* graphicsEngine;
	NodeText* node;

	float health;
	float shield;
	int jade;
	void onHealthChanged(float newHealth);
	void onShieldChanged(float newShield);
	void onJadeChanged(int newJade);
};
