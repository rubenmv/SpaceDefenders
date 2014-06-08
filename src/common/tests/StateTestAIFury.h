#pragma once
#include "..\State.h"
#include "../CollisionManager.h"

class NodeMesh;

class StateTestAIFury :	public State
{
public:
	
	virtual ~StateTestAIFury(void);
		
	static StateTestAIFury* getInstance();


	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateTestAIFury();
	StateTestAIFury(const StateTestAIFury& orig);
	StateTestAIFury &operator= (const StateTestAIFury &);
	static StateTestAIFury* pInstance;
	

	//Deberia ir en el GameObjectManager
	GameObjectManager* gameObjectManager;
	CollisionManager* collisionManager;

	MapManager* mapManager;
	AIEngine*  aiEngine;

	NodeMesh* mapMesh;
	Vector2d position;
	float rotation;
};

