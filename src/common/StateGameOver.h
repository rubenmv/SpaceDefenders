#pragma once
#include "state.h"

class EventManager;
class GraphicsEngine;
class Clock;

class StateGameOver : public State
{
public:
	static StateGameOver* getInstance();
	virtual ~StateGameOver();

	void init();
	void cleanup();

	void update(GameManager* gameManager);
	int getRound(){return levelRound;};
	Clock getWastedTime(){return wastedTime;};
	int getEnemiesKilled(){return enemiesKilled;};

	void setData(int round,Clock wastedTime,int enemiesDefeated)
	{
		this->levelRound = round;
		this->wastedTime = wastedTime; 
		wastedTime.pause();
		this->enemiesKilled = enemiesDefeated;
	}

private:
	StateGameOver();
	StateGameOver(const StateGameOver& orig);
	StateGameOver &operator= (const StateGameOver&);
	static StateGameOver* pInstance;

	EventManager* eventManager;
	GraphicsEngine* graphicsEngine;
	int levelRound;
	Clock wastedTime;
	int enemiesKilled;
};

