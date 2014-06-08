#pragma once
#include "Component.h"

//Se refiere a armas de tipo metralletas, minas tendran ComponentExplosionDamage en vez de este
class ComponentAIProjectile : public Component
{
public:
	ComponentAIProjectile(float damage);
	virtual ~ComponentAIProjectile(void);
	void awake();

	virtual void update();
	virtual void onMessage(const Message& message);

private:
	float damage;
	//Se utiliza para saber si tiene un objetivo al que ir
	bool targetLock;
	void explode(GameObject* targetObject);
	void sendNewTarget();
};

