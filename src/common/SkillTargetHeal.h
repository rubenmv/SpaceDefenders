#pragma once
#include "skill.h"

class NodeMesh;

class SkillTargetHeal :	public Skill
{
public:
	SkillTargetHeal(void);
	~SkillTargetHeal(void);	

	void select();
	void deselect();
	void update();
	void activate();
	void addLogo();
	void addDescription();
	void addBlackLogo();
private:
	float damage;
	NodeMesh* nodeMesh;
};

