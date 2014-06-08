#include "State.h"


//Respetar orden o peta
const char* State::NAME_STRINGS[] = {
		"INTRO",				// Introduccion del juego
		"MENU_MAIN",			// Menu principal
		"MENU_OPTIONS",		// Menu de opciones
		"MENU_CREATE_GAME",	// Pantalla de creacion de partida
		"MENU_CLIENT_LOBBY",	// Lobby del cliente (seleccion de nave y confirmacion)
		"SERVER_LOBBY",		// Lobby del server
		"MENU_SERVER_BROWSER",// Buscador de servidores
		// Tests
		"MENU_TESTS",
		
		"TEST_CLOCK",
		"IN_GAME",
		"TEST_COLLISION",
		"TEST_GAMEOBJECT",
		"TEST_MINE",
		"TEST_PATHFINDING",
		"TEST_STEERING",
		"TEST_WAVES",
		"TEST_SHOT",
		"TEST_REVIVE",
		"TEST_CLIENT",
		"TEST_SERVER",
		"TEST_BROWSER",
		"TEST_ONLINE",
		"TEST_SFML"
};