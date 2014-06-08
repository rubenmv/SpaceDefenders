#pragma once
#include "skill.h"
#include "GameObject.h"

class NodeMesh;

class SkillTurret :	public Skill
{
public:
	SkillTurret(int name);
	~SkillTurret(void);
	
	void select();
	void deselect();
	void update();
	void activate();
	void addLogo();
	void addDescription();
	void addBlackLogo();
private:
	GameObject* turret;
	GameObject::Name name;
	NodeMesh* nodeMesh;
};

