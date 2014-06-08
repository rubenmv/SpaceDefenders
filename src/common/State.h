/*
 * State.h
 * Plantilla de estado de juego
 */

#pragma once

#include <iostream>
#include "GameManager.h" //Si se quita, toca incluirlo en todos los states*.cpp

class State
{
public:
	enum StateName
	{
		INTRO,				// Introduccion del juego
		MENU_MAIN,			// Menu principal
		MENU_OPTIONS,		// Menu de opciones
		MENU_CREATE_GAME,	// Pantalla de creacion de partida
		MENU_CREDITS,		// Creditos del juego
		MENU_CLIENT_LOBBY,	// Lobby del cliente (seleccion de nave y confirmacion)
		SERVER_LOBBY,		// Lobby del server
		MENU_SERVER_BROWSER,// Buscador de servidores
		GAME_OVER,
		// Tests
		MENU_TESTS,
		TEST_CLOCK,
		// Durante la partida
		// Los tests ingame deben ir despues de este estado
		IN_GAME,
		TEST_COLLISION,
		TEST_GAMEOBJECT,
		TEST_MINE,
		TEST_PATHFINDING,
		TEST_STEERING,
		TEST_WAVES,
		TEST_SHOT,
		TEST_REVIVE,
		TEST_CLIENT,
		TEST_SERVER,
		TEST_BROWSER,
		TEST_ONLINE,
		TEST_SFML,
		STATE_NAME_COUNT // Esto siempre al final
	};

	static const char* NAME_STRINGS[];

	friend std::ostream& operator<<(std::ostream& os, enum StateName n)
	{
		if (n >= STATE_NAME_COUNT || n < 0) return os << "???";
		return os << NAME_STRINGS[n];
	}
	friend std::wostream& operator<<(std::wostream& os, enum StateName n)
	{
		if (n >= STATE_NAME_COUNT || n < 0) return os << "???";
		return os << NAME_STRINGS[n];
	}

	virtual void init() = 0;
	virtual void cleanup() = 0;
	virtual void update(GameManager* gameManager) = 0;

	StateName getName()
	{
		return name;
	}

protected:
	GameManager* gameManager;
	StateName name;
};
