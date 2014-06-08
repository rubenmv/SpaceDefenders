#include "ComponentMovementInterpolation.h"

#include <iostream>
#include "Clock.h"
#include "GameObject.h"
#include "NetworkEngine.h"

ComponentMovementInterpolation::ComponentMovementInterpolation()
{
	snapshotHistory = std::list<MovemementSnapshot>();
}

//Interpola el movimiento entre lastSnapshot y nextSnapshot
void ComponentMovementInterpolation::update()
{
	if(snapshotHistory.empty())
	{
		return;
	}

	//Solo hay un snapshot
	if(snapshotHistory.size() == 1)
	{
		const MovemementSnapshot &snapshot = snapshotHistory.front();
		parent->position = snapshot.position;
		parent->rotation = snapshot.rotation;
		return;
	}

	unsigned int currentTime = Clock::currentTime();

	MovemementSnapshot &previousSnapshot = snapshotHistory.front();

	//Empieza en la segunda posicion de la lista
	for (auto it = ++snapshotHistory.begin(); it != snapshotHistory.end(); ++it)
	{
		MovemementSnapshot &nextSnapshot = *it;
		
		if(currentTime <= nextSnapshot.time) // || &nextSnapshot == &snapshotHistory.back()) //La prediccion no funcionaba bien
		{
			float divisor = (float)(nextSnapshot.time - previousSnapshot.time);

			//Ocurre cuando llegan dos paquetes a la vez
			//Solo deberia pasar si los dos primeros paquetes del historial han llegado a la vez
			if(divisor == 0)
			{
				parent->position = nextSnapshot.position;
				parent->rotation = nextSnapshot.rotation;
				return;
			}

			float lerp = (float)(currentTime - previousSnapshot.time)/divisor;

			parent->position = previousSnapshot.position + (nextSnapshot.position - previousSnapshot.position) * lerp;

			float rotationDifference = nextSnapshot.rotation - previousSnapshot.rotation;

			rotationDifference = Math::warpAngle(rotationDifference);

			parent->rotation = previousSnapshot.rotation + rotationDifference * lerp;
			return;
		}
		previousSnapshot = *it;
	}

	//Llega hasta aqui solo cuando:
	//currentTime > snapshotHistory.back().time
	//Los snapshots se han quedado atras porque no se esta moviendo o porque no ha recibido el ultimo paquete
	const MovemementSnapshot &snapshot = snapshotHistory.back();
	parent->position = snapshot.position;
	parent->rotation = snapshot.rotation;

	//Si ha terminado de interpolar todos los paquetes puede que este muerto y por eso no reciba mas
	if(snapshot.death)
	{
		Message message;
		message.type = Message::DEATH;
		parent->broadcastMessage(message);
		parent->kill();
	}
}

void ComponentMovementInterpolation::onMessage(const Message& message)
{
	switch(message.type)
	{
	case Message::NEW_SNAPSHOT:
		onNewSnapshot(message.target, message.value, message.time);
		break;
	case Message::DEATH_SNAPSHOT:
		onDeathSnapshot(message.time);
	}
}

void ComponentMovementInterpolation::onNewSnapshot(Vector2d newPosition, float newRotation, unsigned int snapshotTime)
{
	unsigned int time = snapshotTime + NetworkEngine::SERVER_MILLISECONDS_BETWEEN_UPDATES * INTERPOLATION_RATIO;

	snapshotHistory.push_back(MovemementSnapshot(newPosition, newRotation, time));

	if(snapshotHistory.size() > MAX_HISTORY_LENGTH)
	{
		snapshotHistory.pop_front();
	}
	
}

void ComponentMovementInterpolation::onDeathSnapshot( unsigned int snapshotTime )
{
	MovemementSnapshot &deathSnapshot = snapshotHistory.back();
	deathSnapshot.death = true;
}






