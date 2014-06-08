#include "ComponentMinesGenerator.h"
#include "GameManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Math.h"
#include "Vector2d.h"
#include "NetworkEngine.h"

ComponentMinesGenerator::ComponentMinesGenerator(float newMineTime)
{
	//Tiempo que tarda en crear otra nueva mina
	this->newMineTime = newMineTime;
	this->clock = Clock();

}


ComponentMinesGenerator::~ComponentMinesGenerator(void)
{
}


void ComponentMinesGenerator::update()
{
	
	if(clock.getElapsedTime()/1000 >= newMineTime)
	{
		GameObject* mine = GameManager::getInstance()->getGameObjectManager()->createMine();
		mine->position =  Vector2d(Math::random(-75,75), Math::random(-75,75));
		clock.restart();
	}
	

}

void ComponentMinesGenerator::onMessage(const Message& message)
{
	/*

	switch (message.type)
	{
	case Message:::
		
		break;
	}*/
}