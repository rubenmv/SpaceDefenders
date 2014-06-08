#include "StateTestClock.h"
#include <iostream>
#include <sstream>

#include "../GraphicsEngine.h"
#include "../GameManager.h"
#include "../GameObject.h"
#include "../GameObjectManager.h"
#include "../Math.h"

//Singleton
StateTestClock *StateTestClock::pInstance = NULL;

StateTestClock* StateTestClock::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateTestClock();

	return pInstance;
}


StateTestClock::StateTestClock()
{
	name = State::StateName::TEST_COLLISION;
}


StateTestClock::~StateTestClock()
{
}

void StateTestClock::init()
{
	gameManager = GameManager::getInstance();

	gameObjectManager = gameManager->getGameObjectManager();

	gameObjectManager->createPlayer();

	gameManager->getGraphicsEngine()->createLight();

	clock = Clock();
	clock2 = Clock();
	clock3 = Clock();
}

void StateTestClock::cleanup()
{
}

void StateTestClock::update(GameManager* gameManager)
{
	gameObjectManager->update();
	if(clock.getElapsedTime() >=5000 )
	{
		clock.restart();
	}

	std::wstringstream string;

	string << "Clock de 5 segundos\n";
	string << "Tiempo: " << clock.getElapsedTime() <<" ms\n";
	string << "Tiempo: " << Math::round(clock.getElapsedTime()/1000, 1) <<" s\n\n";


	if(clock2.getElapsedTime() >= 5000)
	{
		clock2.pause();
	}

	if(clock3.getElapsedTime() >= 10000)
	{
		clock2.unpause();
	}
	if(clock3.getElapsedTime() >= 15000)
	{
		clock2.restart();
		clock3.restart();
	}

	string << "Clock de 5 segundos para y luego inicia a los 5 segundos\n";
	string << "Tiempo: " << clock2.getElapsedTime() <<" ms\n";



	gameManager->getGraphicsEngine()->setDebugText(string.str().c_str());
}
