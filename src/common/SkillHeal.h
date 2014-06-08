#pragma once
#include "skill.h"
class NodeMesh;
class SkillHeal : public Skill
{
public:
	SkillHeal(void);
	~SkillHeal(void);

	void select();
	void deselect();
	void update();
	void activate();
	void addLogo();
	void addDescription();
	void addBlackLogo();
private:
	float radius;
	NodeMesh* nodeMesh;
};

