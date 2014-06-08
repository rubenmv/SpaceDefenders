#include <iostream>

#include "Clock.h"

Clock::Clock()
{
	running = true;
	offset = 0;

	startTime = RakNet::GetTimeMS();
}

Clock::~Clock()
{
}

// Devuelve en milisegundos el tiempo que ha pasado desde que empezo
unsigned int Clock::getElapsedTime()
{
	unsigned int elapsedTime = offset;

	if (running)
	{
		elapsedTime += (unsigned int)(RakNet::GetTimeMS()-startTime);
	}
	return elapsedTime;
}

void Clock::start()
{
	running = true;
	startTime = RakNet::GetTimeMS(); 
	offset = 0;
}

void Clock::pause()
{
	if (running) 
	{
		running = false;
		offset += (unsigned int)(RakNet::GetTimeMS()-startTime);
	}
}

void Clock::unpause()
{
	if (!running) 
	{
		running = true;
		startTime = RakNet::GetTime();
	}

}
// Reinicia el reloj devolviendo el tiempo transcurrido en milisegundos

unsigned int Clock::restart()
{
	unsigned int time = getElapsedTime();
	start();
	return (time);
}

//Devuelve true si el reloj esta en marcha

bool Clock::isRunning()
{
	return running;
}

//Devuelve los ms que han pasado desde que se ha encendido el ordenador
unsigned int Clock::currentTime()
{
	return RakNet::GetTimeMS();
}

