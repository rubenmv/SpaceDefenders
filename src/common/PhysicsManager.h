#pragma once
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "Vector2d.h"
#include <vector>


struct AllButMeAabbResultCallback : public btBroadphaseAabbCallback {
   AllButMeAabbResultCallback(btCollisionObject* me) : me(me) {}

   virtual bool process(const btBroadphaseProxy* proxy) {
      btCollisionObject* collisionObject = static_cast<btCollisionObject*>(proxy->m_clientObject);
      if (collisionObject == this->me) {
         return true;
      }
      this->hits.push_back(collisionObject);
      return true;
   }

   btCollisionObject* me;
   btCollisionObjectArray hits;
};

class PhysicsManager
{
public:
	PhysicsManager(void);
	~PhysicsManager(void);

	void update();

	btDynamicsWorld* getWorld()
	{
		return world;
	}

	btCollisionShape* getGroundShape()
	{
		return groundShape;
	}

	void addBody(btRigidBody* body)
	{
		bodies.push_back(body);
	}

	void removeBody()
	{
		if(!bodies.empty())
		{
			world->removeRigidBody(bodies[0]);
			bodies.erase(bodies.begin());
		}
	}

	void removeBody(btRigidBody* body)
	{
		std::vector<btRigidBody*>::iterator it;
		for(it=bodies.begin();it!=bodies.end();++it)
		{
			if(*it == body)
			{
				bodies.erase(it);
				break;
			}
		}
	}

	void createExplosion(Vector2d position, float impulse);

private:
	btDynamicsWorld *world;
	btCollisionShape* groundShape;
	std::vector<btRigidBody*> bodies;
};

