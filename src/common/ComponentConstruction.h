#pragma once
#include "component.h"
#include "Clock.h"

class NodeMesh;
class ComponentConstruction : public Component
{
public:
	ComponentConstruction(const char* meshFilename, const char* meshFilenameStatic = "");
	~ComponentConstruction(void);
		
	virtual void update();
	virtual void onMessage(const Message& message);
	void create();
	void building();
private:
	NodeMesh* node;
	NodeMesh* nodeStatic;

	Clock timeBuild;
};

