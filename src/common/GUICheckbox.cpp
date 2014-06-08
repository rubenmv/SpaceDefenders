#include "GUICheckbox.h"

GUICheckbox::GUICheckbox( bool checked, Vector2d position, const wchar_t* string, int id /*= -1*/)
{
	type = Type::CHECKBOX;
	this->id = id;
	this->visible = true;
	this->enabled = true;
	

	this->text = new GUIText(position, string);
	// Se crean los dos sprites individuales
	this->normalSprite = new GUISprite(position);
	this->checkedSprite = new GUISprite(position);

	if (checked)
	{
		state = State::ACTIVE;
		this->sprite = checkedSprite;
	}
	else
	{
		state = State::NORMAL;
		this->sprite = normalSprite;
	}
	
	// Posicion del contenedor y su contenido
	setPosition(position.x, position.y);
	updateElements();
}

GUICheckbox::~GUICheckbox()
{
	sprite = NULL;
	delete normalSprite;
	normalSprite = NULL;
	delete checkedSprite;
	checkedSprite = NULL;
	delete text;
	text = NULL;
}

void GUICheckbox::update()
{
	switch (state)
	{
	case GUIElement::NORMAL:
	case GUIElement::HOVERED:
	case GUIElement::FOCUSED:
		sprite = normalSprite;
		break;
	case GUIElement::ACTIVE:
		sprite = checkedSprite;
		break;
	}
}

void GUICheckbox::draw( sf::RenderWindow* window )
{
	sprite->draw(window);
	text->draw(window);
}
