#include "GUIButton.h"

GUIButton::GUIButton( Vector2d position, Vector2d size, const wchar_t* string, int id /*= -1*/)
{
	state = State::NORMAL;
	type = Type::BUTTON;
	this->id = id;
	this->size = size;
	this->visible = true;
	this->enabled = true;

	this->text = new GUIText(position, string);
	// Se crean los dos sprites individuales
	this->normalSprite = new GUISprite(position);
	this->pressedSprite = new GUISprite(position);
	// Y el actual apunta al normal
	this->sprite = normalSprite;

	// Con setPosition tambien posiciona el teto
	setPosition(position.x, position.y);
}

GUIButton::~GUIButton()
{
	sprite = NULL; // Atencion, este no hay que borrarlo, solo apunta a los otros
	delete normalSprite;
	normalSprite = NULL;
	delete pressedSprite;
	pressedSprite = NULL;
	delete text;
	text = NULL;
}

void GUIButton::update()
{
	switch (state)
	{
	case GUIElement::NORMAL:
		sprite = normalSprite;
		break;
	case GUIElement::HOVERED:
	case GUIElement::ACTIVE:
	case GUIElement::FOCUSED:
		sprite = pressedSprite;
		break;
	}
}

void GUIButton::draw( sf::RenderWindow* window )
{
	if (visible)
	{
		sprite->draw(window);
		text->draw(window);
	}
}
