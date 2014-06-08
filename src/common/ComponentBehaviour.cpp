#include "ComponentBehaviour.h"
#include "GameObject.h"
#include "GameManager.h"
#include "MapManager.h"


ComponentBehaviour::ComponentBehaviour()
{
	aiming = true;
}


ComponentBehaviour::~ComponentBehaviour()
{
}

void ComponentBehaviour::update()
{

	if(!aiming)
	{
		calculateWallForce();		
	}else
	{
		calculateCloseWallForce();
	}	
	

}

void ComponentBehaviour::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::NEW_AIM:
		aiming = true;
		break;
	case Message::STOP_AIM:
		aiming = false;
		break;
	}
}

void ComponentBehaviour::calculateCloseWallForce()
{
	Vector2d ejectionForce = GameManager::getInstance()->getMapManager()->getWallEjectionForce(parent->position);

	Vector2d ejectionDirection = ejectionForce;
	ejectionDirection.normalize();

	Vector2d velocityDirection = parent->velocity;
	velocityDirection.normalize();

	float intensity = velocityDirection.dot(ejectionDirection); //Devuelve 1 si la nave se aleja de la pared,-1 si va directa hacia ella, 0 si va en paralelo

	//Queremos que cuanto mas vaya hacia la pared mas fuerte sea la fuerza que lo expulse
	intensity = intensity < 0 ? 1.5 : 0 ; //Como queremos que lo expulse solo cuando vaya hacia la pared solo hay intensidad si el dot ha dado negativo

	parent->acceleration += ejectionForce * ejectionForce.getLength()/10 * intensity;
}

void ComponentBehaviour::calculateWallForce()
{
	Vector2d ejectionForce = GameManager::getInstance()->getMapManager()->getWallEjectionForce(parent->position);

	Vector2d ejectionDirection = ejectionForce;
	ejectionDirection.normalize();

	/*Vector2d velocityDirection = parent->velocity;
	velocityDirection.normalize();*/
	/*---Rotacion---------*/
	/*
	if(parent->velocity.getLength() > 0)
	{

		float desiredRotation  = velocityDirection.getAngle();
		float newRotation = parent->rotation - desiredRotation;
		newRotation = Math::warpAngle(newRotation);

		if(Math::abs(newRotation) < 0.001)
		{
			parent->rotation = desiredRotation;
		} else
		{
			parent->rotation -= newRotation * 0.5;
		}

		parent->rotation = Math::warpAngle(parent->rotation);

	}
	*/
	parent->acceleration += ejectionForce;
}
