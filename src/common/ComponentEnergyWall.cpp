#include "ComponentEnergyWall.h"
#include "GameManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "NodeMesh.h"
#include "GraphicsEngine.h"
#include "MapManager.h"
ComponentEnergyWall::ComponentEnergyWall(Vector2d start, Vector2d end)
{
	wallBegin = GameManager::getInstance()->getGameObjectManager()->createWall();
	wallEnd = GameManager::getInstance()->getGameObjectManager()->createWall();

	wallBegin->position = start;
	wallBegin->rotation = (end-start).getAngle();
	wallEnd->position = end;
	wallEnd->rotation = (start-end).getAngle();

	float distance = start.getDistanceFrom(end);
	Vector2d center = end+((start-end).normalize() * distance/2);
	float scale = (distance-10)/15;
	if(scale>0)
	{
		if(!GameManager::getInstance()->isServer())
		{
			energy = GameManager::getInstance()->getGraphicsEngine()->addNodeMesh("../assets/models/inventory/walls/energy_wall/energy.obj");
			energy->setPosition(center);
			energy->setScale(Vector3d(scale,1,1));
			energy->setRotation((start-end).getAngle());
		}
	}
	else
	{
		energy = NULL;
	}

	energyActivated = true;
	bresenham();
}


ComponentEnergyWall::~ComponentEnergyWall(void)
{
	if(wallBegin != NULL)
	{
		wallBegin->kill();
	}
	if(wallEnd != NULL)
	{
		wallEnd->kill();
	}
	if(wallBegin != NULL)
	{
		if(energy != NULL)
		{
			removeBresenham();
		}
		else
		{
			MapManager* mapManager = GameManager::getInstance()->getMapManager();
			mapManager->setEmpty(mapManager->getFrame(wallBegin->position).y,mapManager->getFrame(wallBegin->position).x);
		}
		wallBegin = NULL;
	}
	if(wallEnd != NULL)
	{
		if(energyActivated)
		{
			removeBresenham();
		}
		else
		{
			MapManager* mapManager = GameManager::getInstance()->getMapManager();
			mapManager->setEmpty(mapManager->getFrame(wallEnd->position).y,mapManager->getFrame(wallEnd->position).x);
		}
		wallEnd = NULL;
	}

	parent->kill();
}

void ComponentEnergyWall::update()
{
	if(wallBegin != NULL && wallBegin->isDead())
	{
		if(energyActivated)
		{
			removeBresenham();
		}
		else
		{
			MapManager* mapManager = GameManager::getInstance()->getMapManager();
			mapManager->setEmpty(mapManager->getFrame(wallBegin->position).y,mapManager->getFrame(wallBegin->position).x);
		}
		wallBegin = NULL;
	}
	if(wallEnd != NULL && wallEnd->isDead())
	{
		if(energyActivated)
		{
			removeBresenham();
		}
		else
		{
			MapManager* mapManager = GameManager::getInstance()->getMapManager();
			mapManager->setEmpty(mapManager->getFrame(wallEnd->position).y,mapManager->getFrame(wallEnd->position).x);
		}
		wallEnd = NULL;
	}

	if(wallEnd == NULL && wallBegin == NULL)
	{
		parent->kill();
	}
}

void ComponentEnergyWall::onMessage(const Message& message)
{

}

void ComponentEnergyWall::bresenham()
{
	MapManager* aiAux = GameManager::getInstance()->getMapManager();
		//public void Bresenham(Graphics g,int x0, int y0, int x1, int y1) { 
	int x0 = aiAux->getFrame(wallBegin->position).y;
	int y0 = aiAux->getFrame(wallBegin->position).x;
	int x1 = aiAux->getFrame(wallEnd->position).y;
	int y1 = aiAux->getFrame(wallEnd->position).x;

	int x, y, dx, dy, p, incE, incNE, stepx, stepy;
	dx = (x1 - x0);
	dy = (y1 - y0);
 
	/* determinar que punto usar para empezar, cual para terminar */
	if (dy < 0) 
	{ 
		dy = -dy; 
		stepy = -1; 
	} 
	else 
	{
		stepy = 1;
	}
 
	if (dx < 0) 
	{  
		dx = -dx;  
		stepx = -1; 
	} 
	else 
	{
		stepx = 1;
	}
 
	x = x0;
	y = y0;

	/* se cicla hasta llegar al extremo de la línea */
	if(dx>dy)
	{
		p = 2*dy - dx;
		incE = 2*dy;
		incNE = 2*(dy-dx);
		while (x != x1)
		{
			aiAux->setWall(x,y);
			//GameManager::getInstance()->getGameObjectManager()->createTurret()->position = aiAux->getFrameCenter(x,y);
			x = x + stepx;

			if (p < 0)
			{
				p = p + incE;
			}
			else 
			{
				aiAux->setWall(x,y);
				//GameManager::getInstance()->getGameObjectManager()->createTurret()->position = aiAux->getFrameCenter(x,y);
				y = y + stepy;
				p = p + incNE;
			}
		}
	}
	else
	{
		p = 2*dx - dy;
		incE = 2*dx;
		incNE = 2*(dx-dy);
		while (y != y1)
		{
			aiAux->setWall(x,y);
			//GameManager::getInstance()->getGameObjectManager()->createTurret()->position = aiAux->getFrameCenter(x,y);
			y = y + stepy;
			if (p < 0)
			{
				p = p + incE;
			}
			else 
			{
				aiAux->setWall(x,y);
				//GameManager::getInstance()->getGameObjectManager()->createTurret()->position = aiAux->getFrameCenter(x,y);
				x = x + stepx;
				p = p + incNE;
			}
		}
	}

	aiAux->setWall(x,y);	
	//GameManager::getInstance()->getGameObjectManager()->createTurret()->position = aiAux->getFrameCenter(x,y);
}

void ComponentEnergyWall::removeBresenham()
{
	MapManager* aiAux = GameManager::getInstance()->getMapManager();
	int x0;
	int y0;
	int x1;
	int y1;
	if(wallEnd->isDead())
	{
		x0 = aiAux->getFrame(wallBegin->position).y;
		y0 = aiAux->getFrame(wallBegin->position).x;
		x1 = aiAux->getFrame(wallEnd->position).y;
		y1 = aiAux->getFrame(wallEnd->position).x;
	}
	else
	{
		x0 = aiAux->getFrame(wallEnd->position).y;
		y0 = aiAux->getFrame(wallEnd->position).x;
		x1 = aiAux->getFrame(wallBegin->position).y;
		y1 = aiAux->getFrame(wallBegin->position).x;
	}

	int x, y, dx, dy, p, incE, incNE, stepx, stepy;
	dx = (x1 - x0);
	dy = (y1 - y0);
 
	/* determinar que punto usar para empezar, cual para terminar */
	if (dy < 0) 
	{ 
		dy = -dy; 
		stepy = -1; 
	} 
	else 
	{
		stepy = 1;
	}
 
	if (dx < 0) 
	{  
		dx = -dx;  
		stepx = -1; 
	} 
	else 
	{
		stepx = 1;
	}
 
	x = x0;
	y = y0;

	//aiAux->calculateLessForce(x,y);
	/* se cicla hasta llegar al extremo de la línea */
	if(dx>dy)
	{
		p = 2*dy - dx;
		incE = 2*dy;
		incNE = 2*(dy-dx);
		while (x != x1)
		{
			x = x + stepx;
			if (p < 0)
			{
				p = p + incE;
			}
			else 
			{
				aiAux->setEmpty(x,y);
				y = y + stepy;
				p = p + incNE;
			}
			aiAux->setEmpty(x,y);
		}
	}
	else
	{
		p = 2*dx - dy;
		incE = 2*dx;
		incNE = 2*(dx-dy);
		while (y != y1)
		{
			y = y + stepy;
			if (p < 0)
			{
				p = p + incE;
			}
			else 
			{
				aiAux->setEmpty(x,y);
				x = x + stepx;
				p = p + incNE;
			}
			aiAux->setEmpty(x,y);
		}
	}

	delete energy;
	energy = NULL;

	energyActivated = false;
}