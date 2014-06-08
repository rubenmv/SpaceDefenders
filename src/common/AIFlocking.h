#pragma once
#include <vector>
#include "GameObject.h"
#include "Vector2d.h"
#include "Clock.h"

class GameManager;
class EventManager;
class MapManager;

class AIFlocking
{
public:
	AIFlocking(void);
	~AIFlocking(void);

	void init();
	void update();
	void cleanup();

	void addEnemy(GameObject* enemy);
	void addPlayer(GameObject* player);

	unsigned int getUpdateTime();
private:
	GameManager* gameManager;
	EventManager* eventManager;
	MapManager* mapManager;
	bool surround;
	bool defend;

	std::vector<GameObject*> enemyList;
	std::vector<GameObject*> playersList;

	Vector2d separation(GameObject* enemy);
	Vector2d alignament(GameObject* enemy);
	Vector2d cohesion(GameObject* enemy);
	Vector2d evade(Vector2d target, GameObject* enemy);
	Vector2d seek(Vector2d target, GameObject* enemy);
	Vector2d arrive(Vector2d target, GameObject* enemy);
	Vector2d flee(Vector2d posFlee, GameObject* enemy);
	Vector2d obstacleAvoiding(GameObject* enemy);
	Vector2d followLeader(GameObject* leader, GameObject* enemy);
	Vector2d surroundLeader(GameObject* leader, GameObject* enemy, int pos);
	Vector2d defendLeader(GameObject* leader, GameObject* enemy, int pos, GameObject* player);
	
	void checkInput();

	Clock updateClockAIF;
	int updateTime;
};

