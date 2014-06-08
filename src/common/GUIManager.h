#pragma once

#include "EventManager.h"
#include "GUIElement.h"

#include <SFML/Graphics.hpp>
#include <vector>

class GUIButton;
class GUICheckbox;
class GUITable;
class GUISprite;
class GUIText;

namespace GUI
{
	struct BUTTON_ID
	{
		enum ID
		{
			//Menu principal
			EXIT = 50,
			RESUME,
			BACK,
			SERVER_BROWSER,
			RECONNECT,
			DISCONNECT,
			NEXT,
			SETTINGS,
			CREDITS,

			//Menu opciones
			RESOLUTION_NEXT = 75,
			RESOLUTION_BEFORE,
			ANTIALIAS,
			APPLY,
			SETTINGS_BACK,
			ACCEPT_CHANGES,
			VOLUMEN,
			V_SYNC,
			IRRLICHT,
			PHYSICS,
			ANTIALIASING,
			FULLSCREEN,
			MUSIC,
			APPLY_WINDOW,

			//Tests, estos en la version final no estaran
			OLD_TESTS = 100,
			GAME_OBJECT,
			COLLISIONS,
			PATHFINDING,
			STEERING,
			WAVES,
			SQUADS,
			ONLINE,

			//State Server Browser
			REFRESH = 200,
			JOIN,
			CREATE_GAME,	// Abre pantalla de crear partida
			LAUNCH_SERVER,	// Lanzar el servidor al crear la partida
			CLOSE_SERVER,
			UPDATE,
			READY_CHECKBOX,
			BROWSER_TABLE, //No es un boton pero tambien necesita ID

			//State Client Lobby
			START = 250,
			BOMBER,
			BATTLESHIP,
			ENGINEER,
			FIGHTER,
			//Faltarian los 4 botones para elegir clase de la nave, caza, bomber, etc

			LAST_MENU_BUTTON = 399,//Hace falta para poder controlar por teclado los botones de menu


			//HUD Ingame a partir del 400
			INVENTORY = 400,
			SKILL1,
			SKILL2,
			SKILL3,
			SKILL4,
			SKILL5,
			SKILL6,
			SKILL7,
			SKILL8,
			//Ingame para Tests

			RESTART_MAP = 600,
			//Esto no son botones propiamente dicho y petan cuando se pulsan o se interacciona con ellos
		};
	};

	// Tipografias
	static const char* DEFAULT_FONT = "../assets/fonts/liberation_mono_regular.ttf";
	static const char* MENU_BUTTON_FONT = "../assets/fonts/OpenSans.ttf";
	const int DEFAULT_FONT_SIZE = 12;

	static const char* IMAGE_TITLE = "../assets/images/gui/title.png";
	// Menus
	static const char* MENU_IMAGE_HELP_MAIN = "../assets/images/gui/help_mainmenu.png";
	static const char* MENU_IMAGE_HELP_BROWSER = "../assets/images/gui/help_serverbrowser.png";
	static const char* MENU_IMAGE_HELP_LOBBY = "../assets/images/gui/help_lobby.png";
	static const char* MENU_IMAGE_HELP_SETTINGS = "../assets/images/gui/help_settings.png";

	static const char* MENU_IMAGE_BUTTON_NORMAL = "../assets/images/gui/button.png";
	static const char* MENU_IMAGE_BUTTON_HOVER = "../assets/images/gui/button_hover.png";

	static const char* MENU_IMAGE_ARROW_LEFT = "../assets/images/gui/left.png";
	static const char* MENU_IMAGE_ARROW_LEFT_PRESSED = "../assets/images/gui/left_pressed.png";
	static const char* MENU_IMAGE_ARROW_RIGHT = "../assets/images/gui/right.png";
	static const char* MENU_IMAGE_ARROW_RIGHT_PRESSED = "../assets/images/gui/right_pressed.png";

	static const char* MENU_IMAGE_LOBBY_FIGHTER = "../assets/images/logos/Caza.png";
	static const char* MENU_IMAGE_LOBBY_BATTLESHIP = "../assets/images/logos/Acorazado.png";
	static const char* MENU_IMAGE_LOBBY_ENGINEER = "../assets/images/logos/Ingeniero.png";
	static const char* MENU_IMAGE_LOBBY_BOMBER = "../assets/images/logos/Bomber.png";
	static const char* MENU_IMAGE_IN_GAME_BG = "../assets/images/gui/ingame_menu_background.png";

	static const char* MENU_IMAGE_LOADING = "../assets/images/gui/loading.jpg";

	const int MENU_BUTTON_WIDTH = 335;	// Ancho del boton de menu
	const int MENU_BUTTON_HEIGHT = 65;	// Alto del boton de menu
	const int MENU_COLUMNS = 2;			// Numero de columnas en menus
	const int MENU_MAX_BUTTONS = 6;		// Maximo de botones en menus
	const int MENU_BUTTON_FONT_SIZE = 32;
	const int MENU_CHECBOX_FONT_SIZE = 18;
	const int MENU_BUTTON_VERTICAL_SEPARATION = 5;

	// HUD
	static const char* HUD_IMAGE_PANEL = "../assets/images/hud/Panel.png";
	static const char* HUD_IMAGE_INVENTORY = "../assets/images/hud/inventario.png";
	
	static const char* HUD_IMAGE_HP = "../assets/images/hud/hpBar.png";
	static const char* HUD_IMAGE_SH = "../assets/images/hud/shBar.png";
	static const char* HUD_IMAGE_HP_EMPTY = "../assets/images/hud/hpBarEmpty.png";
	static const char* HUD_IMAGE_SH_EMPTY = "../assets/images/hud/shBarEmpty.png";

	static const char* HUD_IMAGE_ICON_TURRET = "../assets/images/logos/Torreta.png";
	static const char* HUD_IMAGE_ICON_TURRET_2 = "../assets/images/logos/Torreta_2.png";
	static const char* HUD_IMAGE_ICON_TURRET_BIG_CANON = "../assets/images/logos/turret_big_canon.png";
	static const char* HUD_IMAGE_ICON_TURRET_1_CANON = "../assets/images/logos/turret_1_canon.png";
	static const char* HUD_IMAGE_ICON_MINE = "../assets/images/logos/Mina.png";
	static const char* HUD_IMAGE_ICON_MINE_2 = "../assets/images/logos/Mina2.png";
	static const char* HUD_IMAGE_ICON_MINE_3 = "../assets/images/logos/Mina3.png";
	static const char* HUD_IMAGE_ICON_WALL = "../assets/images/logos/Muro.png";
	static const char* HUD_IMAGE_ICON_TRIPLE = "../assets/images/logos/triple.png";
	static const char* HUD_IMAGE_ICON_MISSILE = "../assets/images/logos/misil.png";
	static const char* HUD_IMAGE_ICON_RAY = "../assets/images/logos/rayo.png";

	static const char* HUD_IMAGE_ICON_TURRET_BLACK = "../assets/images/logos/Torreta_black.png";
	static const char* HUD_IMAGE_ICON_TURRET_2_BLACK = "../assets/images/logos/Torreta_2_black.png";
	static const char* HUD_IMAGE_ICON_TURRET_BIG_CANON_BLACK = "../assets/images/logos/turret_big_canon_black.png";
	static const char* HUD_IMAGE_ICON_TURRET_1_CANON_BLACK = "../assets/images/logos/turret_1_canon_black.png";
	static const char* HUD_IMAGE_ICON_MINE_BLACK = "../assets/images/logos/Mina_black.png";
	static const char* HUD_IMAGE_ICON_MINE_2_BLACK = "../assets/images/logos/Mina2_black.png";
	static const char* HUD_IMAGE_ICON_MINE_3_BLACK = "../assets/images/logos/Mina3_black.png";
	static const char* HUD_IMAGE_ICON_TRIPLE_BLACK = "../assets/images/logos/triple_black.png";
	static const char* HUD_IMAGE_ICON_MISSILE_BLACK = "../assets/images/logos/misil_black.png";

	static const char* HUD_IMAGE_ICON_BATTLESHIP = "../assets/images/logos/AcorazadoImg.png";
	static const char* HUD_IMAGE_ICON_BOMBER = "../assets/images/logos/BomberImg.png";
	static const char* HUD_IMAGE_ICON_FIGHTER = "../assets/images/logos/CazaImg.png";
	static const char* HUD_IMAGE_ICON_ENGINEER = "../assets/images/logos/IngenieroImg.png";

	const int HUD_ICON_WIDTH = 50;
	const int HUD_ICON_HEIGHT = 50;

	static const char* HUD_IMAGE_EXPBAR = "../assets/images/hud/expBar.png";
	static const char* HUD_IMAGE_PJ = "../assets/images/hud/PJ.png";
	static const char* HUD_IMAGE_MINIMAP = "../assets/images/hud/minimapa.png";
	
	// Debug
	static const char* DEBUG_IMAGE_RECTANGLE = "../assets/images/updateTimeRectangle.png";
	const int DEBUG_RECTANGLE_WIDTH = 10;
	const int DEBUG_RECTANGLE_HEIGHT = 50;
	const int DEBUG_RECTANGLE_SEPARATION = 42;
}

class GUIManager
{
	public:
		GUIManager();
		~GUIManager();

		// Texto
		GUIText* addText(Vector2d position, const wchar_t* string, int id = -1 );

		// Boton de tipo menu principal
		GUIButton* addMenuButton(float left, float top, float width, float height, int id, const wchar_t* text);
		
		// Boton generico
		GUIButton* addButton( float left, float top, float width, float height, int id = -1, const wchar_t* string = L"" );

		// Checkbox
		GUICheckbox* addCheckBox( bool checked, float left, float top, int id, const wchar_t* string );

		// Tabla
		GUITable* addTable(float left, float top, int id);

		// Sprite/Imagen
		GUISprite* addImage( Vector2d position, const char* fileName, int id = -1 );
	
		// Devuelve true si el event manager debe activar un evento de clic en un elemento del GUI 
		bool onEvent(EventManager::Event& event);

		// Elimina los elementos de la lista
		void clear();

		// Manda al dibujado de cada elemento
		void draw(sf::RenderWindow* window);

		//Devuelve un elemento.
		GUIElement* getElementFromId( int id );

		//Borra un elemento
		void removeChildFromId( int id );

		void removeChild(GUIElement* child);

	private:
		std::vector<GUIElement*> guiElements;

		std::string defaultFont;
		std::string buttonFont;
		int defaultFontSize;
		int buttonFontSize;

		// Agrega un elemento del GUI a la lista
		void addElement(GUIElement* element)
		{
			guiElements.push_back(element);
		}
};

