#pragma once

#include "GUIElement.h"
#include "ResourceManager.h"
#include "ResourceTexture.h"

#include <SFML/Graphics.hpp>

class Vector2d;

class GUISprite : public GUIElement
{
public:
	GUISprite();
	GUISprite(Vector2d position, const char* spriteFilename = "../assets/images/sprite_default.jpg", int id = -1);
	virtual~GUISprite(void);
	
	void setPosition(float x, float y)
	{
		position = Vector2d(x, y);
		sprite.setPosition(x, y);
	}

	void draw(sf::RenderWindow* window);

	void setTexture(const char* fileName)
	{
		texture = resourceManager->getTexture(fileName);
		// true para que se recalcule el rectangulo
		sprite.setTexture( *texture->getSFMLTexture(), true );
		// Actulizamos el size con el el nuevo sprite
		size = Vector2d(sprite.getLocalBounds().width, sprite.getLocalBounds().height);
	}

	sf::FloatRect getLocalBounds()
	{
		return sprite.getLocalBounds();
	}

	void setColor( int color ) 
	{
		sprite.setColor(getSFMLColor(color));
	}

	void setScale(float width, float height)
	{
		// El sf::Sprite usa un factor de escalado, nuevo tam entre actual
		Vector2d factor(width / this->size.x, height / this->size.y);
		sprite.setScale(factor.x, factor.y);
	}

	// Establece el rectangulo que se va a dibujar
	void setMaxSize( Vector2d size )
	{
		sprite.setTextureRect( sf::IntRect(0, 0, size.x, size.y) );
	}

	sf::Color getSFMLColor(int color)
	{
		return colorList[color];
	}

	void initColorList()
	{
		colorList.push_back(sf::Color::Red);
		colorList.push_back(sf::Color::Green);
		colorList.push_back(sf::Color::Blue);
		colorList.push_back(sf::Color::Yellow);
		colorList.push_back(sf::Color::White);
		colorList.push_back(sf::Color(50, 50, 50, 255)); //Grey
		colorList.push_back(sf::Color(139, 69, 19, 255)); //Brown
		colorList.push_back(sf::Color::Black);
	}

private:
	ResourceTexture* texture;
	// Sprite a dibujar que esta enlazado con una textura/imagen
	sf::Sprite sprite;

	// Para cargar y modificar el ResourceTexture
	ResourceManager* resourceManager;
};

