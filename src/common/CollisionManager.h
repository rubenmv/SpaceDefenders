#pragma once

#include "Vector2d.h"
#include <vector>
#include "ID.h"
#include "Clock.h"

class ComponentCollider;
class ComponentVision;
class GameObject;

struct Collision
{
	GameObject* collider;
	Vector2d direction;
};


/*
	Registra colliders y gestiona las acciones cuando se
	producen colisiones dependiendo del objeto
*/
class CollisionManager
{
public:
	CollisionManager(void);
	~CollisionManager(void);

	void addComponentCollider(ComponentCollider* collider);
	void addComponentVision(ComponentVision* vision);
	bool canSeeTarget( Vector2d start, Vector2d end,int id1, int id2 );
	std::vector<GameObject*> getGameObjectBetween(Vector2d upright, Vector2d upleft, Vector2d downright, Vector2d downleft);
	void removeFromList(ComponentCollider* collider);
	void removeFromList(ComponentVision* vision);
	//Devuelve el primer GameObject que encuentre debajo de position;
	GameObject* getGameObjectIn(Vector2d position);
	void update();

	unsigned int getUpdateTime();

private:
	std::vector<ComponentCollider*> colliderEnemyList;
	std::vector<ComponentCollider*> colliderAlliesList;
	std::vector<ComponentVision*> visionList;
	void checkVisionBetween (ComponentVision* visionCollider, ComponentCollider* targetCollider);
	void checkCollisionBetween (ComponentCollider* currentCollider, ComponentCollider* targetCollider);

	Clock updateClockCM;
	int updateTime;

};

