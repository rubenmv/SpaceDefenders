#pragma once

#include <vector>

#include "Component.h"
#include "NodeAS.h"
#include "Vector2d.h"

class GameManager;
class GraphicsEngine;

/*
	Mueve el objeto hacia una posicion y mantiene esta posicion actualizada
	segun la posicion del objeto target, que puede estar moviendose

	CUIDADO: Este componente chafa la rotacion de ComponentBehaviour si esta 
	detras de este en la lista, deberia estar antes o puede que haga mal los giros 

*/

class ComponentTargetPath :	public Component
{

public:
	ComponentTargetPath(float speed, float targetRadius = 20);
	virtual ~ComponentTargetPath(void);

	virtual void awake();
	virtual void update();


	virtual void onMessage(const Message& message);


private:
	Vector2d target;
	float speed;
	bool isReached;
		
	GameManager* gameManager;

	GameObject* targetObject;
	float targetRadius;
	void changeTarget(Vector2d newTarget);
	void targetReached();
	void setTarget(GameObject* newTarget);

};

