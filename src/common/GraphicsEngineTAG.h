#pragma once

#include "GraphicsEngine.h"
#include "GUICheckbox.h"
#include "GUITable.h"

class SceneManager;
class SceneNode;
class NodeMesh;
class NodeCamera;
class NodeText;
class EventManager;
class MapManager;
class GameSettings;
class GUIManager;
class GUIButton;
class ShaderProgramTAG;
class FXEngineTAG;

class GraphicsEngineTAG: public GraphicsEngine
{
public:
	GraphicsEngineTAG();
	virtual~GraphicsEngineTAG();


	bool createWindow(EventManager* eventManager, GameSettings* gameSettings);
	void loadSkybox(Vector3d skyboxRotation = Vector3d());

	// MENUS
	void createInventory();
	GUIButton* createButton(int x, int y, int width, int height, const wchar_t *text, int id);
	void createMenuButton(int buttonNumber, const wchar_t* text, int id);
	void addReconnectButton();
	void addServerBrowserButton();
	void setMenuSettings();
	void initMenuScreen();
	void createMenuServerBrowser();
	void createMenuPathfinding(); // Boton para reiniciar el mapa de obstaculos
	void createMenuClientLobby();
	void createMenuMain();
	void createMenuSettings();
	void createMenuCredits();
	void createMenuGameOver(int levelRound,Clock wastedTime,int enemiesKilled);
	void createMenuCreateGame();
	void createMenuSelectCharacter();
	// Menu de juego para continuar o salir de la partida
	void createMenuInGame();
	void toggleMenuInGame(); // Muestra/oculta el menu in game
	// Para TAG simplemente guardamos los botones y la imagen en un array
	std::vector<GUIElement*> inGameMenu;

	void clearServerTable();
	void addServerToBrowserTable(ServerInfo serverInfo);
	void updateTablePlayers(std::vector<PlayerInfo> playerList);
	void updateResolution();

	void draw(void);
	void drawAll();
	void drawGUI(void);
	void clear();
	void quit();

	Vector2d getMousePositionOnGround();

	bool isRunning()
	{
		return running;
	}

	NodeMesh* addNodeMesh(const char* meshFilename, bool transparent = false, int debugColor = DebugColor::RED , Vector2d position = Vector2d(-500, -500), float rotation = 0, float scale = 1);
	NodeCamera* addNodeCamera(Vector3d position = Vector3d(20, 50, -20), Vector3d target = Vector3d(0, 0, 0));
	NodeText* addNodeText( );
	void removeNodeText(NodeText* nodeText);
	
	void changeNodeColor(NodeMesh* node, int debugColor);

	void drawDebugLine(Vector3d start, Vector3d end, int color = DebugColor::WHITE);
	void initWorld();

	void setMenuFocus(int id); //Selecciona un boton de menu
	void setNextMenuFocus(bool reverse = false); //Selecciona el siguiente boton del menu
	void setMenuPressed(int id); // Activa un boton

	Vector2d getScreenCoordinatesFrom3DPosition(Vector3d position);

	//Para transformar de wstringstream a wchar_t* es con variable.str().c_str(), ejemplo en ComponentDebugGameObject
	void setDebugText(const wchar_t* text);

	// Agrega un cube en una posicion 2d, con un tamaño 2d(ancho(x,z), alto(y)) y un color
	NodeMesh* addCube(Vector2d position, Vector2d size, int color, bool isMap = false);

	Vector2d getWindowSize()
	{
		return windowSize;
	}

	bool canSeeTarget( Vector2d start, Vector2d end);
	
	void createDebugMap(MapManager::CollisionMap &collisionMap);
	void putBegin(bool put);

	void setHudData(HUDData data);
	void createHudCharacter();
	void createHudMiniMap();
	void createHudTopBar();
	void createHUD();
	void addInventoryLogo(int skill,const char* resource);
	unsigned int getUpdateTime();
	unsigned int getUpdateVSTime();

	void drawUpdateTimesRectangles(UpdateTimes ut);
	void addDebugTimeRectangles();
	void setUpdateTimeRectanglesVisible(bool visible);
	void updateTimeRectangleSize( int time, GUISprite* rect );


	//Metodos MiniMapa
	int addGUISprite(const char* spriteFilename, Vector2d position, Vector2d size);
	void setPositionGUISprite(int id,Vector2d position,Vector2d size);
	void removeGUISprite(int id);
	void setColorGUISprite(int id, int color);
	void setScaleGUISprite(int id, Vector2d scale);
	void setVisibleGUISprite(int id, bool visible);
	
	unsigned int getSelectedTableServers()
	{
		return tableServers->getSelected();
	}


	bool isChecked()
	{
		return checkBoxReady->isChecked();
	}

	// Devuelve el manager de escena
	SceneManager* getSceneManager()
	{
		return sceneManager;
	}

	FXEngine* getFXEngine()
	{
		return (FXEngine*)fxEngine;
	}

	// Indica si el soporte para VBO esta activo
	bool isVBOSupported()
	{
		return useVBO;
	}

	//Descripcion de las habilidades
	void addDescription(int id);
	void deleteDescription();
	void addDescriptionText(int skillNumber, std::wstring description);

	//Shaders
	virtual Vector3d getCameraPosition()
	{	
		return Vector3d(0,0,0);
	}
	virtual Vector3d getLightPosition()
	{	
		return Vector3d(0,0,0);
	}
	virtual int getMaterialFromList(int type)
	{
		return 0;
	}

	void setWireframe(bool value);
	void setCulling(bool value);
	void setTextures(bool value);

	bool getTextures()
	{
		// En modo wireframe nunca activa texturas
		if (wireframe)
		{
			return false;
		}
		return textures;
	}

	// Comprueba si se soporta cierta extension de OpenGL
	bool isExtensionSupported( const char* szTargetExtension );

	void setWarningMessage(const wchar_t* warningMessage);; 


private:
	enum Skybox_sides{
		SKY_FRONT,
		SKY_RIGHT,
		SKY_LEFT,
		SKY_BACK,
		SKY_UP,
		SKY_DOWN
	};

	// Ventana SFML para trabajar directamente con OpenGL
	sf::RenderWindow *window;
	Clock fpsClock;
	int lastFPS;
	Vector2d windowSize;
	bool useVBO;

	// Managers
	GUIManager* guiManager;
	SceneManager* sceneManager;
	EventManager* eventManager;
	FXEngineTAG* fxEngine;

	sf::Texture skybox[6];

	// Grid. Guarda los puntos de cada linea a dibujar
	//std::vector<float> gridPoints;
	unsigned int gridVBO;
	unsigned int gridNumPoints;

	int color;
	bool fullscreen;
	bool running;

	// Extensiones
	unsigned int glewStatus;

	// Flags  para framebuffer
	bool renderToTexture;
	bool fboSupport;
	unsigned int renderedTexture, // Textura donde se renderiza la escena
		depthStencilBuffer, // buffers depth y stencil enlazados al framebuffer
		framebuffer,		// Framebuffer object
		uAttrCoord, uAttrTex, // Atributos para el shader del render a textura
		vboQuadVert;			  // Vertices del quad donde se dibuja la textura
	ShaderProgramTAG* shaderRender;

	// Muestra un informe con info sobre la inicializacion de la ventana y otras cosas de opengl
	void logWindowInit();
	// Indica el estado actual del framebuffer
	bool checkFramebufferStatus();

	// Flags para debug de OpenGL
	bool wireframe;
	bool culling;
	bool textures;

	// Tiempos
	Clock updateVSClock; // Reloj para Vertical Sync
	Clock updateClock; // Reloj para Draw
	int updateVSTime; // Tiempo de Vertical Sync
	int updateTime; // Tiempo de Draw

	// Materiales
	std::vector<float*> debugMaterials;

	// GUI ----------------------------------------------

	// MENUS
	bool isMenuState;

	// HUD
	GUISprite* character;
	GUISprite* expBar;
	GUIText* level;
	GUIText* exp;
	GUISprite* miniMap;

	GUIText* panelWave;
	GUIText* panelClock;
	GUIText* panelJade;

	GUIText* warningText;
	Clock warningTextClock;

	GUISprite* inventory;
	GUISprite* hpBar;
	GUISprite* shBar;
	GUIText* hpText;
	GUIText* shText;
	GUIButton* skill1;
	GUIButton* skill2;
	GUIButton* skill3;
	GUIButton* skill4;
	GUIButton* creation1;
	GUIButton* creation2;
	GUIButton* creation3;
	GUIButton* creation4;
	GUIText* description;

	std::wstring descriptions[8];
	// SETTINGS
	GUICheckbox* checkBoxFullScreen;
	GUICheckbox* checkBoxSync;
	GUICheckbox* checkBoxIrrlicht;
	GUICheckbox* checkBoxPhysics;
	GUICheckbox* checkBoxAliasing;
	GUICheckbox* checkBoxMusic;
	GUICheckbox* checkBoxReady;
	GUIButton* resolutionButton;
	GUIButton* begin;
	GUITable* tablePlayers;

	// SERVER BROWSER
	GUITable* tableServers;

	bool isFullScreenChecked()
	{
		return checkBoxFullScreen->isChecked();
	}

	bool isVSyncChecked()
	{
		return checkBoxSync->isChecked();
	}

	bool isIrrlichtChecked()
	{
		return checkBoxIrrlicht->isChecked();
	}

	bool isPhysicsChecked()
	{
		return checkBoxPhysics->isChecked();
	}

	bool isAliasingChecked()
	{
		return checkBoxAliasing->isChecked();
	}

	bool isMusicChecked()
	{
		return checkBoxMusic->isChecked();
	}

	std::wstring stringToWString(std::string str)
	{
		std::wstring wString;
		wString.assign(str.begin(), str.end());
		return wString;
	}

	// DEBUG
	GUIText* debugText;
	// Imagenes/rectangulos de los tiempos de cada update (para debug)
	GUISprite* gomRect;
	GUISprite* cmRect;
	GUISprite* geRect;
	GUISprite* dbgRect;
	GUISprite* vsRect;
	GUISprite* aifRect;
	// Agrega una Flecha en una posicion 2d, con un tamanyo 2d(ancho(x,z), alto(y)) y un color
	void addArrow( Vector2d position,float rotate, Vector2d size, int color, bool isMap);

	//Temporal, cuando se necesita para acceder desde fuera
	//Desactiva o Activa el dibujado de los cubos del mapa de fuerza
	void changeDrawCube(){};
	//Desactiva o Activa el dibujado de las flechas de fuerza
	void changeDrawArrow(){};

	// ---------------------------------------- Especificos de OpenGL
	// Inicializa varios parametros de OpenGL
	void initOpenGL();
	// Apila estados de OpenGL
	void pushGLStates();
	// Desapila los estados de OpenGL
	void popGLStates();
	// Render to texture
	void createFramebuffer();
	
	// ----------------------------------------
	void addTitle();
	void addHelp(const char* menu); // "MAIN", "SETTINGS", ...
	void setWindowsCaption();
	void drawGrid(float size, float separation);
	void drawDebugLineList();
	void createDebugText();
	void initializeDebugMaterials();
	void clearGUI();
	void updateWarningText();
};

