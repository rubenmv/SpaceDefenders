#include "ComponentDebris.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "NodeMesh.h"
#include "GameObjectManager.h"
#include "PhysicsManager.h"

ComponentDebris::ComponentDebris(const char* meshFileName,Vector2d velocity, Vector2d position)
{
	btTransform transform;
	transform.setIdentity();
	transform.setRotation(btQuaternion(velocity.x,velocity.y,0,1));
	transform.setOrigin(btVector3(position.x+velocity.x,0,position.y+velocity.y));

	btDefaultMotionState *motionState = new btDefaultMotionState(transform);
	//Mirar de cambiar la malla por otra cosa
	btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
	//btCollisionShape* colShape = new btSphereShape(2);
	btScalar mass = 1;
	btVector3 localInertia;
	colShape->calculateLocalInertia(mass,localInertia);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
	
	body = new btRigidBody(rbInfo);

	//body->applyCentralImpulse(btVector3(velocity.x*5,3,velocity.y*5));
	body->applyTorqueImpulse(btVector3(velocity.x,0,velocity.y));
	body->setFriction(3);
	nodeMesh = GameManager::getInstance()->getGraphicsEngine()->addNodeMesh(meshFileName);

	btDynamicsWorld* world = GameManager::getInstance()->getPhysicsManager()->getWorld();
	world->addRigidBody(body);

	GameManager::getInstance()->getPhysicsManager()->addBody(body);
	if(world->getNumCollisionObjects() > 100)
	{
		GameManager::getInstance()->getPhysicsManager()->removeBody();
	}
	clock = Clock();
}

ComponentDebris::~ComponentDebris(void)
{
	if(body != NULL)
	{
		GameManager::getInstance()->getPhysicsManager()->getWorld()->removeRigidBody(body);
	}
	
	delete body;
	body = NULL;

	delete nodeMesh;
	nodeMesh = NULL;
}

void ComponentDebris::update()
{
	
	// Actualización de la posición
	if(nodeMesh != NULL && body->isInWorld())
	{
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);
		btVector3 pos = body->getCenterOfMassPosition();

		//Borra el debris si esta fuera del mapa o en una pared
		deleteOnWall(Vector2d(pos[0], pos[2]));

		nodeMesh->setPosition(Vector3d((float)pos[0], (float)pos[1], (float)pos[2]));
		// Actualización de la orientación
		const btQuaternion &quat = trans.getRotation();
		Vector3d rotation = quaternionToVector3d(quat.getX(), quat.getY(), quat.getZ(), quat.getW());
		rotation *= Math::RADTODEG;
		nodeMesh->setRotation(rotation);

		if(clock.getElapsedTime() > 10000 && nodeMesh != NULL)
		{
			removeDebris();

		}
		else if(clock.getElapsedTime() > 1000 && nodeMesh == NULL)
		{
			removeDebris();
		}
	}
	else
	{
		parent->kill();
	}
}

void ComponentDebris::onMessage(const Message& message)
{

}

Vector3d ComponentDebris::quaternionToVector3d(float X,float Y, float Z,float W)
{
	const float sqw = W*W;
	const float sqx = X*X;
	const float sqy = Y*Y;
	const float sqz = Z*Z;
	const float test = 2.0 * (Y*W - X*Z);

	Vector3d euler;
	
	if((test + 0.000001 >= 1.0) && (test - 0.000001 <= 1.0))
	{
		// heading = rotation about z-axis
		euler.z = (float) (-2.0*atan2(X, W));
		// bank = rotation about x-axis
		euler.x = 0;
		// attitude = rotation about y-axis
		euler.y = (float) (Math::PI/2.0);
	}
	else if((test + 0.000001 >= -1.0) && (test - 0.000001 <= -1.0))
	{
		// heading = rotation about z-axis
		euler.z = (float) (2.0*atan2(X, W));
		// bank = rotation about x-axis
		euler.x = 0;
		// attitude = rotation about y-axis
		euler.y = (float) (Math::PI/-2.0);
	}
	else
	{
		// heading = rotation about z-axis
		euler.z = (float) atan2(2.0 * (X*Y +Z*W),(sqx - sqy - sqz + sqw));
		// bank = rotation about x-axis
		euler.x = (float) atan2(2.0 * (Y*Z +X*W),(-sqx - sqy + sqz + sqw));
		// attitude = rotation about y-axis
		euler.y = (float) asin( Math::min_(Math::max_(test,-1), 1));
	}

	return euler;
}

void ComponentDebris::deleteOnWall( Vector2d position )
{

	if(!GameManager::getInstance()->getMapManager()->isEmpty(position))
	{
		removeDebris();
	}
}

void ComponentDebris::removeDebris()
{
	GameManager::getInstance()->getPhysicsManager()->removeBody(body);
	GameManager::getInstance()->getPhysicsManager()->getWorld()->removeRigidBody(body);
	parent->kill();
}
