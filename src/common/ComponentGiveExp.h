#pragma once
#include "component.h"
class ComponentGiveExp : public Component
{
public:
	ComponentGiveExp(float experience);
	~ComponentGiveExp(void);

	void update();
	void onMessage(const Message& message);

private:
	void giveExp();
	float experience;
};

