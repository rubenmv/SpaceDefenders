#pragma once
#include "Component.h"
#include "Clock.h"
#include <vector>

class GameObject;
class GraphicsEngine;

class ComponentHealth :	public Component
{
public:
	ComponentHealth(float health, float shield,float rechargeTime);
	virtual ~ComponentHealth(void);

	virtual void awake();

	virtual void update();
	virtual void onMessage(const Message& message);
private:
	
	float health;
	float maxHealth;
	float shield;
	float maxShield;
	float initialHP;
	float initialShield;
	// Cantidad que se recarga en cada actualizacion
	float rechargeRate;
	bool rechargeActivated;
	
	// Tiempo que tarda en comenzar a regenerarse el escudo
	float rechargeTime;
	Clock rechargeClock;
	// Tiempo que un jugador puede permanecer derribado antes de morir
	float deathTime;
	Clock deathClock;
	// Tiempo para revivir a un jugador 
	float reviveTime;
	Clock reviveClock;
	// Jugadores cercanos que pueden asistir cuando esta caido
	int playersVision; 
	

	void applyModifiers(float modifier, float level);
	void onIncomingDamage(float value);
	void onHealthChange();
	void onShieldChange();
	void repairShip();
//	void impactEnabled( Vector2d velocity);
	void clearImpactList();
	//Prueba

	std::vector<GameObject*> impactList;
	void hurt(float damage);
	void heal(float damage);
};

