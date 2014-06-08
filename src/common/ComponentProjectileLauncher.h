#pragma once
#include "Component.h"
#include "Clock.h"
class GameObjectManager;

class ComponentProjectileLauncher : public Component
{
public:
	ComponentProjectileLauncher(float fireRate, void(ComponentProjectileLauncher::*)());
	~ComponentProjectileLauncher(void);

	virtual void update();
	virtual void onMessage(const Message& message);
	void (ComponentProjectileLauncher::*superShoot)();
	void normalShoot();
	void multipleShoot();
	void normalBulletShoot();
	void multipleBulletShoot();
	void turretShoot1();
	void turretShoot2();
	void enemyBomberShoot();
	void targetShoot();
	void playerRayShoot();
	void turretBulletShoot();
	void battleshipBulletShoot();
	void bomberBulletShoot();
	void engineerBulletShoot();
	void fighterBulletShoot();

private:
	void shoot();
	void stopShoot();
	void sendMessageBulletShoot( GameObject* projectileOrBullet, GameObject* parent );
	Clock delay;
	int charge;
	bool shooting;
	float fireRate;
	GameObjectManager* gameObjectManager;
	GameObject* targetObject;
	float damage;
	bool left;
	GameObject* ray;
};

