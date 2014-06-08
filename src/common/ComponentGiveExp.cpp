#include "ComponentGiveExp.h"
#include "GameManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "NetworkEngine.h"

ComponentGiveExp::ComponentGiveExp(float experience)
{
	this->experience = experience;
}


ComponentGiveExp::~ComponentGiveExp(void)
{

}

void ComponentGiveExp::update()
{

}

void ComponentGiveExp::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::DEATH:
		giveExp();
		break;
	}
}

void ComponentGiveExp::giveExp()
{
	if(!GameManager::getInstance()->isServer())
	{
		Message message;
		message.type = Message::ADD_EXPERIENCE;
		message.value = experience;
		
		GameManager::getInstance()->getGameObjectManager()->getMainPlayer()->broadcastMessage(message);
	}
	else
	{
		GameManager::getInstance()->getNetworkEngine()->giveExperience(experience);
	}
}