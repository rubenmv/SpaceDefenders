#include "ComponentSkill.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "GraphicsEngine.h"

ComponentSkill::ComponentSkill(std::vector<Skill*> skills, int jade)
{
	down = false;
	level = 1;
	eventManager = GameManager::getInstance()->getEventManager();

	this->skills = skills;
	this->jade = jade;
}


ComponentSkill::~ComponentSkill(void)
{
}

void ComponentSkill::awake()
{
	for(size_t i = 0;i < skills.size();i++)
	{
		if(skills[i] != NULL)
		{
			skills[i]->setParent(parent);
			if(skillIsUsable(i+1))
			{
				skills[i]->addLogo();
			}
			else
			{
				skills[i]->addBlackLogo();
			}
			skills[i]->addDescription();
		}
	}
}

void ComponentSkill::update()
{
	if (!down)
	{
		if(GameManager::getInstance()->getEventManager()->mouseState.rightButtonDown)
		{
			for(size_t i = 0;i < skills.size();i++)
			{
				skills[i]->deselect();
			}
		}
		for(size_t i = 0;i < skills.size();i++)
		{
			skills[i]->update();
		}
	}
}

void ComponentSkill::useSkill(unsigned skillNumber)
{
	if(skillNumber > skills.size())
	{
		std::cout << "WARNING: Intentando utilizar la skill " << skillNumber << " cuando solo hay " << skills.size() << std::endl;
		return;
	}

	for(size_t i = 0;i < skills.size();i++)
	{
		skills[i]->deselect();
	}

	if(skillIsUsable(skillNumber))
	{
		skills[skillNumber-1]->select();
	}
}

void ComponentSkill::onMessage(const Message& message)
{
	Message newMessage;
	switch (message.type)
	{
	case Message::TRY_BUY:
		canBuy(message.value, message.gameObject);
		break;
	case Message::ADD_EXPERIENCE:
		jade += message.value*2/3;
		newMessage.type = Message::JADE_CHANGED;
		newMessage.value = jade;
		parent->broadcastMessage(newMessage);
		break;
	case Message::USE_SKILL:
		useSkill(message.value);
		break;
	case Message::LEVEL_CHANGED:
		addLevel(message.value);
		break;
	case Message::DEATH:
		down = true;
		for(size_t i = 0;i < skills.size();i++)
		{
			skills[i]->deselect();
		}
		break;
	case Message::REPAIRED:
		down = false;
		break;
	}

}

void ComponentSkill::addLevel(float level)
{
	this->level = level;
	for(size_t i = 0;i < skills.size();i++)
	{
		if(skills[i]->getRequiredLevel() == level)
		{
			skills[i]->addLogo();
		}
	}
}

void ComponentSkill::canBuy(int cost, GameObject* seller)
{
	Message message;
	if(cost <= jade)
	{
		jade = jade - cost;
		message.type = Message::SOLD;
		seller->broadcastMessage(message);

		message.type = Message::JADE_CHANGED;
		message.value = jade;
		parent->broadcastMessage(message);
	}
	else
	{
		message.type = Message::NOT_SOLD;
		seller->broadcastMessage(message);
	}
}

bool ComponentSkill::skillIsUsable( int skillNumber )
{
	Skill *skill = skills[skillNumber - 1];

	GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();

	if(down)
	{
		return false;
	}

	if(skill->getRequiredLevel() > level)
	{
		//Avisar GUI para que muestre mensaje al jugador
		//El jugador no tiene nivel suficiente
		graphicsEngine->setWarningMessage(GameConstants::SKILL_UNAVAILABLE_MESSAGE);

		return false;
	}

	if(skill->isOnCooldown())
	{
		//Avisar GUI para que muestre mensaje al jugador
		//La skill se esta recargando todavia
		GameManager::getInstance()->getGraphicsEngine()->setWarningMessage(GameConstants::COOLDOWN_MESSAGE);
		return false;
	}

	if(skill->getCost() > jade)
	{
		//Avisar GUI para que muestre mensaje al jugador
		//El jugador no tiene jade suficiente
		GameManager::getInstance()->getGraphicsEngine()->setWarningMessage(GameConstants::INSUFFICIENT_JADE_MESSAGE);

		return false;
	}
	return true;
}
