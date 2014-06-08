#pragma once

#include "Component.h"

class NodeMesh;
class GameObject;

class ComponentGraphics :public Component
{
public:
	ComponentGraphics(const char* meshFilename, bool transparent = false, unsigned int color = 0); //Rojo
	~ComponentGraphics();

	virtual void update();
	virtual void onMessage(const Message& message);

private:
	NodeMesh* node;
	float lastRotation;

};

