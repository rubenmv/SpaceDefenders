#include "ComponentMovement.h"
#include "GameObject.h"
#include "GameManager.h"
#include "MapManager.h"

ComponentMovement::~ComponentMovement(void)
{
}

void ComponentMovement::update()
{
		Vector2d friction = parent->velocity *-0.1;
		
		parent->acceleration += friction;

		/*
		if(parent->acceleration.getLength() > maxSpeed/10.0)
		{
			parent->acceleration.setLength(maxSpeed/10.0);
		}
		*/

		//Porque hay un 60 al sumar la aceleracion?
		//A la aceleracion no le afectaba el tiempo, por lo que la aceleracion era menos potente cuantos menos fps tenia el juego
		//Como los valores de aceleracion los hemos ajustado para 60 fps(deltaTime = 1/60), al ser ahora en funcion del tiempo un 
		//parche para que funcione con los mismos valores es multiplicarlo por 60 para que  acc * (1/60) * 60 = acc
		parent->velocity += parent->acceleration * GameManager::getInstance()->getDeltaTime() * 60; 
		parent->acceleration *= 0;

		float speed = parent->velocity.getLength();

		if(speed > maxSpeed)
		{
			parent->velocity.setLength(maxSpeed);
		} else if(speed < 0.01) //La friccion nunca baja la velocidad a 0 y entonces siempre se estaria moviendo
		{
			parent->velocity *= 0;
		}

		parent->position += parent->velocity * GameManager::getInstance()->getDeltaTime();
		
		//El target input controla la rotacion del jugador
		//Si esta apuntando, la rotacion la controla el ComponentTargetAim
		if(!parent->isPlayer() && !aiming)
		{
			float lerp = 1; //Lerp de 1, indica 100%, hace que miren hacia donde apunte su velocidad. 

			if(parent->getTag() == GameObject::ENEMY)
			{
				//Es para que los enemigos giren poco a poco 
				lerp = 0.1; 
				//Lerp de 0.1, indica 10%, hace que en cada frame solo gira un 10%
				//de los angulos que le faltan para que apunte a donde apunte su velocidad
			}
			
			Vector2d velocityDirection = parent->velocity;
			velocityDirection.normalize();
			/*---Rotacion---------*/

			float desiredRotation  = velocityDirection.getAngle();
			float deltaRotation = parent->rotation - desiredRotation;
			deltaRotation = Math::warpAngle(deltaRotation);

			if(Math::abs(deltaRotation) < 0.1)
			{
				parent->rotation = desiredRotation;
			} else
			{
				parent->rotation -= deltaRotation * lerp;
			}

			parent->rotation = Math::warpAngle(parent->rotation);

		}

}

void ComponentMovement::onMessage(const Message& message)
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

