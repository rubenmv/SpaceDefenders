#include <iostream>
#include "StateIntro.h"
#include "StateMenuMain.h"
#include "tests/StateTestWaves.h"
#include "MapManager.h"
#include "GameObjectManager.h"
#include "AudioManager.h"

// Singleton
StateIntro* StateIntro::pInstance = NULL;

StateIntro* StateIntro::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateIntro();

	return pInstance;
}

StateIntro::StateIntro()
{
	name = State::StateName::INTRO;
}

StateIntro::~StateIntro()
{
	pInstance = NULL;
}

void StateIntro::init()
{
	GameManager::getInstance()->initMapManager();
	GameManager::getInstance()->getAudioManager()->initBuffers();
	GameManager::getInstance()->getGameObjectManager()->loadModels();	
	//MapManager* mapManager = gameManager->getMapManager();
	next = true;
}
void StateIntro::cleanup()
{
}

void StateIntro::update(GameManager* gameManager)
{
	if(next)
	{
		//gameManager->changeState(StateTestWaves::getInstance());
		gameManager->changeState(StateMenuMain::getInstance());
		return;
	}
	
	// Aqui empieza el update
}
