#pragma once
#include "skill.h"
#include "clock.h"
#include "GameObject.h"

class NodeMesh;

class SkillMine :	public Skill
{
public:
	SkillMine(int name);
	~SkillMine(void);

	void select();
	void deselect();
	void update();
	void activate();
	void addLogo();
	void addDescription();
	void addBlackLogo();
private:
	GameObject* mine;
	GameObject::Name name;
	NodeMesh* nodeMesh;
};

