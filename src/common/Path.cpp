#include "Path.h"
#include "GraphicsEngine.h"
#include "MapManager.h"
#include "DebugTools.h"


Path::Path(Vector2d start, Vector2d end)
{
	this->start = start;
	this->end = end;
	this->numberOfEnemies = 0;
	this->numberOfStructs = 0;
	preference = false;
	
}


Path::~Path(void)
{
}
 

void Path::update()
{
	
	//Dibujamos linea
	GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	if(graphicsEngine != NULL)
	{
		if(GameManager::getInstance()->getDebugTools()->isShowingDebug())
		{				
			graphicsEngine->drawDebugLine(start.asVector3d(), end.asVector3d());

		}
	}

}

void Path::addEnemy()
{

	numberOfEnemies++;
}

void Path::deleteEnemy()
{
	numberOfEnemies--;
}

void Path::addStruct()
{
	numberOfStructs++;
}

void Path::deleteStruct()
{
	numberOfStructs--;
}


