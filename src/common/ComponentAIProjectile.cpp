#include "ComponentAIProjectile.h"
#include "GameObject.h"
#include "GameManager.h"
#include "GameObjectManager.h"
#include "MapManager.h"

ComponentAIProjectile::ComponentAIProjectile(float damage)
{
	this->damage = damage;
	targetLock = false;
}


ComponentAIProjectile::~ComponentAIProjectile(void)
{

}

void ComponentAIProjectile::awake()
{

	/*if(!GameManager::getInstance()->isServer())
	{
		GameObject * muzzle = GameManager::getInstance()->getGameObjectManager()->createMuzzleFlash();
		muzzle->position = parent->position;
	}*/

	if (!targetLock)
	{
		Message message;
		message.type = Message::NEW_TARGET;
		message.target = parent->position + Vector2d::getVector2dByAngle(-parent->rotation)*5000;

		parent->broadcastMessage(message);
	}

	parent->rotation += Math::random(-100,100)/100.0;
}

void ComponentAIProjectile::update()
{
	/*if(!GameManager::getInstance()->getMapManager()->isEmpty(parent->position))
	{

		parent->kill();

	}*/
	Vector2d direction = parent->velocity;
	direction.normalize();
	parent->position += direction * (Math::random(-50,50)/100.0);
}

void ComponentAIProjectile::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::NEW_TARGET:
		targetLock = true;
		break;
	//case Message::START_SHOOTING:
	//	onStartShooting(gameObject, message.gameObject);
	//	break;
	case Message::NEW_VISION:
		explode(message.gameObject);
		break;
	case Message::TARGET_REACHED:
		sendNewTarget();
		break;
	}
}

void ComponentAIProjectile::explode(GameObject* targetObject )
{
	//Solo explota si son enemigos
	if(GameObject::areEnemies(targetObject, parent))
	{
		// El objeto target es el que envia el mensaje con el daño para
		// que sus componentes health, etc. lo reciban
		Message message;
		message.type = Message::INCOMING_DAMAGE;
		if(damage != -1)
		{
			message.value = damage;
		}
		else
		{
			message.value = 200;
		}
		message.target = parent->position;
		targetObject->broadcastMessage(message);

		//targetObject->acceleration += parent->velocity/10;
		if(damage != -1)
		{
			parent->kill(); // El proyectil se elimina
		}
	}
}

void ComponentAIProjectile::sendNewTarget()
{
	Message message;

	message.type = Message::NEW_TARGET;
	message.target = Vector2d();
	message.target = parent->position + (Vector2d::getVector2dByAngle(-parent->rotation)*1000);
	parent->broadcastMessage(message);
}
