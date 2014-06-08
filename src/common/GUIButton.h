#pragma once

#include <SFML/Graphics.hpp>

#include "GUIElement.h"
#include "GUISprite.h"
#include "GUIText.h"

class GUIButton : public GUIElement
{
public:
	GUIButton( Vector2d position, Vector2d size, const wchar_t* string, int id = -1 );
	virtual~GUIButton();

	// Pone la imagen del sprite segun el estado
	// y coloca el texto en su lugar
	void update();

	void draw(sf::RenderWindow* window);

	void setText(const wchar_t* text)
	{
		this->text->setString(text);
		updateTextPosition();
	}

	const wchar_t* getText()
	{
		return text->getString();
	}

	void setEnabled( bool enabled ) 
	{
		this->enabled = enabled;
		if ( enabled )
		{
			normalSprite->setColor(4);// Blanco, original
			pressedSprite->setColor(4);
		}
		else
		{
			normalSprite->setColor(5);// Gris
			pressedSprite->setColor(5);
		}
	}

	void setPosition(float x, float y)
	{
		position.x = x;
		position.y = y;

		sprite->setPosition(x, y);
		updateTextPosition();
	}

	// Establece la tipografia
	void setFont(const char* fileName)
	{
		text->setFont(fileName);
		updateTextPosition();
	}

	void setFontSize(unsigned int size)
	{
		text->setFontSize(size);
		updateTextPosition();
	}

	// Imagen del estado normal y tambien pone el de pressed
	void setImage(const char* fileName)
	{
		normalSprite->setTexture(fileName);
		pressedSprite->setTexture(fileName);
	}

	// Imagen del estado activo, pressed
	void setPressedImage(const char* fileName)
	{
		pressedSprite->setTexture(fileName);
	}

private:
	// Imagen actual(activa) del boton, apuntara a normalSprite o pressedSprite
	GUISprite* sprite;
	// Sprite para el estado normal
	GUISprite* normalSprite;
	// Sprite para el estado activo
	GUISprite* pressedSprite;

	// Texto del boton
	GUIText* text;

	// Actualiza la posicion del texto (al centro del boton)
	void updateTextPosition()
	{
		// POSICION DEL TEXTO
		// Origen en el centro del texto
		sf::FloatRect textRect = text->getLocalBounds();
		text->setOrigin( textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
		// Texto al centro del boton
		text->setPosition( position.x + (size.x / 2), position.y + (size.y / 2) );
	}
};

