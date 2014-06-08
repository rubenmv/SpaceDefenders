#pragma once
#include "GameManager.h"
#include "Component.h"
#include "Clock.h"

GameObject;
GameObjectManager;

class ComponentBulletLauncher :	public Component
{
public:
	ComponentBulletLauncher(float damage, float fireRate,void (ComponentBulletLauncher::*)());
	~ComponentBulletLauncher(void);
	
	virtual void update();
	virtual void onMessage(const Message& message);
	void (ComponentBulletLauncher::*superShoot)();
	void normalShoot();
	void multipleShoot();

private:
	void shoot();
	void stopShoot();
	Clock delay;
	bool shooting;
	float fireRate;
	GameObjectManager* gameObjectManager;
	GameObject* targetObject;
	float damage;
};

