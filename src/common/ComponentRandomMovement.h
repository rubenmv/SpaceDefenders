#pragma once
#include "Component.h"
#include <vector>

class ComponentRandomMovement : public Component
{
public:
	ComponentRandomMovement(void);
	~ComponentRandomMovement(void);
	void update();
	void awake();
	void onMessage(const Message& message);
private:
	std::vector<Vector2d> targetList;
	int positionVector;
	void changeTarget(Vector2d direction,GameObject* targetObject);
	void changeTarget();

	void newDirection( Vector2d &direction );

};

