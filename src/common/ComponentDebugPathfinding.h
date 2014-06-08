#pragma once
#include "Component.h"
#include "Vector3d.h"
#include "Vector2d.h"

class NodeText;
class GraphicsEngine;
class DebugTools;
class EventManager;

class ComponentDebugPathfinding :
	public Component
{
public:
	ComponentDebugPathfinding(void);
	virtual ~ComponentDebugPathfinding(void);

	virtual void update();
	virtual void onMessage(const Message& message);

	
private:
	DebugTools* debugTools;
	EventManager* eventManager;
};
