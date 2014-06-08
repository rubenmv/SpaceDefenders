#pragma once

#include "State.h"

class StateIntro : public State
{
public:
	static StateIntro* getInstance();
	virtual ~StateIntro();

	void init();
	void cleanup();
	void update(GameManager* gameManager);

private:
	StateIntro();
	StateIntro(const StateIntro& orig);
	StateIntro &operator= (const StateIntro &);
	static StateIntro* pInstance;

	bool next;
};

