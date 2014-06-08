#include "ComponentExplosionDamage.h"
#include "GameObject.h"

ComponentExplosionDamage::ComponentExplosionDamage(float damage)
{
	this->damage = damage;
}


ComponentExplosionDamage::~ComponentExplosionDamage(void)
{
}

void ComponentExplosionDamage::onMessage(const Message& message)
{
	
	switch (message.type)
	{
	case Message::NEW_VISION:
		explode(message.gameObject);
		break;
	}
}


void ComponentExplosionDamage::update()
{
	Message message;
	message.type = Message::DEATH;
	parent->broadcastMessage(message);
	parent->kill();
}

void ComponentExplosionDamage::explode(GameObject* gameObject)
{
	Message message2;
	message2.type = Message::INCOMING_DAMAGE;
	message2.value = damage;
	gameObject->broadcastMessage(message2);
}
