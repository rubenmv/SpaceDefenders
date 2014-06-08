#pragma once
#include "Component.h"
class ComponentLevelExp :	public Component
{
public:
	ComponentLevelExp(int level = 0, float experience=0, float modHP=0, float modDmg=0);
	virtual ~ComponentLevelExp(void);

	virtual void update();
	virtual void onMessage(const Message& message);

private:
	void addExperience(float exp);
	float level;
	float experience;
	int nextLevel;
	float modHP;
	float modDmg;
};

