#include "ComponentTargetAim.h"
#include "GameObject.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include <sstream>

ComponentTargetAim::ComponentTargetAim(void)
{
	gameObjectTarget = NULL;
	shooting = false;
}

ComponentTargetAim::~ComponentTargetAim(void)
{
	gameObjectTarget = NULL;
}

void ComponentTargetAim::update()
{
	if(gameObjectTarget != NULL)
	{
		if(gameObjectTarget->isDead())
		{
			gameObjectTarget = NULL;
			shooting = false;
			return;
		}
			// Debug
	
/*	GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	if(graphicsEngine != NULL)
	{
		if(GameManager::getInstance()->getDebugTools()->isShowingDebug())
		{				
			graphicsEngine->drawDebugLine(parent->position.asVector3d(), gameObjectTarget->position.asVector3d());
		}
	}
	*/
	// !Debug


		float desiredRotation = (gameObjectTarget->position+gameObjectTarget->velocity*0.2 - parent->position).getAngle();

		float newRotation = parent->rotation - desiredRotation;

		newRotation = Math::warpAngle(newRotation);

		if(Math::abs(newRotation) < 0.001)
		{
			parent->rotation = desiredRotation;
		} else
		{
			parent->rotation -= newRotation * GameManager::getInstance()->getDeltaTime() * 10;
		}

		parent->rotation = Math::warpAngle(parent->rotation);		
	
		if(Math::abs(newRotation) < 10)
		{
			if(!shooting)
			{
				Message message;
				message.type = Message::START_SHOOTING;
				message.gameObject = gameObjectTarget;
				parent->broadcastMessage(message);
				shooting=true;

			}
		}
		else
		{
			if(shooting)
			{
				shooting = false;
				Message message;
				message.type = Message::STOP_SHOOTING;
				parent->broadcastMessage(message);

			}
		}
	}
	else
	{
		
		shooting = false;
	}
}

void ComponentTargetAim::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::NEW_AIM:
		onNewAim(message.gameObject);
		break;
	case Message::STOP_AIM:
		stopAim();
		break;
	}
}

void ComponentTargetAim::onNewAim( GameObject* newTarget )
{
	gameObjectTarget = newTarget;

	shooting = false;
}

void ComponentTargetAim::stopAim()
{
	Message message;
	message.type = Message::STOP_SHOOTING;
	parent->broadcastMessage(message);
	gameObjectTarget = NULL;
	shooting = false;
}