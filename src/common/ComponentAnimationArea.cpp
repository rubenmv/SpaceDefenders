#include "ComponentAnimationArea.h"
#include "GameObject.h"
#include "GameManager.h"
#include "GameObjectManager.h"

ComponentAnimationArea::ComponentAnimationArea(int type, Vector2d position)
{
	this->position = position;
	this->type = type;
	radius = 1;
	radiusMax = 15;
	time = 0;
}


ComponentAnimationArea::~ComponentAnimationArea(void)
{

}

void ComponentAnimationArea::update()
{
	if(time == 5)
	{
		int x, y, r2;
		r2 = radius * radius;
		for (x = -radius; x <= radius; x++)
		{
            y = (int) (Math::sqrt(r2 - x*x) + 0.5);
			if(parent->getTag() != GameObject::REMOTE)
			{
				if(type == 0)
				{
					GameManager::getInstance()->getGameObjectManager()->createFire()->position = Vector2d(position.x + x,position.y + y);
					GameManager::getInstance()->getGameObjectManager()->createFire()->position = Vector2d(position.x + x,position.y - y);
				}
				else
				{
					GameManager::getInstance()->getGameObjectManager()->createSpark()->position = Vector2d(position.x + x,position.y + y);
					GameManager::getInstance()->getGameObjectManager()->createSpark()->position = Vector2d(position.x + x,position.y - y);
				}
			}
			else
			{
				if(type == 0)
				{
					GameManager::getInstance()->getGameObjectManager()->createFire()->position = Vector2d(parent->position.x + x,parent->position.y + y);
					GameManager::getInstance()->getGameObjectManager()->createFire()->position = Vector2d(parent->position.x + x,parent->position.y - y);
				}
				else
				{
					GameManager::getInstance()->getGameObjectManager()->createSpark()->position = Vector2d(parent->position.x + x,parent->position.y + y);
					GameManager::getInstance()->getGameObjectManager()->createSpark()->position = Vector2d(parent->position.x + x,parent->position.y - y);
				}
			}
		}

		radius+=5;
	}
	else
	{
		time++;
	}

	if(radius>16)
	{
		parent->kill();
	}
}

void ComponentAnimationArea:: onMessage(const Message& message)
{

}