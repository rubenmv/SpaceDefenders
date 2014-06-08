#pragma once

#include "Component.h"
#include <vector>
#include <list>
#include "Path.h"
#include "Clock.h"

class GameObject;

class ComponentAIEnemy : public Component
{
public:
	
	ComponentAIEnemy(float attackRange);
	~ComponentAIEnemy(void);

	void update();



	void onMessage(const Message& message);
	void awake();
	
private:

	enum EnemyState{
		ATTACK,
		MOVE,
		RETURN,
		STATE_COUNT
	};

	void addObject(GameObject* gameObject);
	void deleteObject(GameObject* targetObject);
	void selectAim();
	void selectTarget();
	void messageNewTarget(GameObject* gameObject);
	void messageNewTargetPosition(Vector2d target);
	void messageNewAim(GameObject* gameObject);
	void messageStopAIM();
	void updateAttack();
	void updateMove();
	void updateReturn();



	bool watching;
	bool shooting;
	float attackRange;
	GameObject *target;
	EnemyState state;
	std::list<GameObject*> onVisionList;	


	bool selectedTarget;

	
	Path* path; //guarda el camino por el que tiene que ir
		/*
	Pide un nuevo camino, si le pasmos un false, es que lo hemos llamado desde un mensaje, 
	y si no es que lo llamamos porque estamos cerca del punto
	Si lo llamamos cerca del punto da igual el estado en el que este siempre se aplica
	*/
	void changeTarget( bool allStates);
	//Devuelve si se ha salido de la ruta
	float distanceFromThePath();
	Clock clock; //Para controlar el tiempo que pasa en estado de return
};

