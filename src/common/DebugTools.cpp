#include "DebugTools.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "CollisionManager.h"
#include "NodeText.h"
#include "EventManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include <algorithm>
#include <sstream>
#include "MapManager.h"
#include "AIFlocking.h"
#include <string>
#include <iostream>
#include <iomanip>

bool DebugMessage::isExpired() 
{
	return clock.getElapsedTime() > DebugTools::MAX_MESSAGE_TIME;
}

const unsigned int DebugTools::MAX_MESSAGE_TIME = 5000;

DebugTools::DebugTools(void)
{
	graphicsEngine = NULL;
	debugMessageText = NULL;
	showDebug = true;
	updateTime=-1;

	init();
}


DebugTools::~DebugTools(void)
{
	graphicsEngine = NULL;
	eventManager = NULL;
	collisionManager = NULL;
	mapManager = NULL;
	aiFlocking = NULL;
	gameObjectManager = NULL;
	delete debugMessageText;
	delete debugPathMessageText;
	debugMessageFilter = NULL; // Apunta a objeto, se encarga el gameObjectManager

	textTarget.clear();
	textSteps.clear();

	while(!debugMessageList.empty())
	{
		delete debugMessageList.back();
		debugMessageList.pop_back();
	}
	// Limpia los cubos
	removeObstacles();
	cleanCubeMeshList();

}

void DebugTools::init()
{
	GameManager* gameManager = GameManager::getInstance();
	graphicsEngine = gameManager->getGraphicsEngine();
	eventManager = gameManager->getEventManager();
	collisionManager = gameManager->getCollisionManager();
	gameObjectManager = gameManager->getGameObjectManager();

	mapManager = gameManager->getMapManager();
	aiFlocking = gameManager->getAIFlocking();
	debugMessageText = graphicsEngine->addNodeText();
	debugMessageText->setPosition(Vector2d(graphicsEngine->getWindowSize().x - 200, 40));

	debugUpdateTimeText = graphicsEngine->addNodeText();
	debugUpdateTimeText->setPosition(Vector2d(graphicsEngine->getWindowSize().x/2 - 150, 40+65));

	// Debug Pathfinding
	nextStep = true;
	allSteps = true;
	textSteps = "Modo paso a paso: INACTIVO\nPulsa F2 para activar";
	textTarget << "NO HAY OBJETIVO";

	//std::cout<<"\n ----------------init del debug tools ";
	//Añado los rectangulos con los tiempos de debug
	graphicsEngine->addDebugTimeRectangles();
	isDebugRectVisible=true;
	showDebug = false;
}

void DebugTools::cleanup()
{
	
}

void DebugTools::newMessageSend( GameObject* sender, Message message )
{
	if(debugMessageFilter != NULL)
	{
		if(sender != debugMessageFilter) //Si hay un filtro y no coinciden no hace nada
		{
			return;
		}
	}
	for(auto debugMessage : debugMessageList)
	{
		if(debugMessage->type == message.type)
		{
			debugMessage->counter++;
			debugMessage->clock.restart();
			return;
		}
	}

	DebugMessage* newDebugMessage = new DebugMessage();
	newDebugMessage->type = message.type;
	newDebugMessage->counter = 1;
	debugMessageList.push_back(newDebugMessage);
}

void DebugTools::update()
{
	/*updateClockDT.start();

	checkInput();

	if (showDebug)
	{
		updateDebugMessages();
	}
	updateFilter();
	if (isDebugRectVisible) 
	{
		this->drawUpdateTimes(); 
	}
	updateTime=updateClockDT.getElapsedTime();*/
}

bool DebugTools::deleteExpiredMessages(DebugMessage* debugMessage)
{
/*	if(debugMessage->isExpired())
	{
		delete debugMessage;
		return true;
	}
	return false;*/

	return false;
}

void DebugTools::updateDebugMessages()
{
	/*debugMessageList.erase(std::remove_if(debugMessageList.begin(), debugMessageList.end(), deleteExpiredMessages), debugMessageList.end());
	std::wstringstream stream;

	if(debugMessageFilter != NULL && debugMessageFilter->isDead())
	{
		debugMessageFilter = NULL;
	}
	if(debugMessageFilter != NULL)
	{
		stream << debugMessageFilter->getName() << std::endl;
	} else
	{
		stream << "ALL\n";
	}

	for (std::size_t i = 0; i < debugMessageList.size(); i++)
	{
		stream << debugMessageList[i]->counter << "x ";
		stream << debugMessageList[i]->type << std::endl;
	}

	debugMessageText->setText(stream.str().c_str());*/
}

void DebugTools::updateFilter()
{
/*	if(eventManager->mouseState.rightButtonPressed)
	{
		if(!mousePressed) //Solo entra al apretar el boton derecho del raton
		{
			mousePressed = true;
			Vector2d mousePositionOnGround = graphicsEngine->getMousePositionOnGround();
			GameObject* gameObject = collisionManager->getGameObjectIn(mousePositionOnGround);
			setMessageFilter(gameObject);
		}
	} else 
	{
		mousePressed = false;
	}*/
}

void DebugTools::setMessageFilter( GameObject* newFilter )
{
	/*if(debugMessageFilter != newFilter)
	{
		debugMessageFilter = newFilter;

		while(!debugMessageList.empty())
		{
			delete debugMessageList.back();
			debugMessageList.pop_back();
		}
	}*/
}

void DebugTools::checkInput()
{
/*	if(eventManager->isKeyDown(KEY::KEY_F1))
	{
		showDebug = !showDebug;
		debugMessageFilter = NULL;
		if(!showDebug) 
		{
			debugMessageText->setText(L""); 		
		}
	}

	if(eventManager->isKeyDown(KEY::KEY_F3))
	{
		bool rectVisible = !isDebugRectanglesVisible();
		setDebugRectanglesVisible(!isDebugRectanglesVisible());
		debugUpdateTimeText->setText(L"");
		graphicsEngine->setUpdateTimeRectanglesVisible(rectVisible);				
	}

	if(eventManager->isKeyDown(KEY::KEY_F4))
	{
		graphicsEngine->changeDrawArrow();				
	}
	if(eventManager->isKeyDown(KEY::KEY_F5))
	{
		graphicsEngine->changeDrawCube();				
	}*/
}

/* **************************** PATHFINDING **************************** */

//Crea un cubo en la rejilla y modifica la matriz del pathfinding
void DebugTools::createCubeObstacle(Vector2d position)
{
	/*//el tamaño de una celda de la rejilla
	int gridCellSize = mapManager->collisionMap.frameSize;

	//Redondeamos la posicion
	Vector2d discretePosition= Vector2d((int)position.x/gridCellSize, (int)-position.y/gridCellSize); //multiplicamos por -1 la y para hacerla positiva

	//Modificamos la matriz de colisiones
	if (mapManager->isEmpty(discretePosition.x, discretePosition.y))
	{
		//Calculamos la posicion del cubo (centrado en la celda de la rejilla)
		Vector2d positionOnGrid = mapManager->getFrameCenter(discretePosition.x, discretePosition.y);

		mapManager->setWall(discretePosition.x, discretePosition.y);
		cubeObstacleList.push_back(graphicsEngine->addCube(positionOnGrid, Vector2d(0.95, 0.8), DebugColor::GREY, true));
	}*/
}

void DebugTools::removeObstacles()
{
/*	if (!cubeObstacleList.empty())
	{
		for (std::size_t i = 0; i < cubeObstacleList.size(); i++)
		{
			delete cubeObstacleList[i];
		}

		cubeObstacleList.clear();
	}*/
}

// Dibuja cubos segun la lista de nodos
void DebugTools::drawNodeList( std::vector<NodeAS*> list, int color )
{
/*	GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();

	if (!list.empty())
	{
		for (std::size_t i = 0; i < list.size(); i++)
		{
			Vector2d position = Vector2d(list[i]->center.x, list[i]->center.y * -1);
			cubeNodeList.push_back(graphicsEngine->addCube(position, Vector2d(0.4, 0.3), color));
		}
	}*/
}

void DebugTools::cleanCubeMeshList()
{
/*	if(!cubeNodeList.empty())
	{
		for ( std::size_t i = 0; i < cubeNodeList.size(); i++ )
		{
			delete cubeNodeList[i];
		}
		cubeNodeList.clear();
	}*/
}

// Imprime por consola los nodos de una lista
void DebugTools::printList(std::vector<NodeAS*> list)
{
/*	std::cout << std::endl;
	std::cout << "Size: " << list.size() << std::endl;
	for (std::vector<NodeAS*>::iterator it = list.begin(); it != list.end(); ++it)
	{
		std::cout << "\t\t" << (*it)->id << ": ";
		std::cout << "(" << (*it)->center.x << ", " << (*it)->center.y << ")\t";
		std::cout << "G: " << (*it)->g << ", H: " << (*it)->h << ", F: " << (*it)->f << std::endl;
	}
	std::cout << std::endl;*/
}

// Dibuja las lineas del camino
void DebugTools::drawPath( GameObject* gameObject, std::vector<Vector2d> path )
{
/*	GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();

	for( std::size_t i = 0; i < path.size(); i++)
	{
		if(i > 0)
		{
			if(graphicsEngine != NULL)
			{
				Vector2d begin(path[i-1].x, path[i-1].y);
				Vector2d end(path[i].x, path[i].y);
				graphicsEngine->drawDebugLine(begin.asVector3d(), end.asVector3d());
			}
		}
		else
		{
			if(graphicsEngine != NULL)
			{
				Vector2d begin(gameObject->position.x, gameObject->position.y);
				Vector2d end(path[path.size()-1].x, path[path.size()-1].y);
				graphicsEngine->drawDebugLine(begin.asVector3d(), end.asVector3d());
			}
		}
	}*/
}

void DebugTools::updateTextPathfinding()
{
/*	std::wstringstream string;
	string << textSteps.c_str() << "\n\n" << textTarget.str() << "\n\nClic izquierdo para fijar objetivo.\nClic derecho para agregar obstaculo";
	graphicsEngine->setDebugText(string.str().c_str());*/
}

void DebugTools::updateTextWaves()
{
/*	std::wstringstream string;
	string << textSteps.c_str() << "\n\n" << textTarget.str() << "\n\nClic izquierdo para fijar objetivo.\nClic derecho para agregar obstaculo";
	graphicsEngine->setDebugText(string.str().c_str());*/
}

void DebugTools::drawUpdateTimes()
{
/*	std::wstringstream stream;

	if(isDebugRectVisible)
	{

		struct UpdateTimes ut;

		ut.collisionManagerTime = collisionManager->getUpdateTime();
		ut.debugToolsTime = this->updateTime;
		ut.gameManagerTime = GameManager::getInstance()->getUpdateTime();
		ut.gameObjectManagerTime = gameObjectManager->getUpdateTime();
		ut.graphicEngineTime = graphicsEngine->getUpdateTime();
		ut.verticalSyncTime = graphicsEngine->getUpdateVSTime();
		
		//Mostramos el texto por pantalla
		stream << "GOM   CM    GE    DBG   VS    AIF       TOTAL\n";


		addUpdateTimeText(ut.gameObjectManagerTime, stream);
		addUpdateTimeText(ut.collisionManagerTime, stream);
		addUpdateTimeText(ut.graphicEngineTime, stream);
		addUpdateTimeText(ut.debugToolsTime, stream);
		addUpdateTimeText(ut.verticalSyncTime, stream);
		addUpdateTimeText(0, stream);

		stream << "    ";

		addUpdateTimeText(ut.gameManagerTime, stream);
		stream << "\n";

		int gomPerc=(float)ut.gameObjectManagerTime/(float)ut.gameManagerTime*100;
		int cmPerc=(float)ut.collisionManagerTime/(float)ut.gameManagerTime*100;
		int gePerc=(float)ut.graphicEngineTime/(float)ut.gameManagerTime*100;
		int dbgPerc=(float)ut.debugToolsTime/(float)ut.gameManagerTime*100;
		int vsPerc = (float)ut.verticalSyncTime/(float)ut.gameManagerTime*100;
		int aifPerc= 0.0;

		//Mostramos los porcentajes de cada update por pantalla
		addPercentTimeText(gomPerc, stream);
		addPercentTimeText(cmPerc, stream);
		addPercentTimeText(gePerc, stream);
		addPercentTimeText(dbgPerc, stream);
		addPercentTimeText(vsPerc, stream);
		addPercentTimeText(aifPerc, stream);

		debugUpdateTimeText->setText(stream.str().c_str());
		
		graphicsEngine->drawUpdateTimesRectangles(ut); 
	}*/
}

void DebugTools::addUpdateTimeText( int time, std::wstringstream &stream )
{
	/*if(time>=0)
	{
		//obliga a que time ocupe 2 caracteres del estilo, 01, 02, 10, 11...
		stream.fill('0');
		stream.width(2);
		stream << time;
	}
	else
	{
		stream << "--";
	}

	stream << "    ";*/
}

void DebugTools::addPercentTimeText( int percent, std::wstringstream &stream )
{
/*	if(percent>=0)
	{
		//obliga a que time ocupe 2 caracteres del estilo, 01, 02, 10, 11...
		stream.fill('0');
		stream.width(2);
		stream << percent;
		stream << '%';
	}
	else
	{
		stream << "--%";
	}

	stream << "   ";*/
}

bool DebugTools::isDebugRectanglesVisible()
{
	return false;// isDebugRectVisible;
}

void DebugTools::setDebugRectanglesVisible(bool visible)
{
	//isDebugRectVisible=visible;
}


