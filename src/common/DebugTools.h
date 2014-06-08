#pragma once
#include "Message.h"
#include "Clock.h"
#include <vector>
#include "Vector2d.h"
#include "NodeMesh.h"
#include "GameObject.h"
#include "NodeAS.h"
#include <sstream>

class GameObject;
class GraphicsEngine;
class EventManager;
class CollisionManager;
class NodeText;
class MapManager;
class GameObjectManager;
class AIFlocking;

struct DebugMessage
{
	Message::Type type;
	int counter;
	Clock clock;
	bool isExpired();
};

// Struct con los tiempos que tarda cada update
struct UpdateTimes
{
	int gameManagerTime; // indica el tiempo total del update
	int gameObjectManagerTime;
	int collisionManagerTime;
	int graphicEngineTime;
	int verticalSyncTime;
	int debugToolsTime;
	int aiFlockingTime;
};

class DebugTools
{
public:
	const static unsigned int MAX_MESSAGE_TIME;

	DebugTools(void);
	void init();
	void cleanup();
	void update();

	void newMessageSend(GameObject* sender, Message message);
	virtual ~DebugTools(void);

	// OBSTACULOS
	std::vector<NodeMesh*> cubeObstacleList; // Obstaculos
	void createCubeObstacle(Vector2d position);
	void removeObstacles();
	// PATHFINDING
	std::vector<NodeMesh*> cubeNodeList; // Cubos del camino y adyacentes
	void printList(std::vector<NodeAS*> list); // Imprime por consola una lista de nodos
	void drawNodeList(std::vector<NodeAS*> list, int color); // Dibuja cubos
	void cleanCubeMeshList(); // Limpia los cubos creados por drawNodeList en cubeNodeList
	void drawPath(GameObject* gameObject, std::vector<Vector2d> path);
	bool nextStep; // Indica si se va a dibujar el siguiente paso
	bool allSteps; // Indica que se deben realizar todos los pasos
	void updateTextPathfinding(); // Actualiza el texto en pantalla con las string
	void updateTextWaves();
	std::string textSteps;
	std::wstringstream textTarget;

	//OLEADAS

	bool isShowingDebug()
	{
		return false;//showDebug;
	}

	void drawUpdateTimes(); //dibuja TODOS los tiempos de los distintos updates

	void addUpdateTimeText( int time, std::wstringstream &stream );
	void addPercentTimeText( int time, std::wstringstream &stream );
	bool isDebugRectanglesVisible();
private:

	GraphicsEngine* graphicsEngine;
	EventManager* eventManager;
	CollisionManager* collisionManager;
	MapManager* mapManager;
	AIFlocking* aiFlocking;
	GameObjectManager* gameObjectManager;

	std::vector<DebugMessage*> debugMessageList;
	NodeText* debugMessageText;
	NodeText* debugPathMessageText;
	NodeText* debugUpdateTimeText; // Nodo de texto para mostrar el texto del tiempo de los updates
	GameObject* debugMessageFilter;

	bool mousePressed;
	bool showDebug; //Se podria cambiar a debugVisible para que este mas claro
	bool showKeyPressed;
	static bool deleteExpiredMessages(DebugMessage* debugMessage);

	void updateFilter();
	void updateDebugMessages();
	void setMessageFilter(GameObject* newFilter);
	void checkInput();	

	Clock updateClockDT;
	int updateTime;

	bool isDebugRectVisible;
	void setDebugRectanglesVisible(bool);
};

