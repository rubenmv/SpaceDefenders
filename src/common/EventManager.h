#pragma once

#include "GameManager.h"

#include "irrlicht/irrlicht.h"
#include "KeyCode.h"
#include "Vector3d.h"
#include "Vector2d.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

class GUIManager;
class GraphicsEngine;
class NetworkEngine;
class GameObjectManager;
class AudioManager;

class EventManager : public irr::IEventReceiver
{
public:
	struct MouseState
	{
		Vector2d position;
		bool leftButtonPressed;
		bool leftButtonDown;
		bool leftButtonUp;
		bool rightButtonPressed;
		bool rightButtonDown;
		bool rightButtonUp;
		bool wheelUp;
		bool wheelDown;
		MouseState() :	leftButtonPressed(false), rightButtonPressed(false), leftButtonDown(false),
						rightButtonDown(false), leftButtonUp(false), rightButtonUp(false),
						wheelUp(false), wheelDown(false) {}
	} mouseState;

	// Evento que guarda el codigo de tecla, si esta pulsada y el codigo del raton
	struct Event
	{
		int keyCode;
		bool isPressedNow;
		Vector2d mousePosition;
		int mouseWheel; // -1, 0, 1
		// Para los botones y otros elementos
		int callerId;
		int type; // Tipo de evento
	};

	EventManager(void);
	~EventManager(void);

	// Recoge los managers
	void init();

	void setGUIManager(GUIManager* guiManager)
	{
		this->guiManager = guiManager;
	}

	// Eventos de irrlicht
	bool OnEvent(const irr::SEvent &event);
	// Eventos de SFML
	void pollEventSFML(sf::Event event);

	//Devuelve true solo en el momento en el que se baja la tecla
	bool isKeyDown(KEY::EKEY_CODE keyCode);

	//Devuelve true mientras la tecla este presionada
	bool isKeyPressed(KEY::EKEY_CODE keyCode);

	//Devuelve true solo en el momento en el que se sube la tecla
	bool isKeyUp(KEY::EKEY_CODE keyCode);

	//Pone a false todos los keyDown para que solo esten activos 1 frame como maximo, se llama desde GameManager->update()
	void cleanButtonsDowns();

	//void toggleKeyPressed(KEY::EKEY_CODE keyCode);
	//void setKeyPressed(KEY::EKEY_CODE keyCode, bool state);
	int getButtonDown();

private:
	bool keyPressed[KEY::KEY_KEY_CODES_COUNT];
	bool keyDown[KEY::KEY_KEY_CODES_COUNT];
	bool keyUp[KEY::KEY_KEY_CODES_COUNT];

	// Recibe un keycode de sfml y lo convierte a su correspondiente de irrlicht (el que usamos en KeyCode.h)
	int sfmlToIrrKey(int keyCode);
	// Recibe un keyCode de raton y lo convierte al que usamos en KeyCode.h
	int sfmlToIrrMouse(int keyCode, bool pressed);

	void handleKeyEvents(Event keyEvent);
	void handleMouse(Event &mouseEvent);
	// Gestiona los eventos que se activan desde botones y otros elementos del GUI
	void handleGuiEvents(Event guiEvent);
	//Para borrar cuando lo arregle
	int idButton;

	 //Para que no recorra todos los frames los arrays y solo lo haga si hay algo presionado
	bool isSomethingPressed;

	GameManager* gameManager;
	GUIManager* guiManager;
	AudioManager* audioManager;
	GraphicsEngine* graphicsEngine;
	NetworkEngine* networkEngine;
	GameObjectManager* gameObjectManager;
};

