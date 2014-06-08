#pragma once

#include <vector>
#include "irrlicht/irrlicht.h"

#include "ComponentHUD.h"
#include "NetworkEngine.h"
#include "MapManager.h"
#include "EventManager.h"
#include "GameSettings.h"
#include "Vector3d.h"

class GameManager;
class NodeCamera;
class NodeMesh;
class NodeText;
class Vector2d;
class FXEngine;

struct DebugLine
{
	Vector3d start;
	Vector3d end;
	int color;

	DebugLine(Vector3d start, Vector3d end, int color):start(start), end(end), color(color){}
};

struct DebugColor
{
	enum _ColorEnum
	{
		RED,
		GREEN,
		BLUE,
		YELLOW,
		WHITE,
		GREY,
		BROWN,
		_COLOR_COUNT
	};
};


class GraphicsEngine
{
public:
	GraphicsEngine(){};
	virtual~GraphicsEngine(){};
	
	virtual bool createWindow(EventManager* eventManager, GameSettings* gameSettings)= 0;

	virtual void loadSkybox(Vector3d skyboxRotation = Vector3d()){};
	
	// MENUS
	virtual void addReconnectButton()= 0;
	virtual void addServerBrowserButton()= 0;
	virtual void setMenuSettings()= 0;
	virtual void initMenuScreen()= 0;;
	virtual void createMenuMain()= 0;
	virtual void createMenuSettings()= 0;
	virtual void createMenuButton(int buttonNumber, const wchar_t* text, int id)= 0;
	virtual void createMenuServerBrowser()= 0;
	virtual void createMenuPathfinding()= 0; // Boton para reiniciar el mapa de obstaculos
	virtual void createMenuClientLobby()= 0;
	virtual void createMenuCreateGame()= 0;
	virtual void createMenuSelectCharacter() = 0;
	virtual void createMenuCredits() = 0;
	// Menu de juego para continuar o salir de la partida
	virtual void createMenuInGame() = 0; // No pausa el juego
	virtual void toggleMenuInGame() = 0; // Muestra/oculta el menu in game

	virtual void clearServerTable()= 0;
	virtual void addServerToBrowserTable(ServerInfo serverInfo)= 0;
	virtual void updateTablePlayers(std::vector<PlayerInfo> playersList)= 0;
	virtual void updateResolution()=0;
	virtual void createInventory()=0;
	virtual void addInventoryLogo(int skill,const char* resource) = 0;


	virtual void createMenuGameOver(int levelRound,Clock wastedTime,int enemiesKilled)=0;
	virtual void draw(void)= 0;
	//Dibuja la gui sin tener que dibujar todo
	virtual void drawGUI()= 0;
	virtual void clear()= 0;
	virtual void quit()= 0;

	virtual Vector2d getMousePositionOnGround()= 0;

	virtual bool isRunning()= 0;

	virtual NodeMesh* addNodeMesh(const char* meshFilename, bool transparent = false, int debugColor = DebugColor::RED , Vector2d position = Vector2d(-500, -500), float rotation = 0, float scale = 1)= 0;
	virtual NodeCamera* addNodeCamera(Vector3d position = Vector3d(20, 50, -20), Vector3d target = Vector3d(0, 0, 0))= 0;
	virtual NodeText* addNodeText()= 0;
	virtual void addDescriptionText(int skillNumber, std::wstring description) = 0;
	virtual void changeNodeColor(NodeMesh* node, int debugColor)= 0;

	virtual void drawDebugLine(Vector3d start, Vector3d end, int color = DebugColor::WHITE)= 0;
	virtual void initWorld()= 0;

	virtual void setMenuFocus(int id)= 0; //Selecciona un boton de menu
	virtual void setNextMenuFocus(bool reverse = false)= 0; //Selecciona el siguiente boton del menu
	virtual void setMenuPressed(int id) = 0; // Activa un boton


	virtual Vector2d getScreenCoordinatesFrom3DPosition(Vector3d position)= 0;

	//Para transformar de wstringstream a wchar_t* es con variable.str().c_str(), ejemplo en ComponentDebugGameObject
	virtual void setDebugText(const wchar_t* text)= 0;

	// Agrega un cube en una posicion 2d, con un tamaño 2d(ancho(x,z), alto(y)) y un color
	virtual NodeMesh* addCube(Vector2d position, Vector2d size, int color, bool isMap = false)= 0;

	virtual Vector2d getWindowSize()= 0;

	// Para devolver el contenido del listBoxServers
	virtual unsigned int getSelectedTableServers()= 0;

	virtual bool isChecked()= 0;
	virtual bool isFullScreenChecked()= 0;
	virtual bool isVSyncChecked()= 0;
	virtual bool isIrrlichtChecked()= 0;
	virtual bool isPhysicsChecked() = 0;
	virtual bool isAliasingChecked() = 0;
	virtual bool isMusicChecked() = 0;
	virtual bool canSeeTarget( Vector2d start, Vector2d end)= 0;


	virtual void createDebugMap(MapManager::CollisionMap &collisionMap)= 0;
	virtual void putBegin(bool put)= 0;

	virtual unsigned int getUpdateTime()=0;
	virtual unsigned int getUpdateVSTime()=0;
	virtual void setHudData(HUDData data) = 0;

	virtual void drawUpdateTimesRectangles(UpdateTimes)=0; 
	virtual void addDebugTimeRectangles()=0;

	virtual void setUpdateTimeRectanglesVisible(bool)=0;

	virtual void createHudCharacter() = 0;
	virtual void createHudMiniMap() = 0;
	virtual void createHudTopBar() = 0;
	virtual void createHUD() = 0;



	//Metodos para el miniMapa
	virtual int addGUISprite(const char* spriteFilename, Vector2d position, Vector2d size) = 0;
	virtual void setPositionGUISprite(int id,Vector2d position, Vector2d size) = 0;
	virtual void removeGUISprite(int id) = 0;
	virtual void setColorGUISprite(int id, int color)= 0;
	virtual void setScaleGUISprite(int id, Vector2d scale) = 0;
	virtual void setVisibleGUISprite(int id, bool visible) = 0;
	//Temporal para que no se dibujen las flechas, se necesita para acceder desde fuera
	//Desactiva o Activa el dibujado de los cubos del mapa de fuerza
	virtual void changeDrawCube() = 0;
	//Desactiva o Activa el dibujado de las flechas de fuerza
	virtual void changeDrawArrow() = 0;

	//Descripcion de habilidades
	virtual void addDescription(int id) = 0;
	virtual void deleteDescription() = 0;
	//Devuelve la siguiente ID;
	int getNextID()
	{
		nextID++;
		return nextID-1;		
	}

	virtual FXEngine* getFXEngine() = 0;

	//Shaders
	virtual Vector3d getCameraPosition() = 0;
	virtual Vector3d getLightPosition() = 0;
	virtual int getMaterialFromList(int type) = 0;

	virtual void setWarningMessage(const wchar_t* warningMessage) = 0; 

protected:

	//Guarda la ID
	int nextID;

	// Managers
	GameManager* gameManager;
	GameSettings* gameSettings;
};


