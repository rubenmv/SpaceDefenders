#pragma once
#include "component.h"
class ComponentCreateDebris :
	public Component
{
public:
	ComponentCreateDebris(int nDebris);
	~ComponentCreateDebris(void);
	
	void update();
	void onMessage(const Message& message);
private:
	void createDebris();
	int nDebris;
};

