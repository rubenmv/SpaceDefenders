#pragma once


#include "Clock.h"
#include "Vector2d.h"
#include <vector>
#include "Path.h"
#include "GameObject.h"
class GameObjectManager;



enum State_AIEngine{
	WAVE,
	DEFENSE,
	STATE_AIENGINE_COUNT
};

class AIEngine
{
public:
	AIEngine(void);
	~AIEngine(void);

	void update();

	void discountEnemy()
	{
		totalEnemiesKilled++;
		numEnemiesWave--;
	}


	int getLevelRound()
	{

		return levelRound;
	}

	int getNumEnemiesWave()
	{

		return numEnemiesWave;
	}

	int getTotalEnemiesRound()
	{

		return totalEnemiesRound;
	}
	int getTotalEnemiesRoundInGame()
	{

		return totalEnemiesRoundInGame;
	}

	//Vector2d newTargetPath(int num);


	void endGame();
	void addBase();
	//devuelve un indice del nuevo Path
	int getNextPath(int name , std::vector<Path*> pathList);
	
	//Te devuelve uno de los 3 prieros caminos, para iniciar la ruta
	Path* AIEngine::getFirstPath()
	{
		return firstPath;
	}

	//Recibe la coordenada de una estructura y la agrega a un path
	void addStruct(Vector2d position);
	//Recibe la coordenada de una estructura y la agrega a un path
	void deleteStruct(Vector2d position);

	//Asigna al Gameobject el camino mas proxima a donde esta.
	Path* newPath( GameObject* basic );

	int getTotalEnemiesKilled()
	{
		return totalEnemiesKilled;
	}

	Clock getGameTime()
	{
		return globalWaveClock;
	}
private:


	static const int ENEMY_MULTIPLIER;
	static const int INITIAL_ENEMIES;
	static const int DEFEND_TIME; 

	static const float BASIC_MULTIPLIER;
	static const float BOMBER_MULTIPLIER;
	static const float FURY_MULTIPLIER;


	void createEnemies(int num);
	GameObjectManager* gameObjectManager;

	//Crea todos los path posibles
	void createPaths();
	//update de todos los path posibles
	void updatePaths();
	void updateWave();
	void updateDefense();
	bool gameOver;
	Clock deathClock; // Reloj para dejar un tiempo despues de la muerte antes de cambiar de estado
	Clock globalWaveClock; //Para calcular el tiempo total de la partida
	Clock buildingClock; //controla el tiempo de construccion

	int levelRound;
	int numEnemiesWave; //Enemigos de la oleada
	int totalEnemiesRound; //Enemigos totales de la ronda
	int totalEnemiesRoundInGame; //Enemigos totales que han sido creados esta ronda del juego, sirve para controlar si no hay que crear mas oleadas.
	int enemiesRound; //Enemigos que quedan en la ronda
	int totalEnemiesKilled;
	unsigned int waveTime;
	int state;
	int numberBase; //Partes de la base

	Path* firstPath; //Inicio del camino
	void selectEnemyOfTheEnemyPool(); //coge un nuevo el enemigo de la bolsa
	void createEnemyPool(); //Crea la bolsa para cada ronda
	//int twoPath( std::vector<Path*> nextPathList ); //Selecciona el correcto entre 2 caminos
	//int treePath( std::vector<Path*> nextPathList);//Selecciona el correcto entre 3 caminos
	std::vector<Path*> pathList; //lista con los caminos para dibujar las lineas
	std::vector<int> EnemyPool; //lista con los enemigos que pueden salir en cada ronda
	float AIEngine::distanceFromThePath(Vector2d position, Path* path);//Devuelve la distancia entre una posicion y el camino
	int pathWithMoreStructs( std::vector<Path*> nextPathList ); //Devuelve el camino con mas estructuras
	int selectNextPath( std::vector<Path*> pathList ); //Selecciona el proximo camino 

};

