#include "ComponentAIMine.h"
#include <iostream>
#include "GameManager.h"
#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "GameObject.h"
#include "NetworkEngine.h"
#include "Message.h"

ComponentAIMine::ComponentAIMine(float explosionTime)
{
	this->explosionTime = explosionTime;
	this->activated = false;
	this->clock = Clock();
	this->clock.pause();
}


ComponentAIMine::~ComponentAIMine(void)
{
}

void ComponentAIMine::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::NEW_VISION:
		countdown(message.gameObject);
		break;
	}
}

void ComponentAIMine::update()
{
	if(clock.getElapsedTime() >= explosionTime)
	{
		
		GameObject* explosion = GameManager::getInstance()->getGameObjectManager()->createExplosion(20);
		explosion->position = parent->position;
		parent->kill();

	}
}

void ComponentAIMine::countdown( GameObject* target )
{
	//Si son enemigos la mina se activa
	if(GameObject::areEnemies(target, parent))
	{
		clock.unpause();
	}
}

