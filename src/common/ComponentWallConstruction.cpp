#include "ComponentWallConstruction.h"
#include "NodeMesh.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "GameObject.h"
#include "EventManager.h"
#include "GameObjectManager.h"
#include "CollisionManager.h"
#include "NodeText.h"

ComponentWallConstruction::ComponentWallConstruction(const char* meshFilename, unsigned int price)
{
	graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	eventManager = GameManager::getInstance()->getEventManager();
	aiAux = GameManager::getInstance()->getMapManager();
	begin = graphicsEngine->addNodeMesh(meshFilename);
	end = graphicsEngine->addNodeMesh(meshFilename);
	end->setPosition(Vector2d(5000,5000));
	wall = graphicsEngine->addNodeMesh("../assets/models/inventory/walls/energy_wall/energy.obj");
	wall->setPosition(Vector2d(5000,5000));
	building = false;
	canBuild = true;
	//this->price = price;
	this->price = 1;
	priceToShow = graphicsEngine->addNodeText( );
	priceToShow->setText(L"");
}


ComponentWallConstruction::~ComponentWallConstruction(void)
{
	delete wall;
	wall = NULL;

	for(size_t i = 0;i<obstacles.size();i++)
	{
		delete obstacles.at(i);
		obstacles.at(i) = NULL;
	}

	delete begin;
	begin = NULL;

	delete end;
	end = NULL;

	obstacles.clear();

	delete priceToShow;
	priceToShow = NULL;

	graphicsEngine = NULL;
	eventManager = NULL;
	aiAux = NULL;
}

void ComponentWallConstruction::viewing()
{
	parent->position = graphicsEngine->getMousePositionOnGround();
	Vector2d pos = aiAux->getFrame(parent->position);
	begin->setPosition(aiAux->getFrameCenter(pos.y,pos.x));

	if(obstacles.empty())
	{
		canBuild = true;
	}
	else
	{
		canBuild = false;
	}

	if(aiAux->checkMap(aiAux->getFrame(parent->position).y,aiAux->getFrame(parent->position).x) == 'W' || parent->position.getSqrDistanceFrom(Vector2d(900,0)) < 122500)
	{
		canBuild = false;
	}

	if(canBuild)
	{
		begin->setColor(2);
	}
	else
	{
		begin->setColor(0);
	}

	if(GameManager::getInstance()->getEventManager()->mouseState.leftButtonDown)
	{
		if(aiAux->checkMap(pos.y,pos.x) != 'W')
		{
			building = true;
			firstPosition = aiAux->getFrameCenter(pos.y,pos.x);
		}
	}
}

void ComponentWallConstruction::buildingPhase()
{
	parent->position = graphicsEngine->getMousePositionOnGround();
	Vector2d pos = aiAux->getFrame(parent->position);
	lastPosition = aiAux->getFrameCenter(pos.y,pos.x);
	bresenham();

	begin->setRotation((lastPosition-firstPosition).getAngle());
	end->setPosition(lastPosition);
	end->setRotation((firstPosition-lastPosition).getAngle());

	if(aiAux->checkMap(aiAux->getFrame(lastPosition).y,aiAux->getFrame(lastPosition).x) == 'W' || lastPosition.getSqrDistanceFrom(Vector2d(900,0)) < 122500)
	{
		canBuild = false;
	}
	else
	{
		canBuild = true;
	}
	float distance = firstPosition.getDistanceFrom(lastPosition);
	Vector2d center = lastPosition+((firstPosition-lastPosition).normalize() * distance/2);
		
	float scale = (distance-10)/15;
	if(scale>0)
	{
		wall->setPosition(center);
		wall->setScale(Vector3d(scale,1,1));
		wall->setRotation((lastPosition-firstPosition).getAngle());
	}
	else
	{
		wall->setPosition(Vector2d(5000,5000));
	}

	if(firstPosition == lastPosition)
	{
		canBuild = false;
	}

	if(canBuild)
	{
		Vector2d direction = firstPosition - lastPosition;
		direction.normalize();
		direction *= 5;
		Vector2d downright = (firstPosition+direction*4/5*-1).rotateBy(90,firstPosition+direction);
		Vector2d downleft = (firstPosition+direction*4/5*-1).rotateBy(-90,firstPosition+direction);
		Vector2d upright = (lastPosition+direction*4/5).rotateBy(-90,lastPosition-direction);
		Vector2d upleft = (lastPosition+direction*4/5).rotateBy(90,lastPosition-direction);
		/*if(!GameManager::getInstance()->isServer())
		{
			GameManager::getInstance()->getGraphicsEngine()->drawDebugLine(upright.asVector3d(),downright.asVector3d());
			GameManager::getInstance()->getGraphicsEngine()->drawDebugLine(downright.asVector3d(),downleft.asVector3d());
			GameManager::getInstance()->getGraphicsEngine()->drawDebugLine(downleft.asVector3d(),upleft.asVector3d());
			GameManager::getInstance()->getGraphicsEngine()->drawDebugLine(upleft.asVector3d(),upright.asVector3d());
		}*/
		std::vector<GameObject*> list = GameManager::getInstance()->getCollisionManager()->getGameObjectBetween(upright, upleft, downright, downleft);
		if(list.size() > 0)
		{
			canBuild = false;
		}
	}

	if(canBuild)
	{
		begin->setColor(2);
		end->setColor(2);
	}
	else
	{
		begin->setColor(0);
		end->setColor(0);
	}
	Vector2d textPosition = Vector2d(30,-30) + lastPosition;
	priceToShow->setText((std::to_wstring(price*distance)).c_str());
	priceToShow->setPosition(textPosition);

	if(GameManager::getInstance()->getEventManager()->mouseState.leftButtonDown)
	{
		if(canBuild)
		{
			Message message;
			message.type = Message::TRY_BUY;
			message.value = price*distance;
			message.gameObject = parent;
			GameManager::getInstance()->getGameObjectManager()->getMainPlayer()->broadcastMessage(message);
		}	
	}
}

void ComponentWallConstruction::update()
{
	if(eventManager->isKeyDown(KEY::KEY_KEY_1) || eventManager->isKeyDown(KEY::KEY_KEY_2) || 
		eventManager->isKeyDown(KEY::KEY_KEY_3) || eventManager->isKeyDown(KEY::KEY_KEY_4) || eventManager->mouseState.rightButtonDown)
	{
		parent->kill();
	}

	if(building)
	{
		buildingPhase();
	}
	else
	{
		viewing();
	}
}

void ComponentWallConstruction::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::SOLD:
		create();
		break;
	case Message::NOT_SOLD:
		//destroy();
		break;
	case Message::NEW_VISION:
		obstacles.push_back(message.gameObject);
		break;
	case Message::LOST_VISION:
		std::vector<GameObject*>::iterator it;
		if(!obstacles.empty())
		{
			for (it=obstacles.begin(); it!=obstacles.end(); ++it)
			{	
				if( *it == message.gameObject)
				{
					obstacles.erase(it);
					break;
				}
			}
		}
		break;
	}
}

void ComponentWallConstruction::create()
{
	if(GameManager::getInstance()->getNetworkEngine()->isStarted())
	{
		GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_BUILDING_WALL,firstPosition,lastPosition);
	}
	else
	{
		GameObject* start = GameManager::getInstance()->getGameObjectManager()->createBuildingWall(firstPosition);
		start->rotation = (lastPosition-firstPosition).getAngle();
		GameObject* end=NULL;
		end = GameManager::getInstance()->getGameObjectManager()->createBuildingWall(lastPosition);
		end->rotation = (firstPosition-lastPosition).getAngle();
		GameManager::getInstance()->getGameObjectManager()->createBuildingEnergyWall(start,end);
	}

	destroy();
}

void ComponentWallConstruction::destroy()
{
	delete wall;
	wall = NULL;

	for(size_t i = 0;i<obstacles.size();i++)
	{
		delete obstacles.at(i);
		obstacles.at(i) = NULL;
	}

	delete begin;
	begin = NULL;

	delete end;
	end = NULL;

	obstacles.clear();
	parent->kill();
}

void ComponentWallConstruction::bresenham()
{
	//public void Bresenham(Graphics g,int x0, int y0, int x1, int y1) { 
	int x0 = aiAux->getFrame(firstPosition).y;
	int y0 = aiAux->getFrame(firstPosition).x;
	int x1 = aiAux->getFrame(lastPosition).y;
	int y1 = aiAux->getFrame(lastPosition).x;

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

	bool isWall = false;
	Vector2d auxLastPosition = lastPosition;
	/* se cicla hasta llegar al extremo de la línea */
	if(dx>dy)
	{
		p = 2*dy - dx;
		incE = 2*dy;
		incNE = 2*(dy-dx);
		while (x != x1)
		{
			if(aiAux->checkMap(x,y) != 'W')
			{
				lastPosition = aiAux->getFrameCenter(x,y);
				x = x + stepx;
				if(aiAux->checkMap(x,y) != 'W')
				{
					if (p < 0)
					{
						p = p + incE;
					}
					else 
					{
						lastPosition = aiAux->getFrameCenter(x,y);
						y = y + stepy;
						p = p + incNE;
					}
				}
				else
				{
					isWall = true;
					x = x1;
				}
			}
			else
			{
				isWall = true;
				x = x1;
			}
			if(lastPosition.getDistanceFrom(firstPosition) > 120)
			{
				isWall = true;
				x = x1;
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
			if(aiAux->checkMap(x,y) != 'W')
			{
				lastPosition = aiAux->getFrameCenter(x,y);
				y = y + stepy;
				if(aiAux->checkMap(x,y) != 'W')
				{
					if (p < 0)
					{
						p = p + incE;
					}
					else 
					{
						lastPosition = aiAux->getFrameCenter(x,y);
						x = x + stepx;
						p = p + incNE;
					}
				}
				else
				{
					isWall = true;
					y = y1;
				}
			}
			else
			{
				isWall =true;
				y = y1;
			}
			if(lastPosition.getDistanceFrom(firstPosition) > 120)
			{
				isWall = true;
				y = y1;
			}
		}
	}
/*	GameObject* start = GameManager::getInstance()->getGameObjectManager()->createBuildingWall(firstPosition);
	start->rotation = (lastPosition-firstPosition).getAngle();
	GameObject* end=NULL;
	if(isWall)
	{
		end = GameManager::getInstance()->getGameObjectManager()->createBuildingWall(lastPosition);
		end->rotation = (firstPosition-lastPosition).getAngle();
		GameManager::getInstance()->getGameObjectManager()->createBuildingEnergyWall(start,end);
	}
	else
	{
		end = GameManager::getInstance()->getGameObjectManager()->createBuildingWall(auxLastPosition);
		end->rotation = (firstPosition-auxLastPosition).getAngle();
		GameManager::getInstance()->getGameObjectManager()->createBuildingEnergyWall(start,end);
	}
*/
	if(!isWall)
	{
		lastPosition = auxLastPosition;
	}
}