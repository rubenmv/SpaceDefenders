#pragma once
#include "skill.h"

class GameObject;
class NodeMesh;
class SkillWall :
	public Skill
{
public:
	SkillWall(void);
	~SkillWall(void);

	void select();
	void deselect();
	void update();
	void activate();
	void addLogo();
	void addDescription();
	void addBlackLogo(){};
private:
	GameObject* wall;
	NodeMesh* nodeMesh;
};

