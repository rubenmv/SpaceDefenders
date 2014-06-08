#include "GUIManager.h"

#include "GUIButton.h"
#include "GUISprite.h"
#include "GUICheckbox.h"
#include "GUITable.h"

#include "KeyCode.h"

GUIManager::GUIManager()
{
	defaultFont = GUI::DEFAULT_FONT;
	defaultFontSize = GUI::DEFAULT_FONT_SIZE;
	buttonFont = GUI::MENU_BUTTON_FONT;
	buttonFontSize = GUI::MENU_BUTTON_FONT_SIZE;
}

GUIManager::~GUIManager()
{
	clear();
}

// Devuelve true si el event manager debe activar un evento de clic en un elemento del GUI 
bool GUIManager::onEvent(EventManager::Event& event)
{
	// Para comprobar con el raton
	Vector2d elementPosition, elementSize;
	Vector2d mousePosition = event.mousePosition;
	bool xIn, yIn, buttonEvent = false;

	GUIElement* element = NULL;

	// Recorrer la lista de elementos, cambiar el estado y activar evento
	for (size_t i = 0; i < guiElements.size(); i++)
	{
		element = guiElements[i];

		// Los elementos ocultos no realizan ninguna accion
		if (element->getVisible())
		{
			elementPosition = element->getPosition();
			elementSize = element->getSize();

			xIn = (mousePosition.x >= elementPosition.x) && (mousePosition.x <= elementPosition.x + elementSize.x);
			yIn = (mousePosition.y >= elementPosition.y) && (mousePosition.y <= elementPosition.y + elementSize.y);


			switch (element->type)
			{
			case GUIElement::Type::BUTTON:
				if ( xIn && yIn )
				{
					// El raton esta haciendo clic en el boton
					if (event.type == KEY::EMOUSE_INPUT_EVENT::EMIE_LMOUSE_LEFT_UP)
					{
						// Este if evita que se active mas de una vez el mismo evento
						if (element->state != GUIElement::State::ACTIVE)
						{
							element->state = GUIElement::State::ACTIVE;
							// Completamos la info del evento para luego devolverlo
							event.callerId = guiElements[i]->getId();
							event.type = guiElements[i]->state;
							buttonEvent = true;
						}
					}
					else
					{
						// Solo activa hover si antes era NORMAL
						if (guiElements[i]->state != GUIElement::State::HOVERED)
						{
							guiElements[i]->state = GUIElement::State::HOVERED;
							event.callerId = guiElements[i]->getId();
							event.type = guiElements[i]->state;
							buttonEvent = true;
						}
					}
				}
				else
				{
					if (guiElements[i]->state != GUIElement::State::NORMAL)
					{
						guiElements[i]->state = GUIElement::State::NORMAL;
						event.callerId = guiElements[i]->getId();
						event.type = guiElements[i]->state;
						buttonEvent = true;
					}
				}
				
				break;

			case GUIElement::Type::CHECKBOX:
				if ( xIn && yIn )
				{
					// El raton esta haciendo clic en el boton
					if (event.type == KEY::EMOUSE_INPUT_EVENT::EMIE_LMOUSE_LEFT_UP)
					{
						// intercambia estado
						if (element->state == GUIElement::State::ACTIVE)
						{
							element->state = GUIElement::State::NORMAL;
						}
						else
						{
							element->state = GUIElement::State::ACTIVE;
						}
					}
				}
				break;
			case GUIElement::Type::TABLE:
				if ( xIn && yIn )
				{
					// El raton esta haciendo clic dentro de la tabla
					if (event.type == KEY::EMOUSE_INPUT_EVENT::EMIE_LMOUSE_LEFT_UP)
					{
						GUITable* table = (GUITable*)element;
						table->setSelected(event.mousePosition.y);
					}
				}
				break;
			}

			// Actualiza el sprite segun el estado
			guiElements[i]->update();

		}
	}

	return buttonEvent;
}

void GUIManager::clear()
{
	while (!guiElements.empty())
	{
		delete guiElements.back();
		guiElements.pop_back();
	}
	// Esto ya no seria necesario, pero tampoco duele demasiado
	guiElements.clear();
}

// Apila el estado actual de OpenGL y manda al dibujado de los elementos
void GUIManager::draw(sf::RenderWindow* window)
{
	for (size_t i = 0; i < guiElements.size(); i++)
	{
		guiElements[i]->draw(window);
	}
}

// Boton generico
GUIButton* GUIManager::addButton( float left, float top, float width, float height, int id, const wchar_t* string )
{
	GUIButton* button = new GUIButton(Vector2d(left, top), Vector2d(width, height), string, id);

	button->setFont(buttonFont.c_str());
	button->setFontSize(buttonFontSize);

	// Lo agrega a la lista para dibujarlo
	addElement(button);

	return button;
}

// Boton estilo menu
GUIButton* GUIManager::addMenuButton( float left, float top, float width, float height, int id, const wchar_t* string )
{
	// addButton ya registra el boton
	GUIButton* button = addButton( left, top, width, height, id, string );

	button->setImage(GUI::MENU_IMAGE_BUTTON_NORMAL);
	button->setPressedImage(GUI::MENU_IMAGE_BUTTON_HOVER);

	return button;
}

// Checkbox
GUICheckbox* GUIManager::addCheckBox( bool checked, float left, float top, int id, const wchar_t* string )
{
	GUICheckbox* checkbox = new GUICheckbox(checked, Vector2d(left, top), string, id);

	checkbox->setFont(buttonFont.c_str());
	checkbox->setFontSize(GUI::MENU_CHECBOX_FONT_SIZE);

	checkbox->setImage("../assets/images/gui/checkbox.png");
	checkbox->setCheckedImage("../assets/images/gui/checkbox_checked.png");

	// Lo agrega a la lista para dibujarlo
	addElement(checkbox);

	return checkbox;
}

GUITable* GUIManager::addTable( float left, float top, int id )
{
	GUITable* table = new GUITable(left, top, id);

	// Lo agrega a la lista para dibujarlo
	addElement(table);

	return table;
}

GUISprite* GUIManager::addImage( Vector2d position, const char* fileName, int id /*= -1*/ )
{
	GUISprite* table = new GUISprite(position, fileName, id);

	// Lo agrega a la lista para dibujarlo
	addElement(table);

	return table;
}

GUIText* GUIManager::addText( Vector2d position, const wchar_t* string, int id )
{
	GUIText* text = new GUIText(position, string, id);

	// Lo agrega a la lista para dibujarlo
	addElement(text);

	return text;
}

GUIElement* GUIManager::getElementFromId( int id )
{

	for(size_t i = 0; i<guiElements.size();i++)
	{
		if(guiElements[i]->getId() == id)
		{
			return guiElements[i];
		}
	}

	return NULL;
}

void GUIManager::removeChild( GUIElement* child )
{

	for(size_t i = 0; i<guiElements.size();i++)
	{
		if(guiElements[i] == child)
		{
			delete guiElements[i];
			guiElements.erase(guiElements.begin()+i);
			return;
		}
	}
}

void GUIManager::removeChildFromId( int id )
{

	for(size_t i = 0; i<guiElements.size();i++)
	{
		if(guiElements[i]->getId() == id)
		{
			delete guiElements[i];
			guiElements.erase(guiElements.begin()+i);
			return;
		}
	}
}