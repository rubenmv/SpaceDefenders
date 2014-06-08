#pragma once
#include "Component.h"

/*
	Actualiza la posicion a partir de la velocidad actual.
	Es el componente que hace que un objeto finalmente se mueva.
*/
class ComponentMovement : public Component
{
public:
	ComponentMovement(float maxSpeed = 100): maxSpeed(maxSpeed), aiming(false) {};
	~ComponentMovement(void);
	

	virtual void update();
	void onMessage(const Message& message);
private:
	bool aiming;
	float maxSpeed;
	
};

