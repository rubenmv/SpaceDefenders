#pragma once
#include "state.h"
#include "NetworkEngine.h"
#include "ServerBrowser.h"
#include <vector>

class EventManager;


class StateServerBrowser :	public State
{
public:
	static StateServerBrowser* getInstance();
	virtual ~StateServerBrowser();
	
	void init();
	void cleanup();
	void update(GameManager* gameManager);

	void updateListBox();
	void joinServer();

private:
	StateServerBrowser();
	StateServerBrowser(const StateServerBrowser& orig);
	StateServerBrowser &operator= (const StateServerBrowser &);
	static StateServerBrowser* pInstance;

	EventManager* eventManager;
};

