#include "GUIText.h"
#include "Clock.h"

const char* GUIText::DEFAULT_FONT = "../assets/fonts/liberation_mono_regular.ttf";

GUIText::GUIText(int id/*= -1*/, const char* fontFilename)
{
	state = State::NORMAL;
	type = Type::TEXT;
	this->id = -1;
	this->visible = true;
	this->enabled = true;

	this->setFont(fontFilename);
	text.setCharacterSize(12);
	text.setColor(sf::Color::White);
	text.setPosition(this->position.x, this->position.y);
	setString(L"");
}

GUIText::GUIText( Vector2d position, int id /*= -1*/, const char* fontFilename /*= "../assets/fonts/sansation.ttf"*/ )
{
	state = State::NORMAL;
	type = Type::TEXT;
	this->id = id;
	this->position = position;
	this->visible = true;
	this->enabled = true;

	this->setFont(fontFilename);
	text.setCharacterSize(12);
	text.setColor(sf::Color::White);
	text.setPosition(position.x, position.y);
	setString(L"");
	
}


GUIText::GUIText( Vector2d position, const wchar_t* string, int id /*= -1*/, const char* fontFilename /*= "../assets/fonts/sansation.ttf"*/ )
{
	
	state = State::NORMAL;
	type = Type::TEXT;
	this->position = position;
	this->id = id;
	this->visible = true;
	
	
	this->setFont(fontFilename);
	text.setCharacterSize(12);
	text.setColor(sf::Color::White);
	text.setString(string);
	text.setPosition(position.x, position.y);
}

GUIText::~GUIText(void)
{
}

void GUIText::draw( sf::RenderWindow* window )
{
	if (visible)
	{
		window->draw(text);
	}
}