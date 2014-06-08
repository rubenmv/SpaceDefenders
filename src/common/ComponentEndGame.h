#pragma once
#include "component.h"




class ComponentEndGame:	public Component
{
public:
	ComponentEndGame(void);
	~ComponentEndGame(void);

	virtual void onMessage(const Message& message);
	virtual void update();
};

