#pragma once
#include "component.h"

class NodeMesh;

class ComponentGraphicsShadow :	public Component
{
public:
	ComponentGraphicsShadow(const char* meshFilename, bool transparent = false, unsigned int color = 0);
	~ComponentGraphicsShadow(void);

	virtual void update();
	virtual void onMessage(const Message& message);

private:
	NodeMesh* node;
	float lastRotation;
};

