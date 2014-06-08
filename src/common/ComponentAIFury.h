#pragma once
#include "component.h"
#include <list>

class GameObject;

class ComponentAIFury : public Component
{
public:
	ComponentAIFury(GameObject* targetObject);
	~ComponentAIFury(void);

	void update();
	void onMessage(const Message& message);
	void cheatBrother();
	void awake();

private:
	GameObject* brother;
	GameObject* target;

	void messageNewAim(GameObject* gameObject);
	void messageStopAim();
	//Selecciona y envia al caza a la primera posicion que debe ir
	void firstPosition( GameObject* target );
	//Posicion a la que se dirige al empezar.
	Vector2d targetPosition;
	bool reached;
	
};