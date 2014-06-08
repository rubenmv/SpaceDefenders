#pragma once

#include <vector>
#include "Clock.h"
#include "GUIManager.h"
#include "NodeText.h"
class GameObject;

class Skill
{
public:
	Skill(){};
	virtual ~Skill(){};

	virtual void select() = 0;
	virtual void update() = 0;
	virtual void deselect() = 0;
	virtual void activate() = 0;
	float getCost(){return price;};
	unsigned int getDelay(){return delay;};
	float getRequiredLevel(){return levelRequired;};

	//True si la skill se esta recargando
	bool isOnCooldown() 
	{
		return timeSinceActivated.getElapsedTime() < delay;
	}

	void setParent(GameObject* parent) {this->parent = parent;};

	void setSkillNumber(int number){skillNumber = number;};
	virtual void addLogo() = 0;
	virtual void addBlackLogo() = 0;
	virtual void addDescription() = 0;

protected:
	unsigned int delay;
	float duration;
	bool selected;
	float price;
	float levelRequired;
	Clock timeSinceActivated;
	GameObject* parent;
	int skillNumber;
	bool active;
};