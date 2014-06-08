#include "EventManager.h"

#include <iostream>
#include "GameManager.h"
#include "AudioManager.h"
#include "GraphicsEngine.h"
#include "NetworkEngine.h"
#include "GameObjectManager.h"

#include "GUIManager.h"
#include "GUIElement.h"

// States
#include "StateInGame.h"
#include "StateServerBrowser.h"
#include "StateMenuMain.h"
#include "StateMenuTests.h"
#include "StateMenuOptions.h"
#include "StateMenuCredits.h"
#include "StateMenuCreateGame.h"
#include "StateSelectCharacter.h"

#include "tests/StateTestGameObject.h"
#include "tests/StateTestClient.h"
#include "tests/StateTestClock.h"
#include "tests/StateTestPathfinding.h"
#include "tests/StateTestWaves.h"
#include "tests/StateTestEnemyShot.h"
#include "tests/StateClientInGame.h"
#include "tests/StateTestAIFury.h"
#include "tests/StateTestOnline.h"
#include "tests/StateBenchmark.h"

using namespace GUI;

EventManager::EventManager(void)
{
	for (int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++) {
		keyDown[i] = false;
		keyPressed[i] = false;
		keyUp[i] = false;
	}
	isSomethingPressed = false;

	gameManager = GameManager::getInstance();
	guiManager = NULL; // Esto se lo asigna el graphicsEngine
	audioManager = NULL;
}


EventManager::~EventManager(void)
{
	gameManager = NULL;
	guiManager = NULL;
	audioManager = NULL;
}

void EventManager::init()
{
	graphicsEngine = gameManager->getGraphicsEngine();
	networkEngine = gameManager->getNetworkEngine();
	gameObjectManager = gameManager->getGameObjectManager();
	audioManager = gameManager->getAudioManager();
}

// Eventos de irrlicht
bool EventManager::OnEvent(const irr::SEvent &irrEvent)
{
	Event event;

	switch (irrEvent.EventType) 
	{
		case irr::EET_KEY_INPUT_EVENT:
			event.keyCode = irrEvent.KeyInput.Key;
			event.isPressedNow = irrEvent.KeyInput.PressedDown;
			handleKeyEvents(event);
			break;
		case irr::EET_MOUSE_INPUT_EVENT:
			event.keyCode = irrEvent.MouseInput.Event;
			event.mousePosition = Vector2d(irrEvent.MouseInput.X, irrEvent.MouseInput.Y);

			// Si es de tipo rueda
			if (event.keyCode == irr::EMIE_MOUSE_WHEEL)
			{
				event.mouseWheel = (int)irrEvent.MouseInput.Wheel;
			}
			
			handleMouse(event);
			break;
		case irr::EET_GUI_EVENT:
			switch (irrEvent.GUIEvent.EventType)
			{
				case irr::gui::EGET_BUTTON_CLICKED: // Boton clicado
					event.type = GUIElement::State::ACTIVE;
					break;
				case irr::gui::EGET_ELEMENT_HOVERED: // Hover, por encima
					event.type = GUIElement::State::HOVERED;
					break;
				case irr::gui::EGET_ELEMENT_LEFT: // Pierde el foco
					event.type = GUIElement::State::NORMAL;
					break;
				case irr::gui::EGET_ELEMENT_FOCUSED: // Foco
					event.type = GUIElement::State::FOCUSED;
					break;
			}
			event.callerId = irrEvent.GUIEvent.Caller->getID();
			handleGuiEvents(event);
			break;
	}

	return false;
}

// Eventos de SFML
void EventManager::pollEventSFML(sf::Event sfEvent)
{
	Event event;

	// Convierte las teclas a su correspondiente en KeyCode.h
	switch (sfEvent.type)
	{
		// Se cierra la ventana, lo tratamos como si se pulsara escape
		
		case sf::Event::Closed:
			gameManager->quit();
			
		// TECLADO
		case sf::Event::KeyPressed:
			event.keyCode = sfmlToIrrKey(sfEvent.key.code);
			event.isPressedNow = true;
			handleKeyEvents(event);
			break;
		case sf::Event::KeyReleased:
			event.keyCode = sfmlToIrrKey(sfEvent.key.code);
			event.type = event.keyCode;
			event.isPressedNow = false;
			handleKeyEvents(event);
			break;
		// RATON
		case sf::Event::MouseButtonPressed: 
			event.isPressedNow = true;
			event.keyCode = sfmlToIrrMouse(sfEvent.mouseButton.button, event.isPressedNow);
			event.type = event.keyCode;
			handleMouse(event);
			break;
		case sf::Event::MouseButtonReleased:
			event.isPressedNow = false;
			event.keyCode = sfmlToIrrMouse(sfEvent.mouseButton.button, event.isPressedNow);
			event.type = event.keyCode;
			handleMouse(event);

			if (guiManager != NULL)
			{
				if ( guiManager->onEvent(event) )
				{
					handleGuiEvents(event);
				}
			}
			break;
		case sf::Event::MouseWheelMoved:
			event.keyCode = KEY::EMOUSE_INPUT_EVENT::EMIE_MOUSE_WHEEL;
			event.mouseWheel = sfEvent.mouseWheel.delta;
			handleMouse(event);
			break;
		case sf::Event::MouseMoved:
			event.mousePosition = Vector2d(sfEvent.mouseMove.x, sfEvent.mouseMove.y);
			event.keyCode = KEY::EMOUSE_INPUT_EVENT::EMIE_MOUSE_MOVED;
			handleMouse(event);

			if (guiManager != NULL)
			{
				if (guiManager->onEvent(event))
				{
					handleGuiEvents(event);
				}
			}
			break;
	}
}

// Recibe un keycode de sfml y lo convierte a su correspondiente de irrlicht (el que usamos en KeyCode.h)
int EventManager::sfmlToIrrKey(int keyCode)
{
	int irrCode = -1;

	// Letras
	if (keyCode >= sf::Keyboard::A && keyCode <= sf::Keyboard::Z)
	{
		irrCode = (keyCode - sf::Keyboard::A) + KEY::KEY_KEY_A;
	}
	// Numeros
	if (keyCode >= sf::Keyboard::Num0 && keyCode <= sf::Keyboard::Num9)
	{
		irrCode = (keyCode - sf::Keyboard::Num0) + KEY::KEY_KEY_0;
	}
	// Teclas F
	if (keyCode >= sf::Keyboard::F1 && keyCode <= sf::Keyboard::F15)
	{
		irrCode = (keyCode - sf::Keyboard::F1) + KEY::KEY_F1;
	}
	// Otras teclas especiales
	switch (keyCode)
	{
	case sf::Keyboard::Escape:
		irrCode = KEY::EKEY_CODE::KEY_ESCAPE;
		break;
	case sf::Keyboard::Space:
		irrCode = KEY::EKEY_CODE::KEY_SPACE;
		break;
	case sf::Keyboard::Return:
		irrCode = KEY::EKEY_CODE::KEY_RETURN;
		break;
	case sf::Keyboard::LControl:
		irrCode = KEY::EKEY_CODE::KEY_LCONTROL;
		break;
	case sf::Keyboard::LShift:
		irrCode = KEY::EKEY_CODE::KEY_LSHIFT;
		break;
	case sf::Keyboard::LAlt:
		irrCode = KEY::EKEY_CODE::KEY_LMENU; 
		break;
	case sf::Keyboard::PageUp:
		irrCode = KEY::EKEY_CODE::KEY_PRIOR;
		break;
	case sf::Keyboard::PageDown:
		irrCode = KEY::EKEY_CODE::KEY_NEXT;
		break;
	}

	return irrCode;
}

// Recibe un keyCode de raton y lo convierte al que usamos en KeyCode.h
int EventManager::sfmlToIrrMouse(int keyCode, bool pressed)
{
	int irrCode = -1;

	// Convierte teclas de raton
	if (keyCode == sf::Mouse::Left)
	{
		if (pressed)
		{
			irrCode = KEY::EMOUSE_INPUT_EVENT::EMIE_LMOUSE_PRESSED_DOWN;
		}
		else
		{
			irrCode = KEY::EMOUSE_INPUT_EVENT::EMIE_LMOUSE_LEFT_UP;
		}
	}
	else if (keyCode == sf::Mouse::Right)
	{
		if (pressed)
		{
			irrCode = KEY::EMOUSE_INPUT_EVENT::EMIE_RMOUSE_PRESSED_DOWN;
		}
		else
		{
			irrCode = KEY::EMOUSE_INPUT_EVENT::EMIE_RMOUSE_LEFT_UP;
		}
	}

	return irrCode;
}

// Una tecla esta siendo presionada
bool EventManager::isKeyPressed(KEY::EKEY_CODE keyCode)
{
	return keyPressed[keyCode];
}
// Se presiona una tecla
bool EventManager::isKeyDown(KEY::EKEY_CODE keyCode)
{
	return keyDown[keyCode];
}
// Se deja de presionar una tecla
bool EventManager::isKeyUp(KEY::EKEY_CODE keyCode)
{
	return !keyDown[keyCode];
}

// Eventos de teclado
void EventManager::handleKeyEvents(Event keyEvent)
{
	int keyCode = keyEvent.keyCode;

	bool wasPressed = keyPressed[keyCode];
	bool isPressedNow = keyEvent.isPressedNow;

	if(!wasPressed && isPressedNow)
	{
		keyDown[keyCode] = true;
		isSomethingPressed = true;
	}
	else
	{
		keyDown[keyCode] = false;
	}

	if(wasPressed && !isPressedNow)
	{
		keyUp[keyCode] = true;
	}
	else
	{
		keyUp[keyCode] = false;
	}

	keyPressed[keyCode] = isPressedNow;
	
	// Cierra el programa
	if(keyPressed[KEY::KEY_ESCAPE])
	{
		if (!gameManager->isMenuState())
		{
			graphicsEngine->toggleMenuInGame();
		}
		else
		{
			gameManager->quit();
		}
		return;
	}
	// Vuelve al menu de inicio
	if(keyPressed[KEY::KEY_KEY_M])
	{
		audioManager->mute(!audioManager->isMute);
		return;
	}
	// Vuelve al menu de inicio
	/*if(keyPressed[KEY::KEY_F10])
	{
		networkEngine->close();
		gameManager->changeState(StateMenuMain::getInstance());
		return;
	}*/
}

// Estados de raton
void EventManager::handleMouse(Event &mouseEvent)
{
	// Por defecto la posicion actual para los casos de clic
	// De esta manera luego se le pasa al gui manager
	if ( mouseEvent.keyCode != KEY::EMOUSE_INPUT_EVENT::EMIE_MOUSE_MOVED )
	{
		mouseEvent.mousePosition = mouseState.position;
	}
	
	mouseEvent.type = mouseEvent.keyCode;

	switch( mouseEvent.keyCode )
	{
		case KEY::EMOUSE_INPUT_EVENT::EMIE_LMOUSE_PRESSED_DOWN:
			mouseState.leftButtonPressed = true;
			mouseState.leftButtonDown = true;
			break;

		case KEY::EMOUSE_INPUT_EVENT::EMIE_LMOUSE_LEFT_UP:
			mouseState.leftButtonPressed = false;
			mouseState.leftButtonUp = true;
			break;

		case KEY::EMOUSE_INPUT_EVENT::EMIE_RMOUSE_PRESSED_DOWN:
			mouseState.rightButtonPressed = true;
			mouseState.rightButtonDown = true;
			break;

		case KEY::EMOUSE_INPUT_EVENT::EMIE_RMOUSE_LEFT_UP:
			mouseState.rightButtonPressed = false;
			mouseState.rightButtonUp = true;
			break;

		case KEY::EMOUSE_INPUT_EVENT::EMIE_MOUSE_MOVED:
			mouseState.position = mouseEvent.mousePosition;
			break;
		case KEY::EMOUSE_INPUT_EVENT::EMIE_MOUSE_WHEEL:
			if (mouseEvent.mouseWheel < 0)
			{
				mouseState.wheelDown = true;
			}
			else if (mouseEvent.mouseWheel > 0)
			{
				mouseState.wheelUp = true;
			}
			break;
	}
}

// Eventos de menus y otros elementos de la interfaz
void EventManager::handleGuiEvents(Event event)
{
	int id = event.callerId;

	switch(event.type)
	{
		case GUIElement::State::ACTIVE: // clicado o activo por teclado
		{
			mouseState.leftButtonDown = false;
			idButton = id;
			Message message;
			// -----------------------------------------------------------
			// Ids entre 100 a 199 son para el menu princial
			// Ids 200 son para el server broswser
			// Ids 250 para el lobby
			// Ids 400 para dentro del juego
			switch(id) 
			{
			// NAVEGACION
			case BUTTON_ID::BACK: //Boton Atras
				gameManager->changeState(StateMenuMain::getInstance());
				break;
			case BUTTON_ID::RESUME:
				graphicsEngine->toggleMenuInGame();
				break;
			case BUTTON_ID::SETTINGS_BACK: //Boton Atras
				gameManager->getGameSettings()->reset();
				gameManager->changeState(StateMenuMain::getInstance());
				break;
			case BUTTON_ID::APPLY:
				gameManager->getGameSettings()->apply();
				gameManager->getGameSettings()->saveSettings();
				//gameManager->getGameSettings()->loadSettings();
				gameManager->setRestart(true);
				gameManager->quit();
				break;
			case BUTTON_ID::EXIT:
				gameManager->quit();
				break;
			// STATES
			case BUTTON_ID::SETTINGS:
				gameManager->changeState(StateMenuOptions::getInstance());
				break;
			case BUTTON_ID::CREDITS:
				gameManager->changeState(StateMenuCredits::getInstance());
				break;
			case BUTTON_ID::GAME_OBJECT:
				gameManager->changeState(StateTestGameObject::getInstance());
				break;
			case BUTTON_ID::PATHFINDING:
				gameManager->changeState(StateTestPathfinding::getInstance());
				break;
			case BUTTON_ID::ONLINE:
				gameManager->changeState(StateTestOnline::getInstance());
				break;	
			case BUTTON_ID::SERVER_BROWSER:
				gameManager->changeState(StateServerBrowser::getInstance());
				break;	
			case BUTTON_ID::WAVES:
				gameManager->changeState(StateSelectCharacter::getInstance());
				break;	
			case BUTTON_ID::OLD_TESTS:
				gameManager->changeState(StateMenuTests::getInstance(0));
				break;
			case BUTTON_ID::SQUADS:
				gameManager->changeState(StateBenchmark::getInstance());
				break;
			// NETWORK
			case BUTTON_ID::DISCONNECT:
				networkEngine->removeServerFile();
				networkEngine->close();
				gameManager->changeState(StateMenuMain::getInstance());
				break;
			case BUTTON_ID::RECONNECT:
				gameManager->changeState(StateServerBrowser::getInstance());
				break;
			case BUTTON_ID::SKILL1:
				gameManager->getGameObjectManager()->killInventories();
				message.type = Message::USE_SKILL;
				message.value = 1;
				gameManager->getGameObjectManager()->getMainPlayer()->broadcastMessage(message);
				break;
			case BUTTON_ID::SKILL2:
				gameManager->getGameObjectManager()->killInventories();
				message.type = Message::USE_SKILL;
				message.value = 2;
				gameManager->getGameObjectManager()->getMainPlayer()->broadcastMessage(message);
				break;
			case BUTTON_ID::SKILL3:
				gameManager->getGameObjectManager()->killInventories();
				message.type = Message::USE_SKILL;
				message.value = 3;
				gameManager->getGameObjectManager()->getMainPlayer()->broadcastMessage(message);
				break;
			case BUTTON_ID::SKILL4:
				gameManager->getGameObjectManager()->killInventories();
				message.type = Message::USE_SKILL;
				message.value = 4;
				gameManager->getGameObjectManager()->getMainPlayer()->broadcastMessage(message);
				break;
			case BUTTON_ID::SKILL5:
				gameManager->getGameObjectManager()->killInventories();
				message.type = Message::USE_SKILL;
				message.value = 5;
				gameManager->getGameObjectManager()->getMainPlayer()->broadcastMessage(message);
				break;
			case BUTTON_ID::SKILL6:
				gameManager->getGameObjectManager()->killInventories();
				message.type = Message::USE_SKILL;
				message.value = 6;
				gameManager->getGameObjectManager()->getMainPlayer()->broadcastMessage(message);
				break;
			case BUTTON_ID::SKILL7:
				gameManager->getGameObjectManager()->killInventories();
				message.type = Message::USE_SKILL;
				message.value = 7;
				gameManager->getGameObjectManager()->getMainPlayer()->broadcastMessage(message);
				break;
			case BUTTON_ID::SKILL8:
				gameManager->getGameObjectManager()->killInventories();
				message.type = Message::USE_SKILL;
				message.value = 8;
				gameManager->getGameObjectManager()->getMainPlayer()->broadcastMessage(message);
				break;
			}
			break;		
		}

		// -----------------------------------------------------------
		// EFECTOS EN BOTONES
		case GUIElement::State::HOVERED: // Hover, por encima
		{
			if(id >= 50 && id < 600)
			{
				if(id>=400 && id<500)
				{
					graphicsEngine->addDescription(id);
				}
				graphicsEngine->setMenuFocus(id);
			}
			break;
		}
		case GUIElement::State::NORMAL: // Pierde el foco
		{
			if(id >= 50 && id < 600)
			{
				if(id>=400 && id<500)
				{
					graphicsEngine->deleteDescription();
				}
				graphicsEngine->setMenuFocus(-1);
			}
			break;
		}
		case GUIElement::State::FOCUSED: // Foco
		{
			if(id >= 50 && id < 600)
			{
				graphicsEngine->setMenuPressed(id);
			}
			break;
		}
	}
}

// Devuelve el id del boton presionado
int EventManager::getButtonDown()
{
	int idTemp = idButton;
	idButton = 0;
	return idTemp;		
}

// Limpia/inicializa los estados de presionado de botones
void EventManager::cleanButtonsDowns()
{
	mouseState.leftButtonDown = false;
	mouseState.leftButtonUp = false;
	mouseState.rightButtonDown = false;
	mouseState.rightButtonUp = false;
	mouseState.wheelDown = false;
	mouseState.wheelUp = false;

	if(isSomethingPressed)
	{
		for (int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++)
		{
			keyDown[i] = false;
			keyUp[i] = false;
		}
		isSomethingPressed = false;
	}
}
