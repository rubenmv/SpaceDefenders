#pragma once
#include "component.h"
#include <vector>

class NodeMesh;
class GameObject;
class ComponentInventory :	public Component
{
public:
	ComponentInventory(const char* meshFilename, unsigned int price, const char* meshFilenameStatic = "");
	~ComponentInventory(void);
		
	virtual void update();
	virtual void onMessage(const Message& message);
	void create();
	void destroy();
	void viewing();
private:
	NodeMesh* node;
	NodeMesh* nodeStatic;
	std::vector<GameObject*> obstacles;
	int color;
	int price;
	bool canBuild;
};

