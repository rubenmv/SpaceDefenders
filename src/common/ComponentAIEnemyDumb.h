#pragma once
#include "Component.h"
#include <vector>

class ComponentAIEnemyDumb : public Component
{
public:
	ComponentAIEnemyDumb(std::vector<Vector2d> targetList, int startPositionIndex);
	virtual ~ComponentAIEnemyDumb();

	virtual void setParent(GameObject* parent)
	{
		this->parent = parent;
	}

	void awake();

	virtual void update();
	virtual void onMessage(const Message& message);

	void changedTarget();

protected:
	GameObject* parent;
	std::vector<Vector2d> targetList;
	unsigned int positionVector;
};

