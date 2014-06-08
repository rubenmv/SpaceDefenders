#include "ComponentLevelExp.h"
#include "GameObject.h"

ComponentLevelExp::ComponentLevelExp(int level, float experience,float modHP, float modDmg)
{
	this->level = level;
	this->experience = experience;
	nextLevel = 100;
	this->modDmg = modDmg;
	this->modHP = modHP;
}


ComponentLevelExp::~ComponentLevelExp(void)
{

}

void ComponentLevelExp::update()
{

}

void ComponentLevelExp::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::ADD_EXPERIENCE:
		addExperience(message.value);
		break;
	}
}

void ComponentLevelExp::addExperience(float exp)
{
	Message message;
	if(experience + exp >= nextLevel)
	{
		experience = experience + exp - nextLevel;
		
		nextLevel *= 1.5;
		level++;

		message.type = Message::LEVEL_CHANGED;
		message.target = Vector2d(modHP,modDmg);
		message.value = level;
		parent->broadcastMessage(message);
		
		message.type = Message::NEXT_EXPERIENCE_CHANGED;
		message.value = nextLevel;
		parent->broadcastMessage(message);

	}
	else
	{
		experience += exp;
	}

	message.type = Message::EXPERIENCE_CHANGED;
	message.value = experience;

	parent->broadcastMessage(message);
}