#pragma once
#include "component.h"

class NodeMesh;

class ComponentGraphicsStatics :	public Component
{
public:
	ComponentGraphicsStatics(const char* meshFilename, unsigned int color = 0);
	~ComponentGraphicsStatics(void);

	virtual void update();
	virtual void onMessage(const Message& message);

private:
	NodeMesh* staticNode;
};

