#include "ComponentBulletLauncher.h"
#include "GameObject.h"
#include "GameObjectManager.h"

void ComponentBulletLauncher::normalShoot()
{
	Message message;
	message.type = Message::NEW_TARGET;
	GameObject* projectileOrBullet = NULL;

	if(parent->isEnemyTeam())
	{
		projectileOrBullet = gameObjectManager->createEnemyBullet(damage);
	}else{
		projectileOrBullet = gameObjectManager->createBullet(damage);
	}
				
	projectileOrBullet->rotation = parent->rotation;
	projectileOrBullet->position = parent->position + Vector2d::getVector2dByAngle(-projectileOrBullet->rotation)*7;
	projectileOrBullet->velocity = parent->velocity;
	message.target = parent->position + Vector2d::getVector2dByAngle(-projectileOrBullet->rotation)*5000;
	message.gameObject = NULL;

	projectileOrBullet->broadcastMessage(message);
}

void ComponentBulletLauncher::multipleShoot()
{
	Message message;
	message.type = Message::NEW_TARGET;
	GameObject* projectileOrBullet = NULL;

	for(int i=0;i<3;i++)
	{
		if(parent->isEnemyTeam())
		{
			projectileOrBullet = gameObjectManager->createEnemyBullet(damage);
		}else{
			projectileOrBullet = gameObjectManager->createBullet(damage);
		}
				
		projectileOrBullet->rotation = parent->rotation-10+10*i;
		projectileOrBullet->position = parent->position + Vector2d::getVector2dByAngle(-projectileOrBullet->rotation)*7;
		projectileOrBullet->velocity = parent->velocity;
		message.target = parent->position + Vector2d::getVector2dByAngle(-projectileOrBullet->rotation)*5000;
		message.gameObject = NULL;

		projectileOrBullet->broadcastMessage(message);
	}
}

ComponentBulletLauncher::ComponentBulletLauncher(float damage, float fireRate,void (ComponentBulletLauncher::*fnc)())
{
	gameObjectManager = GameManager::getInstance()->getGameObjectManager();
	this->damage = damage;
	delay = Clock();
	shooting = false;
	targetObject = NULL;
	this->fireRate = fireRate;
	superShoot = fnc;
}


ComponentBulletLauncher::~ComponentBulletLauncher(void)
{
	gameObjectManager = NULL;
}

void ComponentBulletLauncher::update()
{
	shoot();
}

void ComponentBulletLauncher::shoot()
{

	if(targetObject != NULL && targetObject->isDead()) //Se supone que isBullet es false
	{
		targetObject = NULL;
		shooting = false;
		return;
	}

	if(delay.getElapsedTime() >= fireRate && shooting)
	{
		(this->*superShoot)();
		delay.restart();
	}
}

void ComponentBulletLauncher::stopShoot()
{
	if(shooting)
	{
		shooting = false;
	}
	targetObject = NULL;
}

void ComponentBulletLauncher::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::START_SHOOTING:
		shooting = true;
		targetObject = message.gameObject;
		break;
	case Message::DEATH:
	case Message::STOP_SHOOTING:
		stopShoot();
		break;

	}
}