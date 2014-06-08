#pragma once

#include "State.h"

class StateServerLobby : public State
{
public:
	static StateServerLobby* getInstance();
	virtual ~StateServerLobby();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateServerLobby();
	StateServerLobby(const StateServerLobby& orig);
	StateServerLobby &operator= (const StateServerLobby &);
	static StateServerLobby* pInstance;
	NetworkEngine* networkEngine;
	GameManager* gameManager;
};

