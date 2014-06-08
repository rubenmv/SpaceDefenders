#pragma once
#include "skill.h"

class NodeMesh;
class GameObject;

class SkillTargetProjectile : public Skill
{
public:
	SkillTargetProjectile(void);
	~SkillTargetProjectile(void);
	
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
	GameObject* projectile;
};

