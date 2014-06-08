#pragma once
#include "component.h"


class ComponentExplosionDamage :	public Component
{
public:
	ComponentExplosionDamage(float damage);
	virtual ~ComponentExplosionDamage(void);

	virtual void onMessage(const Message& message);

	void explode(GameObject* gameObject);

	virtual void update();

private:
	
	float damage;

};

