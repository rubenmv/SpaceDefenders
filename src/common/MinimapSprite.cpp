#include "MinimapSprite.h"
#include "GameManager.h"
#include "GraphicsEngine.h"

MinimapSprite::MinimapSprite(const char* spriteFilename,Vector2d position, Vector2d size)
{
	this->position = position;
	this->size = size;
	id = GameManager::getInstance()->getGraphicsEngine()->addGUISprite(spriteFilename,position,size);
}


MinimapSprite::~MinimapSprite(void)
{
	 GameManager::getInstance()->getGraphicsEngine()->removeGUISprite(id);
}


