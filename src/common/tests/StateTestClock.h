#pragma once

#include "../State.h"
#include "../clock.h"

class GameManager;
class GameObjectManager;


class StateTestClock: public State
{
public:
	

	static StateTestClock* getInstance();
	virtual ~StateTestClock();

	void init();
	void cleanup();

	void update(GameManager* gameManager);
private:
	StateTestClock();
	StateTestClock(const StateTestClock& orig);
	StateTestClock &operator= (const StateTestClock &);
	static StateTestClock* pInstance;

	//Deberia ir en el GameObjectManager
	GameObjectManager* gameObjectManager;
	Clock clock;
	Clock clock2;
	Clock clock3;
};

