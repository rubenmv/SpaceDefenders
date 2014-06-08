#pragma once
#include "../state.h"

class CollisionManager;
class PhysicsManager;
class StateBenchmark :	public State
{
public:
	virtual ~StateBenchmark(void);
		
	static StateBenchmark* getInstance();


	void init();
	void cleanup();

	void update(GameManager* gameManager);
private:
	StateBenchmark();
	StateBenchmark(const StateBenchmark& orig);
	StateBenchmark &operator= (const StateBenchmark &);
	static StateBenchmark* pInstance;
	
	void createFurys();
	void createBombers();
	void createWaves();
	void createTurrets();
	void createMines();
	void createWall();

	//Deberia ir en el GameObjectManager
	GameObjectManager* gameObjectManager;
	CollisionManager* collisionManager;
	PhysicsManager* physicsManager;
	MapManager* mapManager;
	AIEngine*  aiEngine;

	int frames;
	int numberBasics;
	int numberTurrets;
	int numberBomber;
	int numberFury;
	int numberMines;

	Clock clock;
};

