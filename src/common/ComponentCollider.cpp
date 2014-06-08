#include "ComponentCollider.h"
#include <iostream>
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "GameObject.h"
#include "Message.h"


ComponentCollider::ComponentCollider(float collisionRadius)
{
	this->sqrCollisionRadius = collisionRadius * collisionRadius;
	playerIsDeath = false;
}


ComponentCollider::~ComponentCollider(void)
{
	GameManager::getInstance()->getCollisionManager()->removeFromList(this);
}

void ComponentCollider::awake()
{
	GameManager::getInstance()->getCollisionManager()->addComponentCollider(this);

	if(parent->isEnemyTeam())//Por ahora solo hay clipping en los enemigos
	{
		Message message;
		message.type = Message::OUT_OF_CAMERA;
		parent->broadcastMessage(message);
	}
}

void ComponentCollider::onCollision(Collision collision)
{
	if(!parent->isStatic())
	{
		parent->position -= collision.direction/10;
		
	}

	Message message;
	message.type = Message::COLLISION;
	message.target = collision.direction;
	message.gameObject = collision.collider;
	
	parent->broadcastMessage(message);
}

void ComponentCollider::update()
{	
	if(playerIsDeath)
	{
		playerIsDeath = false;
		GameManager::getInstance()->getCollisionManager()->removeFromList(this);
	}
}

void ComponentCollider::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::DEATH:
		handleDeath();
		break;
	case Message::REPAIRED:
		GameManager::getInstance()->getCollisionManager()->addComponentCollider(this);
		break;
	}

}

void ComponentCollider::handleDeath()
{
	if (parent->isPlayer())
	{
		playerIsDeath = true;	
	}
}
