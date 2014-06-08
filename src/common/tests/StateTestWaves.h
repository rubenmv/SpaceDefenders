#pragma once
#include "../State.h"
#include "../CollisionManager.h"
class GameManager;
class GameObjectManager;
class Clock;
class MapManager;
class AIEngine;
class PhysicsManager;
class StateTestWaves: public State
{
public:
	static StateTestWaves* getInstance(int character);
	virtual ~StateTestWaves();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	static const int ENEMY_MULTIPLIER;
	static const int INITIAL_ENEMIES;
	static const int DEFEND_TIME;

	StateTestWaves(int character);
	StateTestWaves(const StateTestWaves& orig);
	StateTestWaves &operator= (const StateTestWaves &);
	static StateTestWaves* pInstance;
	

	//Deberia ir en el GameObjectManager
	GameObjectManager* gameObjectManager;
	CollisionManager* collisionManager;

	MapManager* mapManager;
	AIEngine*  aiEngine;

	PhysicsManager* physicsManager;

	void createWaveEnemies(int); //crea los enemigos en cada oleada
	void createPositionPath();
	Clock* waveClock; //Reloj para calcular el tiempo de las oleadas
	Clock* globalWaveClock; //Para calcular el tiempo total de la partida
	int waveNumber;
	int numberOfEnemies;

	int waveTime;
	int stateOfWave;
	unsigned int timeToDefend; //auxiliar porque no me deja usar directamente DEFEND_TIME
	int timeRemaining; //contador inverso del tiempo restante para defender

	int character;


};

