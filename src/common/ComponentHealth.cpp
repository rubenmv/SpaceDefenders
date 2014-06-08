#include "ComponentHealth.h"
#include "GameManager.h"
#include "MapManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include <iostream>

ComponentHealth::ComponentHealth(float health, float shield, float rechargeTime)
{
	this->health = health;
	this->maxHealth = health;
	this->shield = shield;
	this->maxShield = shield;
	this->initialHP = health;
	this->initialShield = shield;
	this->rechargeActivated = true;
	// El tiempo que tarda en comenzar a regenerarse el escudo
	this->rechargeTime = rechargeTime; 
	this->rechargeClock = Clock();
	// Tiempo que un jugador puede permanecer derribado antes de morir
	this->deathTime = 40000; // 40 segundos
	this->deathClock = Clock();
	// Tiempo para revivir a un jugador 
	this->reviveTime = 5000; // 5 segundos
	this->reviveClock = Clock();
	this->playersVision = 0;
}


ComponentHealth::~ComponentHealth(void)
{

}

void ComponentHealth::awake()
{
	//Lo necesita el componentDebugGameObject para saber la vida
	onHealthChange();
	onShieldChange();
}

void ComponentHealth::applyModifiers(float modifier, float level)
{
	float proportion = shield / maxShield;

	maxShield = initialShield *(1 + modifier*level);
	shield = maxShield*proportion;

	proportion = health / maxHealth;
	//Ahora en el nivel 1 tienes 100 de vida y el modificador es 0.2, tu vida aumentara un 20% por cada nivel
	//En el nivel 2 tendras 120, en el 3 tendras 140, etc
	maxHealth = initialHP *(1 + modifier*level);
	health = maxHealth*proportion;

	onHealthChange();
	onShieldChange();
}

void ComponentHealth::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::INCOMING_DAMAGE:
		onIncomingDamage(message.value);		
		break;
	case Message::LEVEL_CHANGED:
		applyModifiers(message.target.x,message.value);
		break;
	case Message::REPAIRED:
		repairShip();
		break;
	}
}


void ComponentHealth::onIncomingDamage(float damage)
{
	if(health <= 0)
	{
		return;
	}

	if(damage > 0) //Daño normal
	{
		hurt(damage);
	}
	else			//Cura
	{
		heal(damage * -1);
	}
}

void ComponentHealth::update()
{
	// Recarga del escudo
	if(shield < maxShield && health > 0)	
	{
		if(rechargeActivated) // Recarga cada cierto tiempo indicado
		{
			if(rechargeClock.getElapsedTime() >= 100)
			{
				shield += maxShield * 0.01;
				if(shield > maxShield)
				{
					shield = maxShield;
				}
				onShieldChange();
				rechargeClock.restart();
			}
		} 
		else
		{
			// Se espera un tiempo antes de activar la recarga
			if(rechargeClock.getElapsedTime() >= rechargeTime)
			{
				rechargeActivated = true;
				rechargeClock.restart();
			}
		}
	}
}

void ComponentHealth::hurt(float damage)
{
	bool healthChanged = false;
	bool shieldChanged = false;
	
	if(shield > 0)
	{
		if(shield >= damage)
		{
			shield -= damage;
			shieldChanged = true;
		}
		else
		{
			damage -= shield;
			shield = 0;
			health -= damage;
			shieldChanged = true;
			healthChanged = true;
		}
		rechargeActivated = false;
		rechargeClock.restart();
	}
	else
	{
		if(health > 0)
		{
			health -= damage;
			
			healthChanged = true; 

		}

		//clearImpactList();
	}

	if(healthChanged)
	{
		health = Math::max_(health, 0);
		onHealthChange();
	}

	if(shieldChanged)
	{
		onShieldChange();
	}

	if ( health <= 0 )
	{
		Message message;
		message.type = Message::DEATH;
		parent->broadcastMessage(message);

		// Si es un jugador, lo derriba
		if (!parent->isPlayer())
		{
			parent->kill();
		}
		
	}
}

void ComponentHealth::heal(float damage)
{
	bool healthChanged = false;
	bool shieldChanged = false;
	
	if(health < maxHealth)
	{
		if(health + damage <= maxHealth)
		{
			health += damage;
			healthChanged = true;
		}
		else
		{
			damage -= maxHealth-health;
			shield += damage;
			shield = Math::min_(maxShield,shield);
			health = maxHealth;
			shieldChanged = true;
			healthChanged = true;
		}
	}
	else
	{
		shield += damage;
		shield = Math::min_(maxShield,shield);
			
		healthChanged = true; 
	}

	if(healthChanged)
	{
		onHealthChange();
	}

	if(shieldChanged)
	{
		onShieldChange();
	}
}

void ComponentHealth::onHealthChange()
{

	Message message;
	message.type = Message::HEALTH_CHANGED;
	message.target = Vector2d(health,maxHealth);
	parent->broadcastMessage(message);

	

}

void ComponentHealth::onShieldChange()
{
	Message message;
	message.type = Message::SHIELD_CHANGED;
	message.target = Vector2d(shield,maxShield);
	parent->broadcastMessage(message);
}

void ComponentHealth::repairShip()
{
	if(health > 0)
	{
		std::cout << "WARNING: Reparando una nave que no estaba muerta\n";
	}
	
	health = maxHealth;
	shield = maxShield;
	onHealthChange();
	onShieldChange();
}


