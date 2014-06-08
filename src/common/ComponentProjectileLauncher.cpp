#include "ComponentProjectileLauncher.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "GameManager.h"
#include "NetworkEngine.h"
#include "GraphicsEngine.h"
#include "CollisionManager.h"

ComponentProjectileLauncher::ComponentProjectileLauncher(float fireRate, void (ComponentProjectileLauncher::*fnc)())
{
	gameObjectManager = GameManager::getInstance()->getGameObjectManager();
	this->damage = damage;
	delay = Clock();
	charge = 0;
	shooting = false;
	targetObject = NULL;
	this->fireRate = fireRate;
	this->superShoot = fnc;
	left=false;
	ray=NULL;
}


ComponentProjectileLauncher::~ComponentProjectileLauncher(void)
{
	gameObjectManager = NULL;
}

void ComponentProjectileLauncher::update()
{
	shoot();
}

void ComponentProjectileLauncher::shoot()
{

	if(targetObject != NULL && targetObject->isDead()) //Se supone que isBullet es false
	{
		targetObject = NULL;
		shooting = false;
		return;
	}

	if(delay.getElapsedTime() >= fireRate && shooting)
	{
		if(!GameManager::getInstance()->isServer())
		{
			GameObject * muzzle = GameManager::getInstance()->getGameObjectManager()->createMuzzleFlash();
			muzzle->position = parent->position + Vector2d::getVector2dByAngle(-parent->rotation)*7 + parent->velocity*GameManager::getInstance()->getDeltaTime();
		}
		(this->*superShoot)();
		delay.restart();
	}
}

void ComponentProjectileLauncher::stopShoot()
{
	if(ray != NULL && !ray->isDead())
	{
		ray->kill();
		ray=NULL;
	}

	if(shooting)
	{
		shooting = false;
		charge = 0;
	}
	targetObject = NULL;
}

void ComponentProjectileLauncher::onMessage(const Message& message)
{
	Message message2;
	switch (message.type)
	{
	case Message::START_SHOOTING:
		shooting = true;
		targetObject = message.gameObject;
		break;
	case Message::STOP_SHOOTING:
		stopShoot();
		break;
	/*case Message::TYPE_COUNT:
		if(superShoot != &ComponentProjectileLauncher::multipleBulletShoot)
		{
			superShoot = &ComponentProjectileLauncher::multipleBulletShoot;
		}
		else
		{
			superShoot = &ComponentProjectileLauncher::normalBulletShoot;
		}
		break;*/
	case Message::TARGET_SHOT:
		if(superShoot != &ComponentProjectileLauncher::targetShoot)
		{
			superShoot = &ComponentProjectileLauncher::targetShoot;
		}
		else
		{
			superShoot = &ComponentProjectileLauncher::normalBulletShoot;
		}
		break;
	case Message::RAY_SHOT:
		if(superShoot != &ComponentProjectileLauncher::playerRayShoot)
		{
			superShoot = &ComponentProjectileLauncher::playerRayShoot;
		}
		else
		{
			superShoot = &ComponentProjectileLauncher::normalBulletShoot;
		}
		break;
	case Message::LEVEL_CHANGED:
		damage += damage*message.target.y*message.value;
		break;
	}
}

void ComponentProjectileLauncher::normalShoot()
{
	Message message;
	message.type = Message::NEW_TARGET;
	GameObject* projectileOrBullet = NULL;

	if(parent->isEnemyTeam())
	{
		projectileOrBullet = gameObjectManager->createEnemyProjectile(damage);
	}else{
		projectileOrBullet = gameObjectManager->createProjectile(damage);
	}

	message.gameObject = targetObject; //Si es una bala targetObject valdra NULL
	projectileOrBullet->rotation = parent->rotation;
	projectileOrBullet->position = parent->position + Vector2d::getVector2dByAngle(-parent->rotation)*7;
	projectileOrBullet->velocity = parent->velocity;

	projectileOrBullet->broadcastMessage(message);
}

void ComponentProjectileLauncher::multipleShoot()
{
	Message message;
	message.type = Message::NEW_TARGET;
	GameObject* projectileOrBullet = NULL;

	for(int i=0;i<3;i++)
	{
		if(parent->isEnemyTeam())
		{
			projectileOrBullet = gameObjectManager->createEnemyProjectile(damage);
		}else{
			projectileOrBullet = gameObjectManager->createProjectile(damage);
		}
			
		message.gameObject = targetObject; //Si es una bala targetObject valdra NULL
		projectileOrBullet->rotation = parent->rotation-10+10*i;
		projectileOrBullet->position = parent->position + Vector2d::getVector2dByAngle(-parent->rotation)*7;
		projectileOrBullet->velocity = parent->velocity;

		projectileOrBullet->broadcastMessage(message);
	}
}

void ComponentProjectileLauncher::normalBulletShoot()
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
	projectileOrBullet->position = parent->position + Vector2d::getVector2dByAngle(-projectileOrBullet->rotation)*6;
	projectileOrBullet->velocity = parent->velocity;
	message.target = parent->position + Vector2d::getVector2dByAngle(-projectileOrBullet->rotation)*5000;
	message.gameObject = NULL;

	projectileOrBullet->broadcastMessage(message);

}

	

void ComponentProjectileLauncher::battleshipBulletShoot()
{

	GameObject* projectileOrBullet = NULL;
	projectileOrBullet = gameObjectManager->createBulletBattleship();
	sendMessageBulletShoot(projectileOrBullet, parent);
}

void ComponentProjectileLauncher::bomberBulletShoot()
{
	
	GameObject* projectileOrBullet = NULL;
	projectileOrBullet = gameObjectManager->createBulletBomber();	
	sendMessageBulletShoot(projectileOrBullet, parent);
}


void ComponentProjectileLauncher::engineerBulletShoot()
{

	GameObject* projectileOrBullet = NULL;
	projectileOrBullet = gameObjectManager->createBulletEngineer();
	sendMessageBulletShoot(projectileOrBullet, parent);
}
	

void ComponentProjectileLauncher::fighterBulletShoot()
{

	GameObject* projectileOrBullet = NULL;
	projectileOrBullet = gameObjectManager->createBulletFighter();
	sendMessageBulletShoot(projectileOrBullet, parent);
}




void ComponentProjectileLauncher::multipleBulletShoot()
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
		projectileOrBullet->position = parent->position + Vector2d::getVector2dByAngle(-projectileOrBullet->rotation)*6;
		projectileOrBullet->velocity = parent->velocity;
		message.target = parent->position + Vector2d::getVector2dByAngle(-projectileOrBullet->rotation)*5000;
		message.gameObject = NULL;

		projectileOrBullet->broadcastMessage(message);
	}
}

void ComponentProjectileLauncher::turretShoot1()
{
	Message message;
	message.type = Message::NEW_TARGET;
	GameObject* projectileOrBullet = NULL;

	if(parent->isEnemyTeam())
	{
		projectileOrBullet = gameObjectManager->createEnemyProjectile(damage);
	}else{
		projectileOrBullet = gameObjectManager->createProjectile(damage*6);
	}
			
	message.gameObject = targetObject; //Si es una bala targetObject valdra NULL
	projectileOrBullet->rotation = parent->rotation;
	projectileOrBullet->position = parent->position + Vector2d::getVector2dByAngle(-parent->rotation)*7;
	if(left)
	{
		projectileOrBullet->position = projectileOrBullet->position.rotateBy(-10,parent->position);
		left = !left;
	}
	else
	{
		projectileOrBullet->position = projectileOrBullet->position.rotateBy(10,parent->position);
		left = !left;
	}
	projectileOrBullet->velocity = parent->velocity;

	projectileOrBullet->broadcastMessage(message);
}

void ComponentProjectileLauncher::turretShoot2()
{
	Message message;
	message.type = Message::NEW_TARGET;
	GameObject* projectileOrBullet = NULL;
	
	for(int i=0;i<2;i++)
	{
		if(parent->isEnemyTeam())
		{
			projectileOrBullet = gameObjectManager->createEnemyProjectile(damage);
		}else{
			projectileOrBullet = gameObjectManager->createProjectile(damage);
		}
			
		message.gameObject = targetObject;
		projectileOrBullet->rotation = parent->rotation;
		projectileOrBullet->position = parent->position + Vector2d::getVector2dByAngle(-parent->rotation)*7;
		projectileOrBullet->position = projectileOrBullet->position.rotateBy(-10 + 20*i,parent->position);
		projectileOrBullet->velocity = parent->velocity;

		projectileOrBullet->broadcastMessage(message);
	}
}

void ComponentProjectileLauncher::enemyBomberShoot()
{
	//std::cout<<charge<<std::endl;
	if(charge > 10)
	{
		GameObject* projectileOrBullet = NULL;

		ray = gameObjectManager->createRay(parent);
				
		ray->rotation = parent->rotation;
		ray->position = parent->position + Vector2d::getVector2dByAngle(-ray->rotation)*15;

		shooting = false;
		charge = 0;
	}
	else
	{
		(GameManager::getInstance()->getGameObjectManager()->createSpark())->position = parent->position + Vector2d(Math::random(-3,3),Math::random(-3,3));
	}
	charge++;
}

void ComponentProjectileLauncher::playerRayShoot()
{
	//std::cout<<charge<<std::endl;
	if(GameManager::getInstance()->getEventManager()->mouseState.leftButtonPressed)
	{
		if(charge > 15)
		{
			GameObject* projectileOrBullet = NULL;

			ray = gameObjectManager->createRay(parent);
				
			ray->rotation = parent->rotation;
			ray->position = parent->position + Vector2d::getVector2dByAngle(-ray->rotation)*15;

			shooting = false;
			charge = 0;
		}
		else
		{
			(GameManager::getInstance()->getGameObjectManager()->createSpark())->position = parent->position + Vector2d(Math::random(-3,3),Math::random(-3,3));
		}
		charge++;
	}
	else
	{
		charge = 0;
	}
}

void ComponentProjectileLauncher::targetShoot()
{
	if(GameManager::getInstance()->getEventManager()->mouseState.leftButtonDown)
	{
		Message message;
		message.type = Message::NEW_TARGET;
		message.gameObject = GameManager::getInstance()->getCollisionManager()->getGameObjectIn(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
		GameObject* projectileOrBullet = NULL;

		projectileOrBullet = gameObjectManager->createTargetProjectile(damage);
	
		if(message.gameObject == NULL)
		{
			message.target = GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround();
		}
		projectileOrBullet->rotation = parent->rotation;
		projectileOrBullet->position = parent->position + Vector2d::getVector2dByAngle(-parent->rotation)*7;
		projectileOrBullet->velocity = parent->velocity;

		projectileOrBullet->broadcastMessage(message);

		superShoot = &ComponentProjectileLauncher::normalBulletShoot;
	}
}

void ComponentProjectileLauncher::turretBulletShoot()
{
	Message message;
	message.type = Message::NEW_TARGET;
	GameObject* projectileOrBullet1 = NULL;
	GameObject* projectileOrBullet2 = NULL;
	
	projectileOrBullet1 = gameObjectManager->createBullet(damage);
	projectileOrBullet2 = gameObjectManager->createBullet(damage);

	projectileOrBullet1->rotation = parent->rotation;
	projectileOrBullet1->position = parent->position + Vector2d::getVector2dByAngle(-parent->rotation)*7;
	projectileOrBullet2->rotation = parent->rotation;
	projectileOrBullet2->position = parent->position + Vector2d::getVector2dByAngle(-parent->rotation)*7;
	if(left)
	{
		projectileOrBullet1->position = projectileOrBullet1->position.rotateBy(-12,parent->position);
		projectileOrBullet2->position = projectileOrBullet2->position.rotateBy(-8,parent->position);
		left = !left;
	}
	else
	{
		projectileOrBullet1->position = projectileOrBullet1->position.rotateBy(12,parent->position);
		projectileOrBullet2->position = projectileOrBullet2->position.rotateBy(8,parent->position);
		left = !left;
	}
	projectileOrBullet1->velocity = parent->velocity;
	projectileOrBullet2->velocity = parent->velocity;
	message.target = parent->position + Vector2d::getVector2dByAngle(-projectileOrBullet1->rotation)*5000;
	projectileOrBullet1->broadcastMessage(message);
	message.target = parent->position + Vector2d::getVector2dByAngle(-projectileOrBullet2->rotation)*5000;
	projectileOrBullet2->broadcastMessage(message);
}

void ComponentProjectileLauncher::sendMessageBulletShoot( GameObject* projectileOrBullet, GameObject* parent )
{
	Message message;
	message.type = Message::NEW_TARGET;

	projectileOrBullet->rotation = parent->rotation;
	projectileOrBullet->position = parent->position + Vector2d::getVector2dByAngle(-projectileOrBullet->rotation)*6 + parent->velocity*GameManager::getInstance()->getDeltaTime();
	projectileOrBullet->velocity = parent->velocity;
	message.target = parent->position + Vector2d::getVector2dByAngle(-projectileOrBullet->rotation)*5000;
	message.gameObject = NULL;

	projectileOrBullet->broadcastMessage(message);

}
