#pragma once
#include "component.h"
class ComponentJade : public Component
{
public:
	ComponentJade(int basic);
	~ComponentJade(void);

	virtual void update();
	virtual void onMessage(const Message& message);
	void canBuy(int cant, GameObject* seller);
private:
	int jade;
};

