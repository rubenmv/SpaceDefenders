#pragma once

class Entity
{
public:
	virtual~Entity(void){};

	virtual void beginDraw() = 0;
	virtual void endDraw() = 0;
};

