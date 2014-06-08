#include <iostream>
#include "StateInGame.h"
#include "StateMenuMain.h"

StateInGame* StateInGame::pInstance = NULL;

StateInGame* StateInGame::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateInGame();

	return pInstance;
}

StateInGame::StateInGame()
{
	name = State::StateName::IN_GAME;
}

StateInGame::~StateInGame()
{
}

void StateInGame::init()
{
}
void StateInGame::cleanup()
{
}

void StateInGame::update(GameManager* gameManager)
{

}

