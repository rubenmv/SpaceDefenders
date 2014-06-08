#pragma once
#include "Component.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include <vector>

class GameManager;


/*
	Compomenente que envia el objeto con el que colisiona
	y el vector direccion hacia este
*/
class ComponentCollider : public Component
{
public:
	ComponentCollider(float collisionRadius);
	~ComponentCollider(void);

	void onMessage(const Message& message);

	void update();
	void onCollision(Collision collision);
	float getSqrCollisionRadius(){return sqrCollisionRadius;};
	GameObject* getGameObject(){return parent;};
	void handleDeath();
	void awake();
private:
	float sqrCollisionRadius;
	bool playerIsDeath;
};

