#pragma once
#include "component.h"
#include <vector>
#include "Skill.h"

class EventManager;

class ComponentSkill :	public Component
{
public:
	ComponentSkill(std::vector<Skill*> skills, int jade);
	~ComponentSkill(void);

	void awake();
	void update();
	void onMessage(const Message& message);
	void useSkill(unsigned int skillNumber);
	bool skillIsUsable( int skillNumber );
	void addLevel(float level);
private:
	void canBuy(int cant, GameObject* seller);
	std::vector<Skill*> skills;
	bool down;
	EventManager* eventManager;
	int jade;
	float level;
};

