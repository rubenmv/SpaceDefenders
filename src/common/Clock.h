#pragma once

#include "raknet/GetTime.h"

class Clock
{
public:
	Clock();
	virtual ~Clock();

	static unsigned int currentTime();

	unsigned int getElapsedTime();
	void start();
	void pause();
	void unpause();
	unsigned int restart();

	bool isRunning();

private:
	RakNet::Time startTime;
	bool running;
	unsigned int offset;


};


