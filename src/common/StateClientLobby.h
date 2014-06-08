#pragma once
#include "state.h"
#include <vector>
#include "NetworkEngine.h"

class EventManager;


class StateClientLobby :	public State
{
public:
	static StateClientLobby* getInstance();
	virtual ~StateClientLobby();

	void init();
	void cleanup();
	void update(GameManager* gameManager);


private:

	StateClientLobby();
	StateClientLobby(const StateClientLobby& orig);
	StateClientLobby &operator= (const StateClientLobby &);
	static StateClientLobby* pInstance;

	std::string ipServer;
	NetworkEngine* networkEngine;
	EventManager* eventManager;
	//std::vector<PlayerInfo> playerInfoList; 
	bool ready;
	int character;
	bool selected;
};

