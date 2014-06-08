#include "ComponentSteering.h"
#include <algorithm>

#include "ComponentTargetPath.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GameObject.h"
#include "GraphicsEngine.h"
#include "Math.h"
#include "Clock.h"

ComponentSteering::ComponentSteering(bool leader)
{
	this->speed = 1;
	isReached = false; // Comienza parado
	target = Vector2d(0, 0);

	maxSpeed = 2;
	mass = 2;
	max_force = 3;
	gameManager = GameManager::getInstance();
	targetObject = NULL;
	this->leader = leader;
}


ComponentSteering::~ComponentSteering(void)
{
}

Vector2d ComponentSteering::seek(Vector2d target)
{
	//Calculos necesarios del Seek
	Vector2d desired_velocity = target - parent->position;
	desired_velocity.normalize();
	desired_velocity *= speed;
	Vector2d steering = desired_velocity - parent->velocity;
	gameManager->getGraphicsEngine()->drawDebugLine(parent->position.asVector3d(),(parent->position + desired_velocity*10).asVector3d(),1);
	return steering;
}

Vector2d ComponentSteering::flee(Vector2d posFlee)
{
	//Calculos necesarios del Seek
	Vector2d desired_velocity = posFlee - target;
	desired_velocity.normalize();
	desired_velocity *= speed;
	Vector2d steering = desired_velocity - parent->velocity;
	gameManager->getGraphicsEngine()->drawDebugLine(parent->position.asVector3d(),(parent->position + desired_velocity*10).asVector3d(),1);
	return steering;
}

Vector2d ComponentSteering::arrive(Vector2d target)
{
	Vector2d desired_velocity = target - parent->position;
	float distance = desired_velocity.getLength();

	// Check the distance to detect whether the character
	// is inside the slowing area
	if (distance < 20.0) 
	{
		// Inside the slowing area
		desired_velocity.normalize();
		desired_velocity *= (distance / 20.0) * speed;
	} 
	else 
	{
		// Outside the slowing area.
		desired_velocity.normalize();
		desired_velocity *= speed;
	}
 
	// Set the steering based on this
	Vector2d steering = desired_velocity - parent->velocity;
	gameManager->getGraphicsEngine()->drawDebugLine(parent->position.asVector3d(),(parent->position + desired_velocity*100).asVector3d(),1);
	return steering;
}

void ComponentSteering::move(Vector2d steering)
{
	//steering.truncate(max_force);
	//steering /= mass;
	parent->velocity += steering * 0.1;
	//parent->velocity.truncate(maxSpeed);
	//Debug Lines
	gameManager->getGraphicsEngine()->drawDebugLine(parent->position.asVector3d(),(parent->position + parent->velocity*20).asVector3d(),2);
	gameManager->getGraphicsEngine()->drawDebugLine(Vector2d(100,100).asVector3d(),Vector2d(-100,100).asVector3d(),2);
	gameManager->getGraphicsEngine()->drawDebugLine(Vector2d(-100,100).asVector3d(),Vector2d(-100,-100).asVector3d(),2);
	gameManager->getGraphicsEngine()->drawDebugLine(Vector2d(-100,-100).asVector3d(),Vector2d(100,-100).asVector3d(),2);
	gameManager->getGraphicsEngine()->drawDebugLine(Vector2d(100,-100).asVector3d(),Vector2d(100,100).asVector3d(),2);
	//gameManager->getGraphicsEngine()->drawDebugLine(parent->position.asVector3d(),(parent->position + steering*150).asVector3d(),3);
	
	//Modificaciones en la nave
	parent->rotation = parent->velocity.getAngle(); 
	parent->position += parent->velocity * speed;
	//parent->velocity.normalize();
}

Vector2d ComponentSteering::evade(GameObject* mainPlayer) //no funciona
{
  Vector2d distance = mainPlayer->position - parent->position;
  Vector2d futurePosition = mainPlayer->position + mainPlayer->velocity * speed;
  return flee(mainPlayer->position);
}

Vector2d ComponentSteering::followLeader(GameObject* leader)
{
	Vector2d tv = leader->velocity;
	Vector2d force;
 
	// Calculate the behind point
	tv *= -1;
	tv.normalize();
	tv *= 10;

	Vector2d behindLeader = leader->position + tv;
 
	// Creates a force to arrive at the behind point
	force = force + arrive(behindLeader);

 
	return force;
}

void ComponentSteering::update()
{
	if(!isReached)
	{
			if(targetObject != NULL)
			{
				if(targetObject->isDead())
				{
					targetObject = NULL;
					target = target - parent->position;
					target.normalize();
					target *= 10000; //Deberian seguir en linea recta 
										
				} else
				{
					target = targetObject->position;
				}
			}
			Vector2d movement;
			if(leader)
			{
				/*for(unsigned int i=0;i<mainPlayers.size();i++)
				{
					movement += evade(mainPlayers[i]);
				}*/
				movement += arrive(this->target);
				move(movement);

				Vector2d distanceVector = this->target - parent->position;
				// Vector2d te permite calcular las distancias
				float distance = distanceVector.getLength();

				// Entra en el rango del objetivo
				if(distance < 5)
				{
					isReached = true;
					Message message;
					message.type = Message::TARGET_REACHED;
					parent->broadcastMessage(message);
				}
			}
	}
}

void ComponentSteering::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::NEW_TARGET:
		changeTarget(message.target);
		break;
	case Message::NEW_FORCE:
		addForce(message.target);
		break;
	}
}

void ComponentSteering::changeTarget(Vector2d newTarget)
{
	target = newTarget;
	isReached = false;
}

void ComponentSteering::addForce(Vector2d steering)
{
	move(steering);
}