
#include "ComponentRepairSystem.h"

#include "GameManager.h"
#include "GameObject.h"
#include "EventManager.h"


ComponentRepairSystem::ComponentRepairSystem(void)
{
	speed = 100;
	acceleration = 20;
	player = NULL;
}


ComponentRepairSystem::~ComponentRepairSystem(void)
{
}

void ComponentRepairSystem::update()
{
	if(player != NULL)
	{
		parent->position = player->position;
		float deltaTime = GameManager::getInstance()->getDeltaTime();
		speed += acceleration * deltaTime;
		parent->rotation += speed * deltaTime;

		if(speed <= 0 )///*Debug ->*/ || GameManager::getInstance()->getEventManager()->isKeyDown(KEY::KEY_KEY_R))
		{
			Message message;
			message.type = Message::REPAIRED;
			player->broadcastMessage(message);
			parent->kill();
		}
	}
	
}

void ComponentRepairSystem::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::NEW_TARGET:
		player = message.gameObject;
		break;
	case Message::NEW_VISION:
		onNewVision(message.gameObject);
		break;	
	case Message::LOST_VISION:
		onLastVision(message.gameObject);
		break;
	}
}

void ComponentRepairSystem::onNewVision(GameObject * gameObject)
{
	if(gameObject->isPlayer())
	{
		acceleration -=50;
	}
}

void ComponentRepairSystem::onLastVision( GameObject* gameObject )
{
	if(gameObject->isPlayer())
	{
		acceleration +=50;
	}
}
