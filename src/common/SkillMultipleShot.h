#pragma once
#include "skill.h"
class SkillMultipleShot : public Skill
{
public:
	SkillMultipleShot(void);
	~SkillMultipleShot(void);

	void select();
	void deselect();
	void update();
	void activate();
	void addLogo();
	void addDescription();
	void addBlackLogo();
};

