#pragma once

#include "GraphicsEngine.h"
#include "Clock.h"
#include "ShieldShaderCallBack.h"
#include "Shader.h"

class NodeMesh;
class NodeCamera;
class NodeText;
class EventManager;
class MapManager;
class GameSettings;
class FXEngineIrrlicht;
class CScreenQuadSceneNode;

class GraphicsEngineIrrlicht: public GraphicsEngine
{
public:
	GraphicsEngineIrrlicht(void);
	virtual~GraphicsEngineIrrlicht(void);

	bool createWindow(EventManager* eventManager, GameSettings* gameSettings);

	// MENUS -----------------------------------------

	// PANTALLAS
	// Inicializa las opciones comunes de todas las pantallas de menus
	void initMenuScreen();
	// Menu principal
	void createMenuMain();
	// Menu de settings
	void createMenuSettings();
	// Menu de buscar/seleccionar servidor
	void createMenuServerBrowser();
	// Menu de lobby (seleccion nave y confirmar)
	void createMenuClientLobby();
	// Menu del test de pathfinding (boton reiniciar mapa)
	void createMenuPathfinding();
	// Menu de creacion de partida/servidor
	void createMenuCreateGame();
	// Menu de creditos
	void createMenuCredits();
	// Menu de juego para continuar o salir de la partida
	void createMenuInGame();
	void toggleMenuInGame(); // Muestra/oculta el menu in game
	//Menu de Game Over
	void createMenuGameOver(int levelRound,Clock wastedTime,int enemiesKilled);
	//Menu seleccion de personaje waves
	void createMenuSelectCharacter();

	// ELEMENTOS DE MENUS
	// Boton comun
	void createMenuButton(int buttonNumber, const wchar_t* text, int id);
	// Boton de reconexion
	void addReconnectButton();
	// Boton de acceso a la pantalla de server browser
	void addServerBrowserButton();
	// Limpia la tabla de servidores
	void clearServerTable();
	// Crea la tabla de servidores
	void addServerToBrowserTable(ServerInfo serverInfo);
	// Actualiza la tabla de jugadores en el lobby
	void updateTablePlayers(std::vector<PlayerInfo> playersList);
	// Inicializa las opciones del menu de opciones
	void setMenuSettings();
	// Actualiza la resolucion del menu
	void updateResolution();

	void createInventory();

	void addInventoryLogo(int skill,const char* resource);
	// -----------------------------------------

	void draw(void);

	//Dibuja la gui sin tener que dibujar todo
	void drawGUI();
	void clear();
	void quit();

	Vector2d getMousePositionOnGround();

	bool isRunning()
	{
		return running;
	}

	FXEngine* getFXEngine()
	{
		return (FXEngine*)fxEngine;
	}

	NodeMesh* addNodeMesh(const char* meshFilename, bool transparent = false, int debugColor = DebugColor::RED , Vector2d position = Vector2d(-500, -500), float rotation = 0, float scale = 1);
	NodeCamera* addNodeCamera(Vector3d position = Vector3d(20, 50, -20), Vector3d target = Vector3d(0, 0, 0));
	NodeText* addNodeText();

	void changeNodeColor(NodeMesh* node, int color);

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

	// Para devolver el contenido del listBoxServers
	unsigned int getSelectedTableServers()
	{
		return tableServers->getSelected();
	}

	bool isChecked()
	{
		return checkBoxReady->isChecked();
	}

	bool isFullScreenChecked()
	{
		return checkBoxFullScreen->isChecked();
	}

	bool isVSyncChecked()
	{
		return checkBoxSync->isChecked();
	}

	virtual bool isIrrlichtChecked()
	{
		return checkBoxIrrlicht->isChecked();
	}

	virtual bool isPhysicsChecked()
	{
		return checkBoxPhysics->isChecked();
	}

	virtual bool isAliasingChecked()
	{
		return checkBoxAliasing->isChecked();
	}

	virtual bool isMusicChecked()
	{
		return checkBoxMusic->isChecked();
	}

	virtual bool isInventory()
	{
		if(inventory != NULL)
			return true;

		return false;
	}
	virtual bool canSeeTarget( Vector2d start, Vector2d end);

	virtual unsigned int getUpdateTime();
	virtual unsigned int getUpdateVSTime();
	virtual void drawUpdateTimesRectangles(UpdateTimes); 

	void createDebugMap(MapManager::CollisionMap &collisionMap);
	void putBegin(bool put);

	virtual void addDebugTimeRectangles();
	virtual void setUpdateTimeRectanglesVisible(bool);
	void setHudData(HUDData data);
	void createHudCharacter();
	void createHudMiniMap();
	void createHudTopBar();
	void createHUD();



	//Metodos para el miniMapa
	int addGUISprite(const char* spriteFilename, Vector2d position, Vector2d size);
	void setPositionGUISprite(int id,Vector2d position,Vector2d size);
	void removeGUISprite(int id);
	void setColorGUISprite(int id, int color);
	void setScaleGUISprite(int id, Vector2d scale);
	void setVisibleGUISprite(int id, bool visible);

	//Desactiva o Activa el dibujado de los cubos del mapa de fuerza
	void changeDrawCube();

	//Desactiva o Activa el dibujado de las flechas de fuerza
	void changeDrawArrow();

	//Descripcion de habilidades
	void addDescription(int id);
	void deleteDescription();
	void addDescriptionText(int skillNumber, std::wstring description);
	//Para shaders


	virtual Vector3d getCameraPosition()
	{	
		irr::core::vector3df position = sceneManager->getActiveCamera()->getAbsolutePosition();
		Vector3d pos = Vector3d(position.X,position.Y,position.Z);
		return pos;
	}

	virtual Vector3d getLightPosition()
	{
		/*irr::core::vector3df position = sceneManager->ligh
		Vector3d pos = Vector3d(position.X,position.Y,position.Z);*/
		return Vector3d(0,0,0);
	}
	
	//Devuelve material de la lista
	virtual int getMaterialFromList(int type)
	{
		return materialList[type];
	}

	//devuelve CallBcak del escudo

	ShieldShaderCallBack* setShieldCallBack()
	{
		return shieldCallBack;		
	}

	//Le epasamos el nodeMesh y el valor del shader que hemos leido.
	irr::s32 createMaterial( NodeMesh* nodeMesh, int shaderType,  bool transparent, float radius = 1);

	void setWarningMessage(const wchar_t* warningMessage); 
	
	//Modificar el valor del screen que hara los filros;
	void damageEffectScreenQuad( bool keep );
private:
	irr::gui::IGUIButton* createButton(int x, int y, int width, int height, int id, const wchar_t *text, irr::gui::IGUIElement* parent = NULL);

	void addTitle();
	void loadSkybox(Vector3d skyboxRotation = Vector3d());
	void setWindowsCaption();
	void drawGrid(float size, float separation);
	void drawDebugLineList();
	void createDebugText();
	void initializeDebugMaterials();
	void addParticleToMesh(const char* meshFilename, irr::scene::IMeshSceneNode* irrlichtNode);
	void updateTimeRectangleSize( int time, irr::gui::IGUIImage* rect );

	int lastFPS;
	Vector2d windowSize;

	int color;
	bool fullscreen;
	bool running;
	irr::video::SColor backgroundColor;
	irr::video::SMaterial defaultMaterial;

	FXEngineIrrlicht* fxEngine;

	irr::IrrlichtDevice* device;
	irr::video::IVideoDriver* driver;
	irr::scene::ISceneManager* sceneManager;
	irr::gui::IGUIEnvironment* guiEnviroment;
	irr::scene::ISceneCollisionManager *collisionManager;

	irr::gui::IGUIFont* menuFont;
	irr::gui::IGUIFont* debugFont;

	irr::gui::IGUIStaticText* debugText;
	irr::gui::IGUIElement* begin;
	std::vector<irr::video::SMaterial> debugMaterials;

	//Lista temporal que almacena las lineas a dibujar en ese frame, luego se vacia
	std::vector<DebugLine> debugLineList;

	irr::gui::IGUITable* tableServers;
	irr::gui::IGUITable* tablePlayers;
	irr::gui::IGUICheckBox *checkBoxReady;

	irr::gui::IGUICheckBox *checkBoxFullScreen;
	irr::gui::IGUICheckBox *checkBoxSync;
	irr::gui::IGUICheckBox *checkBoxIrrlicht;
	irr::gui::IGUICheckBox *checkBoxPhysics;
	irr::gui::IGUICheckBox *checkBoxAliasing;
	irr::gui::IGUICheckBox *checkBoxMusic;
	irr::gui::IGUIScrollBar* volumen;
	irr::gui::IGUIButton* resolutionButton;

	irr::scene::ISceneNode *mapRoot;
	irr::scene::ISceneNode *mapCube;
	irr::scene::ISceneNode *mapArrow;
	irr::scene::ISceneNode *skybox;

	irr::gui::IGUIImage* inventory;
	irr::gui::IGUIImage* hpBar;
	irr::gui::IGUIImage* shBar;
	irr::gui::IGUIStaticText* hpText;
	irr::gui::IGUIStaticText* shText;
	irr::gui::IGUIButton* skill1;
	irr::gui::IGUIButton* skill2;
	irr::gui::IGUIButton* skill3;
	irr::gui::IGUIButton* skill4;
	irr::gui::IGUIButton* creation1;
	irr::gui::IGUIButton* creation2;
	irr::gui::IGUIButton* creation3;
	irr::gui::IGUIButton* creation4;

	irr::gui::IGUIElement* inGameMenu;

	irr::gui::IGUIImage* miniMap;
	
	irr::gui::IGUIImage* topBar;
	irr::gui::IGUIStaticText* panelWave;
	irr::gui::IGUIStaticText* panelClock;
	irr::gui::IGUIStaticText* panelJade;

	irr::gui::IGUIStaticText* warningText;
	Clock warningTextClock;

	irr::gui::IGUIImage* character;
	irr::gui::IGUIImage* expBar;
	irr::gui::IGUIStaticText* level;
	irr::gui::IGUIStaticText* exp;
	
	bool isMenuState;

	Clock updateVSClockGEI;
	Clock updateClockGEI;
	int updateVSTime;
	int updateTime;
	
	irr::gui::IGUIStaticText* description;

	// Imagenes/rectangulos de los tiempos de cada update (para debug)
	irr::gui::IGUIImage* gomRect;
	irr::gui::IGUIImage* cmRect;
	irr::gui::IGUIImage* geRect;
	irr::gui::IGUIImage* dbgRect;
	irr::gui::IGUIImage* vsRect;
	irr::gui::IGUIImage* aifRect;
	// Agrega una Flecha en una posicion 2d, con un tamanyo 2d(ancho(x,z), alto(y)) y un color
	void addArrow( Vector2d position,float rotate, Vector2d size, int color, bool isMap);
	// Pone a null todas las variables de IGUI
	void clearIGUI();


	// Cosas para shaders
	
	//Leen un .obj, y accede al .mtl, de ahi coge el tipo de shader que necesita
	int readShader(const char* meshFilename );
	//lista de materiales
	std::vector<irr::s32> materialList;
	
	//recibe las urls y crea un shader que agrega a la lista de shaders(hay que seguir un orden);
	void addShader(const char* vertFilename,const char* fragFilename);
	//Lista de shader
	std::vector<Shader*>shaderList;
	std::wstring descriptions[8];
	
	ShieldShaderCallBack* shieldCallBack;

	//Esto es para probar
	int GraphicsEngineIrrlicht::readShaderTexture(const char* filename );
	irr::s32 createMaterialForShader( Shader* shader, irr::video::IShaderConstantSetCallBack* shaderCallback, bool transparent );
	void updateWarningText();
	
	//Probar filtros
	CScreenQuadSceneNode* screenQuad;
	irr::video::ITexture* mainTarget;
};
