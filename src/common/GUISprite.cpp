#include "GUISprite.h"

#include "GameManager.h"
#include "ResourceManager.h"
#include "ResourceTexture.h"

GUISprite::GUISprite()
{
	state = State::NORMAL;
	type = Type::SPRITE;
	this->id = -1;
	this->visible = true;
	this->enabled = true;

	// Para los sprites tenemos que la escala por defecto debe ser 1
	//this->size = Vector2d(1, 1);

	initColorList();

	resourceManager = GameManager::getInstance()->getResourceManager();
}

GUISprite::GUISprite( Vector2d position, const char* spriteFilename, int id /*= -1*/ )
{
	state = State::NORMAL;
	type = Type::SPRITE;
	this->id = id;
	this->visible = true;
	this->enabled = true;
	
	//this->size = Vector2d(1, 1);
	this->position = position;

	initColorList();

	resourceManager = GameManager::getInstance()->getResourceManager();

	// Pide el recurso de textura y actualiza el size del elemento a partir de la nueva textura
	this->setTexture(spriteFilename);
	sprite.setPosition(position.x, position.y);
}

GUISprite::~GUISprite(void)
{
	texture = NULL; // El resource manager se encargara de eliminarla
	resourceManager = NULL;
}

void GUISprite::draw( sf::RenderWindow* window )
{
	if(visible)
	{
		window->draw(sprite);
	}
}
