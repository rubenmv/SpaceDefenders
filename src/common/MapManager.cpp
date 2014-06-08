#include "MapManager.h"
#include <iostream>
#include "Vector2d.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "MinimapSprite.h"

//Libreria para pasar string a float

//#include "string.h"
//solo para leer ficheros luego borrar
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

MapManager::MapManager()
{
	gameManager = GameManager::getInstance();
	graphicsEngine = gameManager->getGraphicsEngine();

	collisionMap.frameSize = 15;
	collisionMap.numFrames = 30;
	collisionMap.mapSize = collisionMap.frameSize * collisionMap.numFrames; // En pixel	

	if(!gameManager->isServer())
	{
		int width = gameManager->getGameSettings()->getWidth();
		int height = gameManager->getGameSettings()->getHeight();
		screenLoading = new MinimapSprite("../assets/images/gui/loading.jpg",Vector2d(0,0),Vector2d(width,height));
		//200 tamaño del sprite de barra
		barLoading = new MinimapSprite("../assets/images/gui/barLoading.png",Vector2d(width/2-200,height/2+150),Vector2d(400,15));
		barLoading->setScale(Vector2d(0,15));
		// Texto

		graphicsEngine->drawGUI();
	}

	initMap();
	delete screenLoading;
	delete barLoading;
}

MapManager::~MapManager()
{
	collisionMap.map.clear();
	pathfinderList.clear();
	updateTime=0; 
}

// Inicializa los muros
void MapManager::initMap()
{
	/*-----Leer escenario de un fichero modo rapido y cutre-----*/
	FILE *file;
	char character;

	fopen_s(&file,"../data/map.txt","r");

	if (file == NULL)
	{

		std::cout<<"Error de apertura del archivo. \n";
	}
	else
	{
		character = fgetc(file);
		int num  = 0;
		while (character != '\n')
		{
			num *= 10;
			num += character-48;
			character = fgetc(file);
		}
		collisionMap.numFrames  = num;
		collisionMap.mapSize = collisionMap.numFrames * collisionMap.frameSize;

		//Esto se realizaba al princpio pero al leer ahora el tamaño del mapa es necesario cambiarlo para iniciarlizar bien
		collisionMap.map.clear();
		// Inicializamos la matriz de vectores con el tamaño indicado (en frames)
		collisionMap.map.resize(collisionMap.numFrames);
		for(int i = 0 ; i < collisionMap.numFrames ; ++i)
		{
			collisionMap.map[i].resize(collisionMap.numFrames);
		}


		for (int i = 0; i < collisionMap.numFrames; i++)
		{
			for (int j = 0; j < collisionMap.numFrames; j++)
			{

				collisionMap.map[j][i].wallEjection = Vector2d(0,0);
				character = fgetc(file);								
				
				if(character == '\n') //comprueba salto de linea
				{
					character = fgetc(file);		
				}

				if(character == 'W')
				{
					collisionMap.map[j][i].type = 'W';
				}
				else
				{
					collisionMap.map[j][i].type = 'E';
				}
			}
		}
		fclose(file);
	}

	/*---------Agrega las fuerzas si es true, las genera y guarda y si es false las lee--*/
	updateForces(false);
}

void MapManager::updateForces( bool dynamic)
{
	/*for(int i=0;i<collisionMap.numFrames;i++)
	{
		for(int j=0;j<collisionMap.numFrames;j++)
		{
			collisionMap.map[i][j].wallEjection=Vector2d();
		}
	}*/
	if(dynamic)
	{
		writeForces();

	}else
	{

		readForces();
	}
	
}


void MapManager::readForces()
{

	std::ifstream in("../data/mapForces.txt");
	std::string line;
	int num = 0;
	//cuenta las lineas del fichero
	while(getline(in,line))
	{
		num++;

	}
	in.close();
	in.open("../data/mapForces.txt");
	if (in == NULL || num != (collisionMap.numFrames*collisionMap.numFrames)*2)
	{

		std::cout<<"\n\nError de apertura del archivo de fuerzas o no cumple los requisitos. \n\n";
	}
	else
	{
	
		for (int i = 0; i < collisionMap.numFrames; i++)
		{
			//400 y 15 tamaño del sprite;
			if(!gameManager->isServer())
			{
				barLoading->setScale(Vector2d(400/collisionMap.numFrames*i,15));
				graphicsEngine->drawGUI();
			}
			for (int j = 0; j < collisionMap.numFrames; j++)
			{
				getline(in,line);
				collisionMap.map[i][j].wallEjection.x = atof(line.c_str());
				getline(in,line);
				collisionMap.map[i][j].wallEjection.y = atof(line.c_str());			
			}
		}
	}
	in.close();
}

void MapManager::writeForces()
{

	std::ofstream out("../data/mapForces.txt");

	if (out == NULL)
	{

		std::cout<<"Error de apertura del archivo. \n";
	}
	else
	{
		out.clear();
		for (int i = 0; i < collisionMap.numFrames; i++)
		{
			//400 y 15 tamaño del sprite;
			if(!gameManager->isServer())
			{
				barLoading->setScale(Vector2d(400/collisionMap.numFrames*i,15));
				graphicsEngine->drawGUI();
			}

			for (int j = 0; j < collisionMap.numFrames; j++)
			{			

				for (int x = 0; x < collisionMap.numFrames; x++)
				{
					for (int y = 0; y < collisionMap.numFrames; y++)
					{


						if(collisionMap.map[x][y].type == 'W') //es la W
						{
							Vector2d direction =    getFrameCenter(x,y) - getFrameCenter(i,j);
							float distance = direction.getLength();	
							direction.normalize();

							float maxDistance = 70;

							if(distance !=  0 && distance < maxDistance)
							{
								//Para 30 de maxDistance: 
								//Con distancia  0 la fuerza es ( 0 - 30) = -30
								//Con distancia 15 la fuerza es (15 - 30) = -15
								//Con distancia 30 la fuerza es (30 - 30) =   0 

								collisionMap.map[i][j].wallEjection +=  direction * (distance - maxDistance)/25;
							}
						}
					}
				}
				out << collisionMap.map[i][j].wallEjection.x <<"\n";
				out << collisionMap.map[i][j].wallEjection.y <<"\n";
			}			
		}
		
	}
	out.close();
}

/*
std::string charToString(char character)
{


}*/
void MapManager::calculateForce(int i,int j)
{
	for (int x = 0; x < collisionMap.numFrames; x++)
	{
		for (int y = 0; y < collisionMap.numFrames; y++)
		{
			Vector2d direction =    getFrameCenter(i,j) - getFrameCenter(x,y);
			float distance = direction.getLength();	
			direction.normalize();

			float maxDistance = 30;

			if(distance !=  0 && distance < maxDistance)
			{
				//Para 30 de maxDistance: 
				//Con distancia  0 la fuerza es ( 0 - 30) = -30
				//Con distancia 15 la fuerza es (15 - 30) = -15
				//Con distancia 30 la fuerza es (30 - 30) =   0 

				collisionMap.map[x][y].wallEjection +=  direction * (distance - maxDistance)/2;
			}
		}
	}
}

void MapManager::calculateLessForce(int i,int j)
{
	for (int x = 0; x < collisionMap.numFrames; x++)
	{
		for (int y = 0; y < collisionMap.numFrames; y++)
		{
			Vector2d direction =    getFrameCenter(i,j) - getFrameCenter(x,y);
			float distance = direction.getLength();	
			direction.normalize();

			float maxDistance = 30;

			if(distance !=  0 && distance < maxDistance)
			{
				//Para 30 de maxDistance: 
				//Con distancia  0 la fuerza es ( 0 - 30) = -30
				//Con distancia 15 la fuerza es (15 - 30) = -15
				//Con distancia 30 la fuerza es (30 - 30) =   0 

				collisionMap.map[x][y].wallEjection -=  direction * (distance - maxDistance)/2;
			}
		}
	}
}
ComponentPathfinder* MapManager::addComponentPathfinder(ComponentPathfinder* pathfinder)
{
	pathfinderList.push_back(pathfinder);
	return pathfinder;
}

void MapManager::removeFromList(ComponentPathfinder* pathfinderToRemove)
{
	std::vector<ComponentPathfinder*>::iterator iterator = pathfinderList.begin();
	while(iterator != pathfinderList.end())
	{
		ComponentPathfinder* pathfinder = *iterator;
		if(pathfinder == pathfinderToRemove)
		{
			pathfinderList.erase(iterator);
			return;
		}
		else
		{
			++iterator;
		}
	}
}

bool MapManager::isInMap(int col, int row)
{
	if (col >= 0 && col < collisionMap.numFrames && row >= 0 && row < collisionMap.numFrames)
	{
		return true;
	}

	return false;
}

char MapManager::checkMap(int col, int row)
{
	char value = 'W';

	if (isInMap(col, row))
	{
		value = collisionMap.map[col][row].type;
	}

	return value;
}

char MapManager::checkMap(Vector2d position)
{
	char value = 'W';

	// Casilla a partir de posicion
	Vector2d frame = getFrame(position);
	frame = Vector2d(frame.y,frame.x);

	if (isInMap(frame.x, frame.y))
	{
		value = collisionMap.map[frame.x][frame.y].type;
	}

	return value;
}


// Modifica la matriz (pone 'W' (wall) en (col,row))	
void MapManager::setWall(int col, int row) 
{
	if (isInMap(col, row))
	{
		if(isEmpty(col, row))
		{
			collisionMap.map[col][row].type = 'W';
			//Esto se podria quitar???
			//onMapChange();

			calculateForce(col,row);
			if(GameManager::getInstance()->getNetworkEngine()->isStarted() && GameManager::getInstance()->isServer())
			{
				GameManager::getInstance()->getNetworkEngine()->messageSetWall(col,row);
			}
		}
	}
}

void MapManager::setEmpty(int col, int row)
{
	if (isInMap(col, row))
	{
		if(!isEmpty(col, row))
		{
			collisionMap.map[col][row].type = 'E';

			calculateLessForce(col,row);
			if(GameManager::getInstance()->getNetworkEngine()->isStarted() && GameManager::getInstance()->isServer())
			{
				GameManager::getInstance()->getNetworkEngine()->messageSetEmpty(col,row);
			}
		}
	}
}

// Indica si la casilla es caminable/alcanzable
bool MapManager::isEmpty(int col, int row)
{
	if(checkMap(col, row) == 'E')
	{
		return true;
	}

	return false;
}

bool MapManager::isEmpty(Vector2d position)
{
	if(checkMap(position) == 'E')
	{
		return true;
	}

	return false;
}

Vector2d MapManager::getFrame( Vector2d position )
{
	// y es fila
	int row = (int)(-position.y / collisionMap.frameSize);
	// x es columna
	int col = (int)(position.x / collisionMap.frameSize);

	return Vector2d(row, col);
}

// Devuelve las coordenadas del centro del nodo (x, y)
Vector2d MapManager::getFrameCenter( int col, int row)
{
	int frameSize = collisionMap.frameSize;

	Vector2d center = Vector2d( frameSize * col + (frameSize / 2), frameSize * row + (frameSize / 2) );

	center.y *= -1;

	return center;
}

Vector2d MapManager::getFrameCenter( Vector2d position )
{
	Vector2d frame = getFrame(position);

	return getFrameCenter(frame.y,frame.x);
}

// Se mueve en el vector entre dos posiciones y comprueba si algun
// se encuentra en un frame no caminable
bool MapManager::canSeeTarget( Vector2d start, Vector2d end, float radius )
{
	return true;
	Vector2d startFrame = getFrame(start);
	startFrame = Vector2d(startFrame.x,startFrame.y);

	if(!isInMap(startFrame.x, startFrame.y))
	{
		return true;
	}

	Vector2d endFrame = getFrame(end);
	endFrame = Vector2d(endFrame.x,endFrame.y);

	if(!isInMap(endFrame.x, endFrame.y))
	{
		return true;
	}

	bool visionClear = true;
	Vector2d current = start;
	// Vector y distancia entre estos
	Vector2d distanceVector = end - start;
	Vector2d direction = distanceVector;
	direction.normalize();

	// Vector alrededor de la posicion actual (en x)
	Vector2d left, right, up, down; 
	left = right = up = down = current;
	left.x -= radius / 2;
	right.x += radius / 2;
	up.y -= radius / 2;
	down.y += radius / 2;

	float distance = distanceVector.getLength();

	while(distance > 1 && visionClear)
	{
		// Posicion actual y lados estan libres
		if ( isEmpty(current) && isEmpty(left) && isEmpty(right) && isEmpty(up) && isEmpty(down) )
		{ // Seguimos avanzando por los vectores
			current += direction * 2;
			left = right = up = down = current;
			left.x -= radius / 2;
			right.x += radius / 2;
			up.y -= radius / 2;
			down.y += radius / 2;
			// Actualizamos la distancia
			distanceVector = end - current;
			distance = distanceVector.getLength();
		}
		else // Encuentra obstaculo
		{
			visionClear = false;
		}
	}

	return visionClear;
}


Vector2d MapManager::getFlowFieldForce(Vector2d position)
{
	Vector2d framePoisition = getFrame(position);

	Vector2d flowFieldForce(0,0);

	if(isInMap(framePoisition.x, framePoisition.y))
	{
		flowFieldForce = collisionMap.map[framePoisition.x][framePoisition.y].flow;
	}

	return flowFieldForce;
}

Vector2d MapManager::getWallEjectionForce(Vector2d position)
{
	Vector2d framePoisition = getFrame(position);
	framePoisition = Vector2d(framePoisition.y, framePoisition.x);

	Vector2d wallEjection(0,0);

	if(isInMap(framePoisition.x, framePoisition.y))
	{
		wallEjection = collisionMap.map[framePoisition.x][framePoisition.y].wallEjection;
	}

	return wallEjection;
}

