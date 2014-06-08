#pragma once
#include "component.h"
#include "Clock.h"

class EventManager;
class FXEngine;

struct HUDData
{
	int jade;
	Vector2d health; //Contiene la vida maxima y la actual
	Vector2d shield; //Contiene el escudo maximo y el actual
	int waves;
	Clock clock;
	int level;
	int exp;
	int nextExp;
	float cd;
};

class ComponentHUD : public Component
{
public:
	ComponentHUD(int jade, Vector2d health, Vector2d shield, int waves, int level, int exp, int nextExp);
	~ComponentHUD(void);
	
	void update();
	void onMessage(const Message& message);

	HUDData getData()
	{
		return data;
	}

private:
	HUDData data;
	EventManager* eventManager;
	FXEngine* fxEngine;
};

