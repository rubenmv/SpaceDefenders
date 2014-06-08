#pragma once
#include "component.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "clock.h"
class NodeMesh;

class ComponentDebris :	public Component
{
public:
	ComponentDebris(const char* meshFileName,Vector2d velocity, Vector2d position);
	~ComponentDebris(void);

	void update();
	void onMessage(const Message& message);
	void explosion(Vector2d position);
private:
	Vector3d quaternionToVector3d(float X,float Y, float Z,float W);
	void removeDebris();
	void deleteOnWall(Vector2d position);
	btRigidBody *body;
	NodeMesh* nodeMesh;

	Clock clock;
};

