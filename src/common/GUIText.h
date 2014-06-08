#pragma once

#include "GUIElement.h"
#include <SFML/Graphics.hpp>
#include "Vector2d.h"

#include <sstream>

class GUIText : public GUIElement
{
public:
	static const char* DEFAULT_FONT;

	GUIText(int id = -1, const char* fontFilename = DEFAULT_FONT);
	GUIText(Vector2d position, int id = -1, const char* fontFilename = DEFAULT_FONT);
	GUIText(Vector2d position, const wchar_t* string, int id = -1, const char* fontFilename = DEFAULT_FONT);
	~GUIText(void);

	void update() {};

	void draw(sf::RenderWindow* window);

	void setPosition(float x, float y)
	{
		position = Vector2d(x, y);
		text.setPosition(x, y);
	}

	void setRotation(float angle)
	{
		text.setRotation(angle);
	}

	void setFontSize(unsigned int size)
	{
		text.setCharacterSize(size);
	}

	void setFont(const char* fileName)
	{
		if ( !font.loadFromFile(fileName) )
		{
			std::cout << "ERROR: No se pudo encontrar la fuente "<< fileName << std::endl;
		}

		text.setFont(font);
	}

	void setString(const wchar_t* string)
	{
		this->text.setString(string);
	}

	const wchar_t* getString()
	{
		
		return this->text.getString().toWideString().c_str();
	}

	void setColor(unsigned int r, unsigned int g, unsigned int b)
	{
		text.setColor(sf::Color(r, g, b));
	}

	void setOrigin(float x, float y)
	{
		text.setOrigin(x, y);
	}

	// Devuelve el rectangulo que envuelve al texto
	sf::FloatRect getLocalBounds()
	{
		return text.getLocalBounds();
	}

private:
	sf::Text text;
	sf::Font font;
};

