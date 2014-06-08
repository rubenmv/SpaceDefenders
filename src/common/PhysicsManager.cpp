#include "PhysicsManager.h"
#include "GameManager.h"


PhysicsManager::PhysicsManager(void)
{
	btCollisionConfiguration *collisionConfiguration = new btDefaultCollisionConfiguration();
	btBroadphaseInterface *broadPhase = new btDbvtBroadphase(new btHashedOverlappingPairCache());

	btDispatcher *collisionDispatcher = new btCollisionDispatcher(collisionConfiguration);
	btConstraintSolver *constraintSolver = new btSequentialImpulseConstraintSolver();

	world = new btDiscreteDynamicsWorld(collisionDispatcher, broadPhase, constraintSolver, collisionConfiguration);
	world->setGravity(btVector3(0,-20,0));

	groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);

	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-6,0)));
	btRigidBody::btRigidBodyConstructionInfo    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,-10,0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	world->addRigidBody(groundRigidBody);
}


PhysicsManager::~PhysicsManager(void)
{
}

void PhysicsManager::update()
{
	world->stepSimulation(GameManager::getInstance()->getDeltaTime(),10);
}

void PhysicsManager::createExplosion(Vector2d position,float impulse)
{
	float radius = 30;

	btVector3 center(position.x,-5,position.y);
	btVector3 aabbMin = center - radius*btVector3(1.0f, 1.0f, 1.0f);
	btVector3 aabbMax = center + radius*btVector3(1.0f, 1.0f, 1.0f);
	
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(position.x,-5,position.y));

	btDefaultMotionState *motionState = new btDefaultMotionState(transform);
	//Mirar de cambiar la malla por otra cosa
	btCollisionShape* colShape = new btSphereShape(radius);
	btScalar mass = 1;
	btVector3 localInertia;
	colShape->calculateLocalInertia(mass,localInertia);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
	
	btRigidBody* body = new btRigidBody(rbInfo);

	AllButMeAabbResultCallback callback(body);

	world->getBroadphase()->aabbTest(aabbMin,aabbMax,callback);



	for(int i=0;i<callback.hits.size();i++)
	{
		btVector3 pulseDirection(callback.hits[i]->getWorldTransform().getOrigin() - center);


		float rate = 1 - pulseDirection.getY()/5;

		pulseDirection.setY(0);

		float distance = pulseDirection.norm();

		distance = Math::min_(distance, radius-5);
		pulseDirection.setY(0.1);

		btVector3 objectImpulse((2 + rate) * Math::max_(1.0f - distance/radius, 0)* pulseDirection); //linear    www

		btRigidBody::upcast(callback.hits[i])->applyCentralImpulse(objectImpulse*impulse);
	}

}