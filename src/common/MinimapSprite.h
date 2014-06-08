#pragma once

#include "Vector2d.h"
#include "GameManager.h"
#include "GraphicsEngine.h"

class MinimapSprite
{
public:
	MinimapSprite(const char* spriteFilename,Vector2d position, Vector2d size);
	~MinimapSprite(void);
	
	void setPosition(Vector2d position)
	{
		this->position = position;
		GameManager::getInstance()->getGraphicsEngine()->setPositionGUISprite(id, position,size);
	}

	void setColor(int color)
	{
		this->color = color;
		GameManager::getInstance()->getGraphicsEngine()->setColorGUISprite(id, color);
	}
	//Se le pasa el tamaño que queremos que tenga, si le pasas 4x4 sera 4x4
	void setScale(Vector2d scale)
	{
		if(scale.x <= 0 || scale.y <=0)
		{
			GameManager::getInstance()->getGraphicsEngine()->setVisibleGUISprite(id, false);
			
		}else
		{
			size = scale;		
			GameManager::getInstance()->getGraphicsEngine()->setScaleGUISprite(id, size);
			GameManager::getInstance()->getGraphicsEngine()->setVisibleGUISprite(id, true);
		}
		
	}
	
	Vector2d getSize()
	{

		return size;
	}
	int getId()
	{
		return id;
	}

private:

	int id;
	Vector2d position;
	Vector2d size;
	int color;
};

