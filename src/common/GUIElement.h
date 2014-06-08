#pragma once

#include "Vector2d.h"
#include <SFML/Graphics.hpp>

// Clase padre para todos los elementos del UI
// Contiene propiedades comunes como posicion y tamanyo
class GUIElement
{
public:
	enum State
	{
		NORMAL,	// Estado por defecto
		HOVERED,	// El raton pasa por encima
		ACTIVE,	// El elemento ha sido activado (clic)
		FOCUSED	// El elemento esta seleccionado (tiene el foco, cambia su imagen)
	} state;


	enum Color{
		RED,
		GREEN,
		BLUE,
		YELLOW,
		WHITE,
		GREY,
		BROWN,
		BLACK
	};

	enum Type
	{
		SPRITE,
		TEXT,
		BUTTON,
		CHECKBOX,
		TABLE
	} type;

	GUIElement() {};
	virtual ~GUIElement() {};

	virtual void setId(int id)
	{
		this->id = id;
	}

	virtual int getId()
	{
		return this->id;
	}
 
	// Delega el dibujado a cada elemento (pueden contener otros elementos)
	virtual void draw(sf::RenderWindow* window) = 0;
 
	// Cada elemento, ademas de la posicion suya, cambia la de otros
	// elementos que contenga (sf::sprite, sf::text, etc.)
	virtual void setPosition(float x, float y) = 0;

	virtual Vector2d getPosition()
	{
		return position;
	}

	// Size del contenedor del elemento, sera la zona donde se pueda hacer clic
	// no cambia el tamanyo del sprite, texto, etc.
	virtual void setSize(float width, float height)
	{
		size = Vector2d(width, height);
	}

	virtual Vector2d getSize()
	{
		return size;
	}

	virtual void setVisible( bool visible ) 
	{
		this->visible = visible;
	}

	virtual bool getVisible()
	{
		return visible;
	}

	virtual void setEnabled( bool enabled ) 
	{
		this->enabled = enabled;
	}

	virtual bool getEnabled()
	{
		return enabled;
	}

	// Actualiza el sprite segun el estado, entre otras cosas especificas del elemento
	virtual void update() {};
protected:
	// Id que identifica el elemento (para activar eventos)
	int id;
	// Posicion en x,y
	Vector2d position;
	// Tamanyo en width,height
	Vector2d size;

	bool visible;
	bool enabled;

	std::vector<sf::Color> colorList; //lista con unos cuantos colores
};


