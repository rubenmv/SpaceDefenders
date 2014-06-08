#pragma once
#include "component.h"
#include <vector>
#include <list>
#include "ComponentAIEnemy.h"
#include "Vector2d.h"

class GameObject;

class ComponentAIBomber : public Component
{
public:
	ComponentAIBomber(float attackRange);
	~ComponentAIBomber(void);

	void update();
	void onMessage(const Message& message);
	void awake();

private:
	enum EnemyState
	{
		ATTACK,
		MOVE,
		RETURN
	};


	std::list<GameObject* > defenders;
	std::list<GameObject* > players;
	void addObject(GameObject* gameObject);
	void deleteObject(GameObject* targetObject);
	void selectAim();
	void selectTarget();
	void messageNewTarget(GameObject* gameObject);
	void messageNewTargetPosition(Vector2d target);
	void messageNewAim(GameObject* gameObject);
	void updateAttack();
	void updateMove();
	void updateReturn();
	void defenderSquad();
	void attackSquad(GameObject* targetPlayer);
	bool watching;
	bool shooting;
	float attackRange;
	GameObject *target;
	EnemyState state;
	//Devuelve true si target1 tiene mayor pref que target 2
	//bool selectPref(GameObject* target1,GameObject* target2);
	std::list<GameObject*> onVisionList;	


	bool selectedTarget;

	/*---En el futuro lo hara la IACentral*/
	//std::vector<Vector2d> targetList;
//	Vector2d targetPosition; //guarda el camino por el que tiene que ir
	Path* path;
	//int positionVector; 
	void changeTarget( bool allStates);
	//Devuelve si se ha salido de la ruta
	float distanceFromThePath();
	void messageStopAIM();
	bool charging;
};

