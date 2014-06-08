#pragma once
#include "component.h"
#include <vector>
#include <list>
class ComponentAIDefender :	public Component
{
public:
	ComponentAIDefender(GameObject* target, float attackRange);
	~ComponentAIDefender(void);

	void update();
	void onMessage(const Message& message);
private:
	void messageNewTarget(GameObject* gameObject);
	void messageNewAim(GameObject* gameObject);
	void messageStopAIM();
	void addObject(GameObject* gameObject);
	void deleteObject(GameObject* gameObject);
	void selectTarget();
	void selectAim();

	float attackRange;
	bool shooting;
	GameObject* target;
	GameObject* targetPlayer;
	std::list<GameObject* > onVisionList;
};

