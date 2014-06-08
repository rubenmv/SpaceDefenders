#pragma once

#include <vector>
#include "Clock.h"
#include "Vector2d.h"

#include "Component.h"
#include "NodeAS.h"


// Debug
#include "NodeMesh.h"
#include "DebugTools.h"

class GameManager;
class GraphicsEngine;
class MapManager;

class ComponentPathfinder :	public Component
{
public:
	ComponentPathfinder(float speed);
	virtual ~ComponentPathfinder(void);

	virtual void update();
	virtual void onMessage(const Message& message);

	// Devuelve el camino
	std::vector<Vector2d> getPath()
	{
		return path;
	}

	// Cambia el target e inicializa la busqueda si se puede
	void changeTarget(Vector2d newTarget);

	Vector2d getCurrentTarget()
	{
		return currentTarget;
	}
	Vector2d getFinalTarget()
	{
		return finalTarget;
	}
	Vector2d getParentPosition()
	{
		return parent->position;
	}

private:
	Vector2d currentTarget; // Target actual del camino
	Vector2d finalTarget; // Target destino
	float speed;
	Clock clock; // Para el tiempo que se tarda en calcular el camino
	bool isReached;
	bool pathFound;

	// Nodo inicio y fin de camino para mas comodidad
	NodeAS* origin;
	NodeAS* destiny;
	void seek();
	// Listas con las casillas a considerar o no en el pathfinding
	std::vector<NodeAS*> openList;
	std::vector<NodeAS*> closedList;
	std::vector<Vector2d> path;

	// Calcula y corrige la id de un nodo a partir de su posicion en cuadricula (a partir del center)
	void fixId(NodeAS* node);
	// Corrige el centro segun su posicion en la cuadricula
	void fixCenter(NodeAS* node);
	// Obtiene el nodo siguiente lo guarda en closedList (usa A*)
	void nextNode();
	// Obtiene la distancia del nodo actual al destino sin tener en cuenta diagonales ni obstaculos
	float getManhattanDistance(Vector2d current, Vector2d target);
	// Obtiene la distancia del nodo actual al destino teniendo en cuenta diagonales
	float getDiagonalDistance(Vector2d current, Vector2d target);
	// Fachada para el metodo de obtencion de distancia h
	float getHeuristicDistance(Vector2d current, Vector2d target);
	// Obtiene el nodo de menor coste F de la lista
	NodeAS* getMinCostNode(std::vector<NodeAS*> list);
	// Indica si un nodo esta en la lista
	bool isInList(NodeAS* node, std::vector<NodeAS*> list); // Por NodeAS
	bool isInList(int idNode, std::vector<NodeAS*> list); // Por Id
	NodeAS* getFromList(NodeAS* node, std::vector<NodeAS*> list); // Obtiene nodo de la lista

	
	// Obtiene lista de nodos adyacentes candidatos
	std::vector<NodeAS*> getAdjacents(NodeAS* currentNode, NodeAS* destiny);
	// Crea un nodo y lo inicializa a partir de la posicion y el destino
	NodeAS* createNode(int row, int col, NodeAS* parentNode);
	// Inicializa los parámetros para la búsqueda de un nuevo camino
	void init();
	// Limpia las listas usadas para el pathfinding
	void clearPathLists();
	// Realiza smoothing sobre el camino, elimina targes innecesarios
	void smoothPath();

	GameManager* gameManager;
	MapManager* mapManager;
	DebugTools* debugTools;

	GameObject* targetObject;
};

