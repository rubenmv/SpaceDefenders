#pragma once
#include "component.h"
#include <vector>

class NodeMesh;
class GameObject;
class GraphicsEngine;
class EventManager;
class MapManager;
class NodeText;

class ComponentWallConstruction : public Component
{
public:
	ComponentWallConstruction(const char* meshFilename, unsigned int price);
	~ComponentWallConstruction(void);

	void viewing();
	void buildingPhase();
	void update();
	void onMessage(const Message& message);
	void create();
	void destroy();
	void bresenham();

private:
	GraphicsEngine* graphicsEngine;
	EventManager* eventManager;
	MapManager* aiAux;
	Vector2d firstPosition;
	Vector2d lastPosition;

	bool building;
	bool canBuild;
	NodeMesh* begin;
	NodeMesh* end;
	NodeMesh* wall;
	NodeText* priceToShow;
	unsigned int price;
	std::vector<GameObject*> obstacles;
};

