#pragma once
#include "state.h"

class EventManager;

class StateSelectCharacter : public State
{
public:
	virtual ~StateSelectCharacter(void);
	static StateSelectCharacter* getInstance();

	void init();
	void cleanup();
	void update(GameManager* gameManager);

private:
	StateSelectCharacter();
	StateSelectCharacter(const StateSelectCharacter& orig);
	StateSelectCharacter &operator= (const StateSelectCharacter &);
	static StateSelectCharacter* pInstance;

	EventManager* eventManager;
	//std::vector<PlayerInfo> playerInfoList; 
	int character;
	bool selected;
};
