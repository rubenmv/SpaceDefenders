#include "ComponentEndGame.h"
#include "Message.h"
#include "GameObject.h"
#include "GameManager.h"
#include "AIEngine.h"
#include "GameObjectManager.h"

ComponentEndGame::ComponentEndGame(void)
{
	GameManager::getInstance()->getAIEngine()->addBase();
}


ComponentEndGame::~ComponentEndGame(void)
{
}



void ComponentEndGame::update()
{

}

void ComponentEndGame::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::DEATH:
		GameManager::getInstance()->getAIEngine()->endGame();
		break;
	}
}

