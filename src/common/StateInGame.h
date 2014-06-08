#ifndef STATEINGAME_H
#define	STATEINGAME_H

#include "State.h"

class StateInGame :	public State
{
public:
	static StateInGame* getInstance();
	virtual ~StateInGame();

	void init();
	void cleanup();

	void update(GameManager* gameManager);

private:
	StateInGame();
	StateInGame(const StateInGame& orig);
	StateInGame &operator= (const StateInGame &);
	static StateInGame* pInstance;

};

#endif	/* STATEINGAME_H */

