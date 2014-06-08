#pragma once
#include "Component.h"
#include "NodeAS.h"
#include "Vector2d.h"
#include <vector>

class GameManager;
class GraphicsEngine;

class ComponentSteering : public Component
{
public:
	ComponentSteering(bool leader);
	~ComponentSteering(void);
	void update();
	void awake(){};
	void onMessage(const Message& message);
		
	void setTarget(GameObject* newTarget)
	{
		targetObject = newTarget;
	};

private:
	Vector2d target;
	float speed;
	float maxSpeed;
	float mass;
	float max_force;
	bool isReached;
	bool leader;
	GameManager* gameManager;

	GameObject* targetObject;
	void changeTarget(Vector2d newTarget);
	void addForce(Vector2d steering);
	void move(Vector2d steering);
	Vector2d seek(Vector2d target);
	Vector2d flee(Vector2d posFlee);
	Vector2d arrive(Vector2d target);
	Vector2d evade(GameObject* mainPlayer);
	Vector2d followLeader(GameObject* leader);
};

