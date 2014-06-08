#pragma once
#include "skill.h"
#include "NodeMesh.h"
class SkillBomb : public Skill
{
public:
	SkillBomb(void);
	~SkillBomb(void);

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

