#include <algorithm>
#include <sstream>
#include "Math.h"
#include "Clock.h"

#include "ComponentPathfinder.h"
#include "NodeAS.h"

#include "GameManager.h"
#include "EventManager.h"
#include "GameObject.h"

#include "GraphicsEngine.h"
#include "MapManager.h"


ComponentPathfinder::ComponentPathfinder(float speed)
{
	this->speed = speed;
	isReached = true; // Comienza parado
	finalTarget = currentTarget = Vector2d();

	gameManager = GameManager::getInstance();
	mapManager = gameManager->getMapManager();
	debugTools = gameManager->getDebugTools();
	targetObject = NULL;

	mapManager->addComponentPathfinder(this);
}

ComponentPathfinder::~ComponentPathfinder(void)
{
	mapManager->removeFromList(this);
	gameManager = NULL;
	mapManager = NULL;
	debugTools = NULL;
}


void ComponentPathfinder::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::NEW_TARGET:
		changeTarget(message.target);
		break;
	}
}

// Nuevo target, nueva busqueda
void ComponentPathfinder::changeTarget(Vector2d newTarget)
{
	// Antes de iniciar una busqueda comprobamos si el objetivo es valido
	if ( mapManager->isEmpty(newTarget))
	{
		currentTarget = newTarget;
		finalTarget = newTarget;
		this->init(); // Inicializa parametros para busqueda
	}
	else
	{
		isReached = true; // Por si ya iba de camino
		debugTools->textTarget.str(L"");
		debugTools->textTarget.clear();
		debugTools->textTarget << "OBJETIVO NO VALIDO";
	}
}



// Limpia las listas y su contenido
void ComponentPathfinder::clearPathLists()
{
	for ( std::size_t i = 0; i < openList.size(); i++ )
	{
		delete openList[i];
	}
	openList.clear();
	for ( std::size_t i = 0; i < openList.size(); i++ )
	{
		delete closedList[i];
	}
	closedList.clear();

	path.clear();
}

// Inicializa los parametros para la busqueda de nuevo camino
void ComponentPathfinder::init()
{
	isReached = false;
	pathFound = false;

	// Limpiamos las listas
	clearPathLists();
	// Inicializamos el debug
	debugTools->cleanCubeMeshList();

	// Creamos los nodos inicio y final
	// Origen					id, parent, posicion
	origin = new NodeAS(0, NULL, Vector2d(parent->position.x, parent->position.y));
	// Destino
	destiny = new NodeAS(0, NULL, Vector2d(finalTarget.x, finalTarget.y));
	// Corregimos las IDs
	fixId(origin);
	fixId(destiny);
	// Corregimos los centros
	fixCenter(origin);
	fixCenter(destiny);

	if(origin->getId() != destiny->getId())
	{
		origin->setH( getHeuristicDistance(origin->center, destiny->center) );
		origin->setF();

		// Nodo original a la lista abierta
		openList.push_back(origin);

		clock.start();
	}
	else // Directo a las coordenadas, las id son las mismas (misma casilla)
	{
		isReached = true;
		pathFound = true;
		currentTarget = finalTarget;
	}

	debugTools->textTarget.str(L"");
	debugTools->textTarget.clear();
	debugTools->textTarget << "BUSCANDO CAMINO";
}

void ComponentPathfinder::update()
{
	if (!isReached)
	{
		if (pathFound) // Sigue moviendose
		{
			if (mapManager->canSeeTarget(parent->position, currentTarget, 1))
			{
				// *** DEBUG, LINEAS PATH ***
				debugTools->drawPath(parent, path); // Se actualiza en cada update
				// **************************

				Vector2d distanceVector = currentTarget - parent->position;
				Vector2d direction = distanceVector;
				direction.normalize();

				// Vector2d te permite calcular las distancias
				float distance = distanceVector.getLength();

				// Entra en el rango del objetivo
				if(distance < 5)
				{
					if(path.empty()) // Objetivo final alcanzado
					{
						isReached = true;
					}
					else // No hemos llegado al objetivo final
					{
						path.pop_back();
						if(!path.empty()) // Puede haber quedado vacio
						{
							smoothPath();
							currentTarget = path.back();
						}
					}
				}
				else // Actualiza posicion
				{
					seek();
					//parent->rotation = direction.getAngle();
					//parent->position += direction * speed * gameManager->getDeltaTime();
				}
			}
			else
			{
				init();
			}
		}
		else // Sigue buscando
		{
			// Debug por pasos
			if ( debugTools->nextStep )
			{
				nextNode(); // Busca siguiente nodo
			}

			// ******** CAMINO ENCONTRADO ********
			if (pathFound)
			{
				debugTools->textTarget.str(L"");
				debugTools->textTarget.clear();
				debugTools->textTarget << "OBJETIVO ENCONTRADO\nSe ha tardado " << clock.getElapsedTime() << "ms";

				// Recoger el camino desde el destino
				destiny->center = finalTarget;
				NodeAS* currentNode = destiny;

				do
				{
					currentNode->normalizeCenter(); // cuarto cuadrante
					path.push_back(currentNode->center);
					currentNode = currentNode->parent;
				} while ( currentNode->getId() != origin->getId() );

				// Suavizado
				smoothPath();
				currentTarget = path.back();
			}

			// Debug por pasos
			if ( !debugTools->allSteps )
			{
				debugTools->nextStep = false;
			}

		}
	}
}

// Calcula y corrige la id de un nodo a partir de su posicion en cuadricula (a partir del center)
void ComponentPathfinder::fixId(NodeAS* node)
{
	Vector2d frame = mapManager->getFrame(node->getCenter());
	// Asignamos la Id y el centro del nodo en esa casilla
	node->setId(mapManager->collisionMap.numFrames * frame.y + frame.x); // totalColumnas x fila (y) + columna (x)
}
// Corrige las coordenadas del centro
void ComponentPathfinder::fixCenter( NodeAS* node )
{
	int frameSize = mapManager->collisionMap.frameSize;
	// Obtenemos la casilla del mapa donde se encuentra el nodo
	Vector2d frame = mapManager->getFrame(node->getCenter());

	// frame.x = col, frame.y = row
	Vector2d center = Vector2d( frameSize * frame.y + (frameSize / 2), frameSize * frame.x + (frameSize / 2) );

	node->setCenter(center);
}


// Comprueba si un nodo esta en la lista (NodeAS)
bool ComponentPathfinder::isInList(NodeAS* node, std::vector<NodeAS*> list)
{
	if(!list.empty())
	{
		for (std::vector<NodeAS*>::iterator it = list.begin(); it != list.end(); ++it)
		{
			if ( (*it)->id == node->id )
				return true;
		}
	}

	return false;
}

// Comprueba si un nodo esta en la lista (Id)
bool ComponentPathfinder::isInList(int idNode, std::vector<NodeAS*> list)
{
	if(!list.empty())
	{
		for (std::vector<NodeAS*>::iterator it = list.begin(); it != list.end(); ++it)
		{
			if ( (*it)->id == idNode )
				return true;
		}
	}

	return false;
}

// Comprueba si un nodo esta en la lista y lo recoge
NodeAS* ComponentPathfinder::getFromList( NodeAS* node, std::vector<NodeAS*> list )
{
	if(!list.empty())
	{
		for (std::vector<NodeAS*>::iterator it = list.begin(); it != list.end(); ++it)
		{
			if ( (*it)->getId() == node->getId() )
			{
				return *it;
			}
		}
	}

	return NULL;
}

void ComponentPathfinder::seek()
{
	//Calculos necesarios del Seek
	Vector2d desired_velocity = (path[path.size()-1] - parent->position);
	desired_velocity.normalize();
	desired_velocity *= 0.8;
	Vector2d steering = desired_velocity - parent->velocity;
	parent->velocity += steering * 0.1;
	//parent->velocity.truncate(maxSpeed);
	//Debug Lines
	gameManager->getGraphicsEngine()->drawDebugLine(parent->position.asVector3d(),(parent->position + parent->velocity*20).asVector3d(),2);

	//gameManager->getGraphicsEngine()->drawDebugLine(parent->position.asVector3d(),(parent->position + steering*150).asVector3d(),3);

	//Modificaciones en la nave
	parent->rotation = parent->velocity.getAngle();
	parent->position += parent->velocity * 1;
}

// Devuelve el nodo con menor coste F
NodeAS* ComponentPathfinder::getMinCostNode(std::vector<NodeAS*> list)
{
	NodeAS* node = NULL;

	if(!list.empty())
	{
		node = list[0];

		for (unsigned int i = 0; i < list.size(); i++)
		{
			if( list[i]->getF() < node->getF() ) // Mejor F
			{
				node = list[i];
			}
		}
	}

	return node;
}

// Distancia hasta destino, no tiene en cuenta obstaculos ni diagonales
float ComponentPathfinder::getManhattanDistance(Vector2d current, Vector2d target)
{
	return (Math::abs(current.x - target.x) + Math::abs(current.y - target.y));
}



// Calcula el peso H teniendo en cuenta diagonales
float ComponentPathfinder::getDiagonalDistance(Vector2d current, Vector2d target)
{
	int h;
	int c1 = 1; // Coste horizontal
	int c2 = 2; // Coste diagonal

	float xDistance = Math::abs(current.x - target.x);
	float yDistance = Math::abs(current.y - target.y);

	if (xDistance > yDistance)
		h = c2 * yDistance + c1 * (xDistance - yDistance);
	else
		h = c2 * xDistance + c1 * (yDistance - xDistance);

	return h;
}

// Fachada para el metodo de obtencion de distancia H
float ComponentPathfinder::getHeuristicDistance(Vector2d current, Vector2d target)
{
	float h = getDiagonalDistance(current, target);
	return h;
}

// Crea un nodo en el lugar indicado
NodeAS* ComponentPathfinder::createNode(int row, int col, NodeAS* parentNode) // target para calcular H
{
	if(mapManager->isEmpty(row, col))
	{
		int numFrames = mapManager->collisionMap.numFrames;
		// Id correspondiente a la casilla
		int id = numFrames * row + col; // totalColumnas x fila + columna

		if(!isInList(id, closedList))
		{
			// Extraemos el id de la casilla correspondiente
			int frameSize = mapManager->collisionMap.frameSize;

			Vector2d center = Vector2d(frameSize * row + (frameSize / 2), frameSize * col + (frameSize / 2));
			NodeAS* node = new NodeAS(id, parentNode, center);
			// G = distancia entre padre y este
			node->setG( getHeuristicDistance(node->getParent()->getCenter(), node->getCenter()) );
			// H = distancia entre este y destino
			node->setH( getHeuristicDistance(node->getCenter(), destiny->getCenter()) );
			node->setF();

			return node;
		}
	}

	return NULL;
}

// Devuelve una lista de nodos adyacentes válidos y no en lista cerrada del mapa de la IA
std::vector<NodeAS*> ComponentPathfinder::getAdjacents(NodeAS* currentNode, NodeAS* destiny)
{
	std::vector<NodeAS*> adjacents;

	int frameSize = mapManager->collisionMap.frameSize;

	// Obtenemos la casilla del mapa donde se encuentra el nodo actual
	int mx = (int)(currentNode->center.x / frameSize);
	int my = (int)(currentNode->center.y / frameSize);

	/* ASI ES MAS BONITO PERO NO SE COMO HACER LA CONDICION EXTRA DE LAS DIAGONALES
	   YA QUE NO QUIERO INCLUIR LAS QUE NO SON CAMINABLES
	int xPositions[3] = {mx, mx - 1, mx + 1};
	int yPositions[3] = {my, my - 1, my + 1};

	NodeAS* node = NULL;

	// Se crean los nodos caminables
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if(j != 0 && i != 0)
			{
				if (mapManager->isFree(i, j))
				{
					node = createNode(xPositions[i], yPositions[j], currentNode);
				}
			}
			else
			{
				node = createNode(xPositions[i], yPositions[j], currentNode);
			}


			if(node != NULL)
			{
				adjacents.push_back(node);
			}
		}
	}
	*/

	int right = mx + 1;
	int left = mx - 1;
	int up = my - 1;
	int down = my + 1;

	// Crear los nodos adyacentes caminable que no esten en la lista cerrada
	adjacents.push_back(createNode(mx, up, currentNode)); // Superior
	adjacents.push_back(createNode(left, my, currentNode)); // Izquierdo
	adjacents.push_back(createNode(mx, down, currentNode)); // Inferior
	adjacents.push_back(createNode(right, my, currentNode)); // Derecho

	// DIAGONALES
	if(mapManager->isEmpty(mx, up)) // Superior
	{
		if (mapManager->isEmpty(right, my)) // y derecho
			adjacents.push_back(createNode(right, up, currentNode)); // Superior-Derecho

		if (mapManager->isEmpty(left, my)) // e izquierdo
			adjacents.push_back(createNode(left, up, currentNode)); // Superior-Izquierdo
	}

	if(mapManager->isEmpty(mx, down)) // Inferior
	{
		if (mapManager->isEmpty(right, my)) // y derecho
			adjacents.push_back(createNode(right, down, currentNode)); // Inferior-Derecho

		if (mapManager->isEmpty(left, my)) // e izquierdo
			adjacents.push_back(createNode(left, down, currentNode)); // Inferior-Izquierdo
	}

	// Eliminamos los nodos NULL
	// Se tratan de los nodos que no se han creado porque no son caminable o ya estaban en la lista cerrada
	if(!adjacents.empty())
	{
		for (std::vector<NodeAS*>::iterator it = adjacents.begin(); it != adjacents.end(); )
		{
			if( (*it) == NULL )
			{
				delete *it;
				it = adjacents.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	return adjacents;
}

void ComponentPathfinder::nextNode()
{
	/*	Pathfinding A*
		A = Punto inicial
		B = Punto final
		G = Coste desde la posicion original a las posibles actuales
		H = Coste aproximado desde A hasta B
		F = G + H, para decidir la mejor casilla
	*/

	std::vector<NodeAS*> adjacents;
	NodeAS* currentNode;

	// Busca en la lista abierta el nodo de menor coste F
	currentNode = openList.back(); // ordenada
	openList.pop_back();

	// Pasar a la lista cerrada
	closedList.push_back(currentNode);

	// Miramos si el nodo actual es el destino (mejor que hacer isInList(closedList))
	if(currentNode->getId() == destiny->getId())
	{
		pathFound = true;
		destiny->parent = currentNode->parent; // Arreglamos el destino con el nuevo parent
	}

	if (!pathFound)
	{
		// Obtener los nodos adyacentes candidatos (caminables)
		adjacents = getAdjacents(currentNode, destiny);

		// Miramos los nodos adyacentes
		for (std::vector<NodeAS*>::iterator newNode = adjacents.begin(); newNode != adjacents.end(); ++newNode)
		{
			NodeAS* oldNode = getFromList( (*newNode), openList );

			if ( oldNode != NULL ) // Esta en la lista abierta
			{
				if ( (*newNode)->getG() < oldNode->getG() )
				{
					oldNode->setParent(currentNode);
					// G de adyacente = G del actual + distancia entre actual (ahora parent) y adyacente
					oldNode->setG( currentNode->g + getHeuristicDistance( currentNode->getCenter(), oldNode->getCenter() ) );
					oldNode->setF();
				}
			}
			else
			{
				// No esta en lista, se agrega
				openList.push_back(*newNode);
			}
		}

		// Mantenemos la lista ordenada
		std::sort( openList.begin(), openList.end(), [](NodeAS* n1, NodeAS* n2){ return n1->f > n2->f; } );
	}

	if(openList.empty())
	{
		debugTools->textTarget.str(L"");
		debugTools->textTarget.clear();
		debugTools->textTarget << "NO SE ENCUENTRA NINGUN CAMINO";
		debugTools->updateTextPathfinding();
		isReached = true; // No hay camino, lo detenemos
	}

	// DEBUG, PINTA CUBOS A PARTIR DE LAS LISTAS
	//debugTools->cleanCubeMeshList(); // Primero limpiamos los cubos actuales
	//debugTools->drawNodeList(openList, DebugColor::YELLOW);
	//debugTools->drawNodeList(closedList, DebugColor::GREEN);
}

// Realiza un suavizado del camino, eliminando puntos innecesarios, se llama cada vez que ve un nuevo objetivo
void ComponentPathfinder::smoothPath()
{
	bool finish = false;

	std::size_t i = 0; //Empieza mirando si puede ir directo al ultimo.

	Vector2d currentPos = parent->position;
	Vector2d nextPos; // Posicion con la que comprobar

	float radius = mapManager->collisionMap.frameSize - 2; // ESTO DEBERIA SER ANCHO DE LA NAVE

	while (!finish && i < path.size())
	{
		nextPos = path[i];

		//if(graphicsEngine->canSeeTarget(currentPos, nextPos) && graphicsEngine->canSeeTarget(o1, d1) && graphicsEngine->canSeeTarget(o2, d2))
		if( mapManager->canSeeTarget(currentPos, nextPos, radius) )
		{
			//Camino libre
			std::size_t pathSize = path.size();

			while (i < pathSize) //Quita todos los sitios que se puede saltar
			{
				path.pop_back();
				i++;
			}
			path.push_back(nextPos);
			finish = true;
		}

		i++;
	}
}
