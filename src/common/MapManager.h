
#pragma once
#include <vector>
#include "Vector2d.h"
#include "ComponentPathfinder.h"
#include "Clock.h"

class MinimapSprite;
class GUIText;
class GraphicsEngine;

class MapManager
{
public:
	MapManager();
	MapManager(const MapManager& orig);
	~MapManager();


	struct Frame
	{

		char type;
		Vector2d flow; //Fuerza que indica las corrientes
		Vector2d wallEjection; //Fuerza que te expulsa de la pared
	};



	struct CollisionMap
	{
		int mapSize; // Tamaño del mapa en pixel
		int frameSize; // Tamaño de cada frame
		int numFrames; // Tamaño del mapa en frames
		std::vector< std::vector< Frame > > map;
	}collisionMap;

	// Inicializa el mapa de objetos estaticos
	void initMap();

	// Devuelve el valor en una casilla del mapa por fila, columna
	char checkMap(int col, int row);
	// Devuelve el valor en una casilla del mapa por posicion x, y
	char checkMap(Vector2d position);


	// Indica si la casilla es caminable
	bool isEmpty(int col, int row);
	bool isEmpty(Vector2d position);
	// Obtiene la fila y columna de un frame por posicion
	Vector2d getFrame(Vector2d position);
	// Devuelve las coordenadas del centro de un frame
	Vector2d getFrameCenter( int col, int row );
	Vector2d getFrameCenter(Vector2d position );
	void setWall(int col, int row); // Coloca un obstaculo en el mapa de colisiones
	void setEmpty(int col, int row); //Quita un obstaculo del mapa de colisiones

	//Calcula las fuerzas, segun si se le pasa true las genera, si es false las lee de un fichero
	void updateForces(bool dynamic);
	void calculateForce(int i,int j);
	void calculateLessForce(int i,int j);
	// Registra un nuevo componentPathfinder
	ComponentPathfinder* addComponentPathfinder(ComponentPathfinder* pathfinder);
	// Elimina un componentPathfinder de la lista
	void removeFromList(ComponentPathfinder* pathfinderToRemove);

	CollisionMap getCollisionMap(){return collisionMap;};

	// Lanza un vector y comprueba y hay algun obstaculo por el camino dentro de un radio
	bool canSeeTarget(Vector2d start, Vector2d end, float radius);
	Vector2d getFlowFieldForce(Vector2d position);
	Vector2d getWallEjectionForce(Vector2d position);
	
	unsigned int getUpdateTime();

	int getMapSize()
	{

		return collisionMap.mapSize;
	}


private:

	//Lee las fuerzas de un fichero
	void MapManager::readForces();
	//Genera y escribe fuerzas en un fichero
	void MapManager::writeForces();


	std::vector<ComponentPathfinder*> pathfinderList;
	// Comprueba si la fila y columna pertenecen al mapa
	bool isInMap(int col, int row);

	Clock updateClockAIE;
	int updateTime;

	MinimapSprite* screenLoading;
	MinimapSprite* barLoading;
	GUIText* textLoading;

	GameManager* gameManager;
	GraphicsEngine* graphicsEngine;
};


