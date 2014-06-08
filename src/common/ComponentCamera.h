#pragma once
#include "Component.h"
#include "Vector3d.h"

class NodeCamera;
class EventManager;

const float MAX_SPEED = 5.0;
const float ACCELERATION = 1;
const float FRICTION = 0.9;
const float MAX_ZOOM = 200.0;
const float MIN_ZOOM = 30.0;

class ComponentCamera :	public Component
{
public:
	ComponentCamera(Vector3d relativePosition);
	virtual ~ComponentCamera(void);

	virtual void update();
	virtual void onMessage(const Message& message);
	void onNewVision(GameObject *gameObject);
	void onLostVision( GameObject* gameObject );
	void awake();
	void shake();

	float generateRandom();

private:
	NodeCamera* node;
	Vector3d relativePosition;
	EventManager* eventManager;
	float startYPosition;
	float shakeRate;

	float speed;

	Vector3d offset;
};

