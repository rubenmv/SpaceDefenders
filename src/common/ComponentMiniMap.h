#pragma once

#include "Component.h"
#include "MinimapSprite.h"
#include "Clock.h"


const int MINIMAP_SIZE = 160;
const int CONSTANT_SCALE = 17;
class ComponentMiniMap: public Component
{
public:

	ComponentMiniMap(const char* spriteFilename, int color);
	~ComponentMiniMap(void);
	void awake();
	void update();

	void onMessage(const Message& message){};
private:
	MinimapSprite* minimapSprite;

	static const int MINIMAP_SIZE = 160;
	static const int CONSTANT_SCALE = 17;

	static const int REFRESH_RATE = 10; //Se refiere a cuantas veces por segundo se actualiza, el juego va a 60
	Clock clock;
	float scale;
	int delay;
	const char* spriteFilename;
	int color;
};

