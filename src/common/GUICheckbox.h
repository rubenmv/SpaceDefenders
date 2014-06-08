#pragma once

#include <SFML/Graphics.hpp>

#include "GUIElement.h"
#include "GUISprite.h"
#include "GUIText.h"

class GUICheckbox : public GUIElement
{
public:
	GUICheckbox( bool checked, Vector2d position, const wchar_t* string, int id = -1 );
	virtual~GUICheckbox();

	// Actualiza el sprite segun el estado
	void update();

	void draw(sf::RenderWindow* window);

	// Actualiza la posicion del sprite a la derecha del texto
	void updateSpritePosition()
	{
		sf::FloatRect textRect = text->getLocalBounds();
		sprite->setPosition(textRect.left + textRect.width, textRect.top);
	}

	// Coloca el texto y el sprite en su lugar dentro de la caja
	// y actualiza el tamanyo de la caja que los envuelve
	void updateElements()
	{
		sf::FloatRect textRect = text->getLocalBounds();
		sf::FloatRect spriteRect = sprite->getLocalBounds();
		float margin = 5;

		// Obtenemos el size de la caja que los envuelve
		this->size.x = textRect.width + spriteRect.width;
		this->size.y = spriteRect.height; // Suponemos que la imagen es un poco mas grande que el texto

		// Origen y del texto en su centro
		text->setOrigin( textRect.left, textRect.top  + textRect.height/2.0f);
		// El texto centrado en la caja
		text->setPosition(position.x, position.y + size.y/2);
		// Los sprites a la derecha del texto
		normalSprite->setPosition(position.x + textRect.width + margin, position.y);
		checkedSprite->setPosition(position.x + textRect.width + margin, position.y);
	}

	void setText(const wchar_t* text)
	{
		this->text->setString(text);
	}

	void setPosition( float x, float y )
	{
		position.x = x;
		position.y = y;
	}

	// Establece la tipografia
	void setFont(const char* fileName)
	{
		text->setFont(fileName);
	}

	void setFontSize(unsigned int size)
	{
		text->setFontSize(size);
	}

	// Imagen del estado normal
	void setImage(const char* fileName)
	{
		normalSprite->setTexture(fileName);
		updateElements();
	}

	// Imagen del estado activo, checked
	void setCheckedImage(const char* fileName)
	{
		checkedSprite->setTexture(fileName);
		updateElements();
	}

	bool isChecked()
	{
		return ( state == GUIElement::State::ACTIVE );
	}

private:
	// Imagen actual(activa) del boton, apuntara a normalSprite o checkedSprite
	GUISprite* sprite;
	// Sprite para el estado normal
	GUISprite* normalSprite;
	// Sprite para el estado activo
	GUISprite* checkedSprite;

	// Texto del boton
	GUIText* text;
};

