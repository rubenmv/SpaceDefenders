#include <algorithm>

#include "ComponentTargetPath.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GameObject.h"
#include "GraphicsEngine.h"
#include "Math.h"
#include "Clock.h"


ComponentTargetPath::ComponentTargetPath(float speed,float targetRadius)
{
	isReached = false;
	this->target = Vector2d(0, 0);
	this->speed = speed;
	this->targetRadius = targetRadius;

	gameManager = GameManager::getInstance();
	targetObject = NULL;
}


ComponentTargetPath::~ComponentTargetPath(void)
{
	gameManager = NULL;
}

void ComponentTargetPath::awake()
{
}


void ComponentTargetPath::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::NEW_TARGET:
		if(message.gameObject == NULL)
		{
			changeTarget(message.target);
		}else
		{
			setTarget(message.gameObject);
		}
		break;
	}
}


void ComponentTargetPath::changeTarget(Vector2d newTarget)
{
	targetObject = NULL;
	target = newTarget;
	isReached = false;
}

void ComponentTargetPath::update()
{
	if(!isReached)
	{
		if(targetObject != NULL)
		{
			if(targetObject->isDead())
			{
				targetObject = NULL;

				targetReached();

			} else
			{
				// La posicion del target va cambiando segun se mueve el objeto de destino
				target = targetObject->position;// + targetObject->velocity*500;
			}
		}
			
		Vector2d distanceVector = target - parent->position;
		
		if(targetRadius != 0 && distanceVector.getSqrLength() < targetRadius*targetRadius)
		{
			targetReached();
		} else
		{
			distanceVector.normalize();
			parent->acceleration += distanceVector * speed;

		}		
	}
}

void ComponentTargetPath::setTarget( GameObject* newTarget )
{
	isReached = false;
	targetObject = newTarget;
	target = targetObject->position;
}

void ComponentTargetPath::targetReached()
{
	isReached = true;
	Message message;
	message.type = Message::TARGET_REACHED;
	parent->broadcastMessage(message);
}
