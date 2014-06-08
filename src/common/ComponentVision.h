#pragma once
#include "component.h"
#include "CollisionManager.h"
#include "GameObject.h"

class  GameManager;

/*
	Descripción de la clase
*/
class ComponentVision :public Component
{
public:
	ComponentVision(float visionRadius);
	~ComponentVision(void);

	void onMessage(const Message& message);

	void update();
	void onVision(Collision collision);
	float getSqrVisionRadius(){return sqrVisionRadius;};
	GameObject* getGameObject(){return parent;};
	void noVision(GameObject* gameObject);

private:
	float sqrVisionRadius;
	std::vector<GameObject*> onVisionList;
	std::vector<GameObject*> onVisionListTemp;
};

