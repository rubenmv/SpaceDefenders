#include "GraphicsEngineIrrlicht.h"
#include "GameSettings.h"
#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include <fstream>
#include "NodeMesh.h"
#include "NodeMeshIrrlicht.h"
#include "NodeCamera.h"
#include "NodeCameraIrrlicht.h"
#include "NodeText.h"
#include "NodeTextIrrlicht.h"

#include "ResourceTexture.h"
#include "ResourceManager.h"

#include "Vector3d.h"
#include "Math.h"

#include "GUIManager.h"
#include "GameObjectManager.h"
#include "GameManager.h"
#include "MapManager.h"
#include "EventManager.h"
#include "FXEngine.h"
#include "FXEngineIrrlicht.h"
#include "AIEngine.h"
#include "MinimapSprite.h" //se borrara cuando quitemos lo de dibujar paredes en el minimapa


//Probando shaders
#include "ShieldShaderCallBack.h"
#include "Shader.h"
#include "TerrainShaderCallBack.h"
#include "CScreenQuadSceneNode.h"
#include "FilterShaderCallBack.h"


using namespace GUI;

GraphicsEngineIrrlicht::GraphicsEngineIrrlicht(void)
{
	gameManager = GameManager::getInstance();

	backgroundColor = irr::video::SColor(255, 20, 20, 20);
	lastFPS = 0;
	device = NULL;
	driver = NULL;
	sceneManager = NULL;
	guiEnviroment = NULL;
	fxEngine = NULL;
	begin = NULL;
	initializeDebugMaterials();
	defaultMaterial = debugMaterials[DebugColor::RED];
	windowSize = Vector2d();
	isMenuState = false;

	nextID = 1000;
	updateTime=-1;	

	clearIGUI();
}


GraphicsEngineIrrlicht::~GraphicsEngineIrrlicht(void)
{
	device->drop();
	updateVSTime=0;

	delete fxEngine;
	fxEngine = NULL;

	gameManager = NULL;
}

void GraphicsEngineIrrlicht::drawGrid(float size, float separation)
{
	driver->setMaterial(defaultMaterial);
	driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);

	int gridLines = size / separation; // num de lineas a dibujar

	float ground = 0;

	for (int i = 0; i < gridLines; i++)
	{
		// Horizontales: se dibujan desde 0 a hacia -y (invertimos la i)
		driver->draw3DLine(irr::core::vector3df(0, ground, -i * separation), irr::core::vector3df(size, ground, -i * separation));
		// Verticales
		driver->draw3DLine(irr::core::vector3df(i*separation, ground, 0), irr::core::vector3df(i*separation, ground, -size));
	}
}

// Agrega un cube en una posicion 2d, con un tamanyo 2d(ancho(x,z), alto(y)) y un color
NodeMesh* GraphicsEngineIrrlicht::addCube( Vector2d position, Vector2d size, int color, bool isMap)
{
	// El tamanyo con el que se crea es el de los frames del grid del mapa
	irr::scene::IMeshSceneNode* cubeNode = sceneManager->addCubeSceneNode(GameManager::getInstance()->getMapManager()->collisionMap.frameSize);
	//cubeNode->setMaterialTexture(0, driver->getTexture("../assets/shaders/wall.bmp"));
	cubeNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	cubeNode->setMaterialFlag(irr::video::EMF_BLEND_OPERATION, true);
	//cubeNode->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
	//cubeNode->getMaterial(0).AmbientColor = debugMaterials[3].EmissiveColor;
	//cubeNode->setMaterialType((irr::video::E_MATERIAL_TYPE)materialList[0]);
	//cubeNode->getMesh()->setMaterialFlag(irr::video::EMF_GOURAUD_SHADING, false);
	NodeMesh* cubeNodeMesh = new NodeMeshIrrlicht(cubeNode);
	// Se redimensiona segun el tamanyo padre con el que se creo
	cubeNode->setScale(Vector3d(size.x, size.y, size.x));
	if(cubeNode)
	{
		cubeNode->setPosition(position);
	//	changeNodeColor(cubeNodeMesh, color);

	}

		if(isMap)
		{
			cubeNode->setParent(mapCube);
		}
//	}

	return cubeNodeMesh;
}

// Agrega una Flecha en una posicion 2d, con un tamanyo 2d(ancho(x,z), alto(y)) y un color
void GraphicsEngineIrrlicht::addArrow( Vector2d position,float rotate, Vector2d size, int color, bool isMap)
{
	// El tamanyo con el que se crea es el de los frames del grid del mapa
	irr::scene::IMesh* mesh = sceneManager->getMesh(Resources::MESH_ARROW);

	irr::scene::IMeshSceneNode* arrowNode = sceneManager->addMeshSceneNode(mesh);

	
	// Se redimensiona segun el tamanyo padre con el que se creo
	arrowNode->setScale(Vector3d(size.x, size.y, size.x));
	if(arrowNode)
	{
		arrowNode->setPosition(position);
	
		arrowNode->setRotation(Vector3d(0,rotate,0));
		
		arrowNode->getMaterial(0).AmbientColor = debugMaterials[color].EmissiveColor;

		if(isMap)
		{
			arrowNode->setParent(mapArrow);

		}
	}

	
}

void GraphicsEngineIrrlicht::changeDrawCube()
{

	mapCube->setVisible(!mapCube->isVisible());

}

void GraphicsEngineIrrlicht::changeDrawArrow()
{

mapArrow->setVisible(!mapArrow->isVisible());

}



bool GraphicsEngineIrrlicht::createWindow(EventManager* eventManager, GameSettings* gameSettings)
{
	this->gameSettings = gameSettings;

	irr::SIrrlichtCreationParameters parameters = irr::SIrrlichtCreationParameters();
	if(gameSettings->getAliasing())
	{
		parameters.AntiAlias = 16;
	}
	else
	{
		parameters.AntiAlias = 0;
	}
	parameters.DriverType = irr::video::EDT_OPENGL;
	parameters.Vsync = gameSettings->getSync();
	parameters.Fullscreen = gameSettings->getFullScreen();
	parameters.WindowSize = irr::core::dimension2du(gameSettings->getWidth(), gameSettings->getHeight());
	parameters.EventReceiver = eventManager;
	device = createDeviceEx(parameters);

	windowSize.x = gameSettings->getWidth();
	windowSize.y = gameSettings->getHeight();

	if (!device)
	{
		if(parameters.Fullscreen) //Si falla al arrancar en fullscreen, arranca con openGL
		{
			parameters.DriverType = irr::video::EDT_OPENGL;
			device = createDeviceEx(parameters);
			if(!device)
			{
				return false;
			}
		} else
		{
			return false;
		}		
	}

	running = device->run();

	driver = device->getVideoDriver();
	sceneManager = device->getSceneManager();
	guiEnviroment = device->getGUIEnvironment();
	collisionManager = sceneManager->getSceneCollisionManager();

	irr::gui::IGUISkin* skin = guiEnviroment->getSkin();
	
	//creamos shader y lo agregamos a la lista
	
	addShader(Resources::SHADER_SHIELD_VERT,Resources::SHADER_SHIELD_FRAG);
	addShader(Resources::SHADER_TERRAIN_VERT,Resources::SHADER_TERRAIN_FRAG);


	/*-----------------------------------*/

	// Motor de efectos
	fxEngine = new FXEngineIrrlicht(sceneManager, driver);
	
	//Creamos las texturas
	mainTarget = driver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(windowSize.x,windowSize.y),"mainTarget");

	//Creamos el quad
	screenQuad = new CScreenQuadSceneNode(sceneManager->getRootSceneNode(),sceneManager,10);

	
	/*----Asignamos texturas--------*/
	screenQuad->getMaterial(0).setTexture(0,mainTarget);

	/*--------------------------------------*/


	debugFont = guiEnviroment->getFont("../assets/fonts/fontcourier.bmp");
	menuFont = guiEnviroment->getFont("../assets/fonts/OpenSans.xml");

	
	if (debugFont)
	{
		skin->setFont(debugFont);
		skin->setColor(irr::gui::EGDC_BUTTON_TEXT, irr::video::SColor(255,0,0,0));
	}
	skin->setFont(menuFont, irr::gui::EGDF_BUTTON);

	loadSkybox();

	guiEnviroment->loadGUI("../assets/images/gui/guiPlantilla.xml");

	//skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);
	createDebugText();

	return true;
}

void GraphicsEngineIrrlicht::draw(void)
{
	updateClockGEI.start(); // Inicializamos el reloj

	updateWarningText();

	running = device->run();
	if(running)
	{
		if(!device->isWindowActive())
		{
			//device->yield(); // Libera irrlicht del procesador por un frame
			//return;		   // Hace falta probarlo primero y que no rompa nada	
		}
		driver->beginScene(true, true, 0);

		/*--------------Filter------------------------*/
		driver->setRenderTarget(mainTarget,true,true,irr::video::SColor(255,255,255,255));
		/*--------------------------------------------*/
		sceneManager->drawAll();
		
		drawDebugLineList();


// 		if(!isMenuState)
// 		{
// 			float gridSize = gameManager->getMapManager()->collisionMap.mapSize;
// 			float lineSeparation = gameManager->getMapManager()->collisionMap.frameSize;
// 
// 			drawGrid(gridSize, lineSeparation);
// 		}

		guiEnviroment->drawAll();
		
		updateTime=updateClockGEI.getElapsedTime();
		updateVSClockGEI.start();


		/*--------------Filter------------------------*/
		irr::video::SColor colors[] =
		{
			irr::video::SColor(255,255,255,255),
			irr::video::SColor(255,255,255,255),
			irr::video::SColor(255,255,255,255),
			irr::video::SColor(255,255,255,255)
		};

		//driver->setRenderTarget(rtt0,true,true,irr::video::SColor(255,0,0,0));
		driver->draw2DImage(mainTarget,irr::core::rect<irr::s32>(0,0,windowSize.x,windowSize.y),irr::core::rect<irr::s32>(0,0,windowSize.x,windowSize.y),0,colors);
		driver->setRenderTarget(irr::video::ERT_FRAME_BUFFER,true,true);
		
		screenQuad->render();

		/*----------------------------------------*/

		driver->endScene();

		setWindowsCaption();

	}

	updateVSTime=updateVSClockGEI.getElapsedTime();
}

void GraphicsEngineIrrlicht::drawGUI()
{

	
	driver->beginScene(true, true, 0);
//	sceneManager->drawAll();	

	guiEnviroment->drawAll();
	
	driver->endScene();

	setWindowsCaption();


}

void GraphicsEngineIrrlicht::loadSkybox(Vector3d skyboxRotation)
{
	skybox = sceneManager->addSkyBoxSceneNode(
		driver->getTexture("../assets/images/skybox/nebula_top.jpg"),
		driver->getTexture("../assets/images/skybox/nebula_bottom.jpg"),
		driver->getTexture("../assets/images/skybox/nebula_left.jpg"),
		driver->getTexture("../assets/images/skybox/nebula_right.jpg"),
		driver->getTexture("../assets/images/skybox/nebula_front.jpg"),
		driver->getTexture("../assets/images/skybox/nebula_back.jpg"));

	irr::scene::ISceneNodeAnimator* ani=sceneManager->createRotationAnimator(irr::core::vector3df(0.002,0,0.002));
	skybox->setRotation(irr::core::vector3df(skyboxRotation.x, skyboxRotation.y, skyboxRotation.z));
	skybox->addAnimator(ani);
	ani->drop();
}

void GraphicsEngineIrrlicht::setWindowsCaption()
{
	int fps = driver->getFPS();
	if (lastFPS != fps)
	{
		irr::core::stringw str = L"Space Defenders FPS:";
		str += fps;

		device->setWindowCaption(str.c_str());
		lastFPS = fps;
	}
}

NodeMesh* GraphicsEngineIrrlicht::addNodeMesh(const char* meshFilename,bool transparent, int debugColor, Vector2d position , float rotation, float scale)
{
	irr::scene::IAnimatedMesh* mesh = sceneManager->getMesh(meshFilename);
	if(!mesh)
	{
		std::cerr << "Error: No se ha encontrado la malla en:" << device->getFileSystem()->getAbsolutePath(meshFilename).c_str() << "\n";
		return NULL;
	}
	
	Vector3d rotationV3(0, rotation, 0);
	Vector3d scaleV3(scale, scale, scale);

	irr::scene::IMeshSceneNode* irrlichtNode = sceneManager->addMeshSceneNode(mesh, 0, -1, position.asVector3d(), rotationV3, scaleV3);


	if(transparent)
	{
		irrlichtNode->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
		irrlichtNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	}

	addParticleToMesh(meshFilename, irrlichtNode); 

	NodeMesh* nodeMesh = new NodeMeshIrrlicht(irrlichtNode); 

	int shader = readShader(meshFilename);

	if(shader >=  0)
	{
		//irrlichtNode->setMaterialTexture(0, driver->getTexture("../assets/shaders/shield.png"));
		
	    irr::s32 material = createMaterial(nodeMesh, shader, transparent,scale); //Por ahora uso la posicion solo en el impacto, y es la posicion del projectile
		irrlichtNode->setMaterialType((irr::video::E_MATERIAL_TYPE)material);
		
		switch(shader)
		{
		case Material::SHIELD:
		irrlichtNode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false); //PAra el escudo
			irrlichtNode->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
			break;
		case  Material::TERRAIN:
			
			break;
		}		
		
	}
	
	return(nodeMesh);
}

NodeCamera* GraphicsEngineIrrlicht::addNodeCamera(Vector3d position , Vector3d target)
{
	irr::scene::ICameraSceneNode* irrlichtNode = sceneManager->addCameraSceneNode(NULL, position, target);

	NodeCamera* nodeCamera = new NodeCameraIrrlicht(irrlichtNode);

	return(nodeCamera);
}


NodeText* GraphicsEngineIrrlicht::addNodeText()
{
	irr::gui::IGUIStaticText* irrlichtText = guiEnviroment->addStaticText(L"", irr::core::recti(0,0, 400, 250));
	
	//irrlichtText->setOverrideColor(irr::video::SColor(255,255,255,255));
	NodeText* nodeText = new NodeTextIrrlicht(irrlichtText);
	return nodeText;
}


void GraphicsEngineIrrlicht::createDebugText()
{
	debugText = guiEnviroment->addStaticText(L"", irr::core::recti(20,60, 700, 500));
	debugText->setOverrideColor(irr::video::SColor(255,255,255,255));
}

void GraphicsEngineIrrlicht::initWorld()
{
	irr::scene::ILightSceneNode* irrlichtLight = sceneManager->addLightSceneNode(NULL, Vector3d(0,0,0), irr::video::SColor(255,200,200,200));
	irrlichtLight->getLightData().Type = irr::video::ELT_DIRECTIONAL;
	irrlichtLight->setRotation(Vector3d(45, 180, 0));

	sceneManager->setAmbientLight(irr::video::SColor(255,100,110,120));
	createHUD();
}

// ------------- MENUS --------------

//Cuidado que este pide el ancho y la altura en vez de las esquinas
irr::gui::IGUIButton* GraphicsEngineIrrlicht::createButton(int x, int y, int width, int height, int id, const wchar_t* text, irr::gui::IGUIElement* parent)
{
	irr::gui::IGUIButton* button = guiEnviroment->addButton(irr::core::rect<irr::s32>(x , y, x + width, y + height), parent, id, text);

	button->setUseAlphaChannel(true);
	button->setImage(driver->getTexture(GUI::MENU_IMAGE_BUTTON_NORMAL));
	button->setPressedImage(driver->getTexture(GUI::MENU_IMAGE_BUTTON_HOVER));

	return button;
}


void GraphicsEngineIrrlicht::createMenuButton(int buttonNumber, const wchar_t* text, int id)
{
	int maxButtons = GUI::MENU_MAX_BUTTONS;
	int columns = GUI::MENU_COLUMNS;
	int buttonHeight = GUI::MENU_BUTTON_HEIGHT;
	int buttonWidth = GUI::MENU_BUTTON_WIDTH;
	int buttonPosition = buttonNumber;

	// Reinicia el contador
	if (buttonNumber > (maxButtons - 1) )
	{
		buttonPosition -= maxButtons;
	}
	//El centro de la pantalla menos la mitad de lo que ocupan todas las columnas
	int leftPosition = buttonPosition/3 * (buttonWidth + 50) + windowSize.x / 2 - (buttonWidth*columns + 50 * (columns - 1))/2; 
	int rightPosition = leftPosition + buttonWidth;
	int startTopPosition = windowSize.y/2-112;
	int space = buttonHeight + 15;

	int topPosition = startTopPosition + space * (buttonNumber % 3);
	int bottomPosition = topPosition + buttonHeight;

	irr::gui::IGUIButton *button;

	button = guiEnviroment->addButton(irr::core::rect<irr::s32>(leftPosition, topPosition, rightPosition, bottomPosition),							
		0, id, text);

	button->setUseAlphaChannel(true);
	button->setImage(driver->getTexture(GUI::MENU_IMAGE_BUTTON_NORMAL));
	button->setPressedImage(driver->getTexture(GUI::MENU_IMAGE_BUTTON_HOVER));
	button->setDrawBorder(false);

}


void GraphicsEngineIrrlicht::initMenuScreen()
{
	clear(); // Limpia la pantalla
	addTitle();

	irr::gui::IGUIImage* help = guiEnviroment->addImage(irr::core::rect<irr::s32>(windowSize.x/2 - 430,windowSize.y/2+150, windowSize.x/2 + 430, windowSize.y/2+150 + 100));
	help->setImage(driver->getTexture(GUI::MENU_IMAGE_HELP_MAIN));
}

void GraphicsEngineIrrlicht::setMenuSettings()
{
	irr::scene::ICameraSceneNode *camera = sceneManager->addCameraSceneNodeMaya(NULL, -500);
	camera->setFOV(60 * Math::DEGTORAD);

	isMenuState = true;
	irr::gui::IGUISkin* skin = guiEnviroment->getSkin();

	if (menuFont)
	{
		//skin->setFont(menuFont);
		skin->setColor(irr::gui::EGDC_BUTTON_TEXT, irr::video::SColor(255,255,255,255));
	}
}

void GraphicsEngineIrrlicht::createMenuSettings()
{
	addTitle();
	int buttonHeight = GUI::MENU_BUTTON_HEIGHT;
	int buttonWidth = GUI::MENU_BUTTON_WIDTH;
	irr::gui::IGUIImage* help = guiEnviroment->addImage(irr::core::rect<irr::s32>(windowSize.x/2 - 430,windowSize.y/2+150, windowSize.x/2 + 430, windowSize.y/2+150 + 100));
	help->setImage(driver->getTexture(GUI::MENU_IMAGE_HELP_SETTINGS));

	float vertical = windowSize.y/2+60-150;
	float horizontal = 100;

	checkBoxFullScreen = guiEnviroment->addCheckBox(gameManager->getGameSettings()->getFullScreen(),irr::core::rect<irr::s32>(horizontal + windowSize.x/2 - 355,vertical + 20,horizontal + windowSize.x/2 - 355+buttonWidth/2,vertical+20+buttonHeight/3), NULL, BUTTON_ID::FULLSCREEN, L"FULLSCREEN");
	checkBoxSync = guiEnviroment->addCheckBox(gameManager->getGameSettings()->getSync(),irr::core::rect<irr::s32>(horizontal + windowSize.x/2 + 45,vertical + 20,horizontal + windowSize.x/2 + 45+buttonWidth/2,vertical+20+buttonHeight/3), NULL, BUTTON_ID::V_SYNC, L"VSYNC");
	checkBoxIrrlicht =  guiEnviroment->addCheckBox(gameManager->getGameSettings()->isIrrlicht(),irr::core::rect<irr::s32>(horizontal + windowSize.x/2 - 355,vertical+90,horizontal + windowSize.x/2 - 355+buttonWidth/2,vertical+90+buttonHeight/3), NULL, BUTTON_ID::IRRLICHT, L"Use Irrlicht");
	checkBoxPhysics =  guiEnviroment->addCheckBox(gameManager->getGameSettings()->usePhysics(),irr::core::rect<irr::s32>(horizontal + windowSize.x/2 + 45,vertical+90,horizontal + windowSize.x/2 + 45+buttonWidth/2,vertical+90+buttonHeight/3), NULL, BUTTON_ID::PHYSICS, L"Use Physics");
	checkBoxAliasing =  guiEnviroment->addCheckBox(gameManager->getGameSettings()->useAliasing(),irr::core::rect<irr::s32>(horizontal + windowSize.x/2 - 355,vertical+55,horizontal + windowSize.x/2 - 355+buttonWidth/2,vertical+55+buttonHeight/3), NULL, BUTTON_ID::ANTIALIASING, L"Use Antialiasing");
	checkBoxMusic = guiEnviroment->addCheckBox(gameManager->getGameSettings()->getVolumen(),irr::core::rect<irr::s32>(horizontal + windowSize.x/2 + 45,vertical+55,horizontal + windowSize.x/2 + 45+buttonWidth/2,vertical+55+buttonHeight/3), NULL, BUTTON_ID::MUSIC, L"Music");
		
	vertical = windowSize.y/2+95-250;

	irr::gui::IGUIButton* button = guiEnviroment->addButton(irr::core::rect<irr::s32>(windowSize.x/2-177 -70, vertical,windowSize.x/2-177 - 5,vertical+buttonHeight), NULL, BUTTON_ID::RESOLUTION_BEFORE, L"");
	button->setUseAlphaChannel(true);
	button->setImage(driver->getTexture(GUI::MENU_IMAGE_ARROW_LEFT));
	button->setPressedImage(driver->getTexture(GUI::MENU_IMAGE_ARROW_LEFT_PRESSED));
	irr::gui::IGUIButton* button2 = guiEnviroment->addButton(irr::core::rect<irr::s32>(windowSize.x/2 + 177 + 5, vertical,windowSize.x/2 + 177 + 70,vertical+buttonHeight), NULL, BUTTON_ID::RESOLUTION_NEXT, L"");
	button2->setUseAlphaChannel(true);
	button2->setImage(driver->getTexture(GUI::MENU_IMAGE_ARROW_RIGHT));
	button2->setPressedImage(driver->getTexture(GUI::MENU_IMAGE_ARROW_RIGHT_PRESSED));

	resolutionButton = createButton(windowSize.x/2-170, windowSize.y/2+95-250, buttonWidth, buttonHeight,  BUTTON_ID::RESOLUTION_NEXT, GameManager::getInstance()->getGameSettings()->getResolution().str().c_str());

	createMenuButton(2, L"BACK", BUTTON_ID::SETTINGS_BACK);
	createMenuButton(5, L"APPLY", BUTTON_ID::APPLY);

	/*volumen = guiEnviroment->addScrollBar(true,irr::core::rect<irr::s32>(280,350, 280 + 714, 550 + 100),NULL,BUTTON_ID::VOLUMEN);
	volumen->setMax(100);
	volumen->setPos(gameSettings->getVolumen());*/
}

void GraphicsEngineIrrlicht::createMenuCredits()
{
	addTitle();

	irr::gui::IGUIImage* help = guiEnviroment->addImage(irr::core::rect<irr::s32>(windowSize.x/2 - 430,windowSize.y/2+150, windowSize.x/2 + 430, windowSize.y/2+150 + 100));
	help->setImage(driver->getTexture(GUI::MENU_IMAGE_HELP_MAIN));

	irr::gui::IGUIStaticText* warningText = guiEnviroment->addStaticText(L"Space Defenders 2013-2014\nMartin Malaga, Alejandro; Garcia Cusso, Fernando\nGarcia Guilabert, Kevin; Martinez Vilar, Ruben\nRamírez Bernabeu, Oscar",
		irr::core::rect<irr::s32>(windowSize.x/2 - 350,windowSize.y/2 - 150,windowSize.x,windowSize.y/2+40) );
	warningText->setOverrideFont(guiEnviroment->getFont("../assets/fonts/OpenSans.xml"));

	createMenuButton(2, L"BACK",  BUTTON_ID::BACK);
}

void GraphicsEngineIrrlicht::createHUD()
{
	createHudCharacter();
	createHudMiniMap();
	createHudTopBar();
	createInventory();
}

void GraphicsEngineIrrlicht::createHudCharacter()
{
	character = NULL;
	switch(gameManager->getGameObjectManager()->getMainPlayer()->getName())
	{
	case GameObject::N_BATTLESHIP_PLAYER:
		guiEnviroment->addImage(driver->getTexture( GUI::HUD_IMAGE_ICON_BATTLESHIP ),irr::core::vector2di(0,windowSize.y-210),true,character);
		break;
	case GameObject::N_BOMBER_PLAYER:
		guiEnviroment->addImage(driver->getTexture( GUI::HUD_IMAGE_ICON_BOMBER ),irr::core::vector2di(0,windowSize.y-210),true,character);
		break;
	case GameObject::N_FIGHTER_PLAYER:
	guiEnviroment->addImage(driver->getTexture( GUI::HUD_IMAGE_ICON_FIGHTER ),irr::core::vector2di(0,windowSize.y-210),true,character);
		break;
	case GameObject::N_ENGINEER_PLAYER:
		guiEnviroment->addImage(driver->getTexture( GUI::HUD_IMAGE_ICON_ENGINEER ),irr::core::vector2di(0,windowSize.y-210),true,character);
		break;
	}
	
	expBar = guiEnviroment->addImage(driver->getTexture(GUI::HUD_IMAGE_EXPBAR),irr::core::vector2di(32,windowSize.y-16),true,NULL);
	expBar->setScaleImage(true);
	character = guiEnviroment->addImage(driver->getTexture(GUI::HUD_IMAGE_PJ),irr::core::vector2di(0,windowSize.y-210),true,NULL);
	level = guiEnviroment->addStaticText(L"",irr::core::rect<irr::s32>(11,windowSize.y-21,40,windowSize.y));
	exp = guiEnviroment->addStaticText(L"",irr::core::rect<irr::s32>(50,windowSize.y-20,200,windowSize.y));

	warningText =  guiEnviroment->addStaticText(L"",irr::core::rect<irr::s32>(windowSize.x/2-100, windowSize.y - 200, windowSize.x/2 + 250,windowSize.y-150));
	warningText->setOverrideFont(guiEnviroment->getFont("../assets/fonts/titillium.png"));
}

void GraphicsEngineIrrlicht::createHudMiniMap()
{
	miniMap = guiEnviroment->addImage(driver->getTexture(GUI::HUD_IMAGE_MINIMAP),irr::core::vector2di(windowSize.x-208,windowSize.y-200),true,NULL);
}

void GraphicsEngineIrrlicht::createHudTopBar()
{
	//topBar = guiEnviroment->addImage(driver->getTexture("../assets/images/hud/BarraTop.png"),irr::core::vector2di(0,0),true,NULL); 
	
	guiEnviroment->addImage(driver->getTexture(GUI::HUD_IMAGE_PANEL),irr::core::vector2di(windowSize.x/2-158,2),true,NULL,-1,L""); //centro	
	
	
	panelClock = guiEnviroment->addStaticText(L"",irr::core::rect<irr::s32>(windowSize.x/2 - 30 - 120, 8 + 17, windowSize.x/2 + 30 - 120, 32 + 17));
	panelClock->setOverrideFont(guiEnviroment->getFont("../assets/fonts/titillium-Bold.png"));
	panelClock->setTextRestrainedInside(false);

	panelJade = guiEnviroment->addStaticText(L"",irr::core::rect<irr::s32>(windowSize.x/2 - 95 ,8 + 20, windowSize.x/2 + 125 - 95, 32 + 10));
	panelJade->setOverrideFont(guiEnviroment->getFont("../assets/fonts/titillium-Bold.png"));
	panelJade->setTextRestrainedInside(false);
	panelJade->setTextAlignment(irr::gui::EGUIA_LOWERRIGHT,irr::gui::EGUIA_CENTER);


	panelWave = guiEnviroment->addStaticText(L"",irr::core::rect<irr::s32>(windowSize.x/2 + 95, 8 + 10,windowSize.x/2 + 60 + 95, 32 + 10)); 
	panelWave->setOverrideFont(guiEnviroment->getFont("../assets/fonts/titillium-Bold.png"));
	panelWave->setTextRestrainedInside(false);

}

void GraphicsEngineIrrlicht::createInventory()
{
	inventory = guiEnviroment->addImage(driver->getTexture(GUI::HUD_IMAGE_INVENTORY),irr::core::vector2di(windowSize.x/2-290,windowSize.y-150),true,NULL);
	
	//cambio vida y escudo
	irr::gui::IGUIImage *hpBarEmpty, *shBarEmpty; //barras de vida y escudo vacias (las declaro aqui porque no se van a tocar en ningun lado)
	hpBarEmpty = guiEnviroment->addImage(driver->getTexture(GUI::HUD_IMAGE_HP_EMPTY),irr::core::vector2di(windowSize.x/2-160,25),true,NULL);
	shBarEmpty = guiEnviroment->addImage(driver->getTexture(GUI::HUD_IMAGE_SH_EMPTY),irr::core::vector2di(windowSize.x/2-5,25),true,NULL);
	

	hpBar = guiEnviroment->addImage(driver->getTexture(GUI::HUD_IMAGE_HP),irr::core::vector2di(windowSize.x/2-160,25),true,NULL);
	shBar = guiEnviroment->addImage(driver->getTexture(GUI::HUD_IMAGE_SH),irr::core::vector2di(windowSize.x/2-5,25),true,NULL);
	hpBar->setScaleImage(false);
	shBar->setScaleImage(false);	
	
	shText = guiEnviroment->addStaticText(L"",irr::core::rect<irr::s32>(windowSize.x/2+270-180, 20+60, windowSize.x/2+400-180, 35+60),false,true,NULL);
	hpText = guiEnviroment->addStaticText(L"",irr::core::rect<irr::s32>(windowSize.x/2+270-430, 20+60, windowSize.x/2+400-430, 35+60),false,true,NULL);
	

	creation1  = guiEnviroment->addButton(irr::core::rect<irr::s32>(30, 62, 80, 112),inventory,BUTTON_ID::SKILL1,L"1");
	creation1->setNotClipped(true);
	creation2 = guiEnviroment->addButton(irr::core::rect<irr::s32>(92, 62, 142, 112),inventory,BUTTON_ID::SKILL2,L"2");
	creation2->setNotClipped(true);
	creation3  = guiEnviroment->addButton(irr::core::rect<irr::s32>(154, 62, 204, 112),inventory,BUTTON_ID::SKILL3,L"3");
	creation3->setNotClipped(true);
	creation4 = guiEnviroment->addButton(irr::core::rect<irr::s32>(214, 62, 265, 112),inventory,BUTTON_ID::SKILL4,L"4");
	creation4->setNotClipped(true);
	skill1 = guiEnviroment->addButton(irr::core::rect<irr::s32>(317, 62, 367, 112),inventory,BUTTON_ID::SKILL5,L"5");
	skill1->setNotClipped(true);
	skill2 = guiEnviroment->addButton(irr::core::rect<irr::s32>(379, 62, 429, 112),inventory,BUTTON_ID::SKILL6,L"6");
	skill2->setNotClipped(true);
	skill3 = guiEnviroment->addButton(irr::core::rect<irr::s32>(441, 62, 491, 112),inventory,BUTTON_ID::SKILL7,L"7");
	skill3->setNotClipped(true);
	skill4 = guiEnviroment->addButton(irr::core::rect<irr::s32>(501, 62, 552, 112),inventory,BUTTON_ID::SKILL8,L"8");
	skill4->setNotClipped(true);
}

void GraphicsEngineIrrlicht::addDescriptionText(int skillNumber, std::wstring description)
{
	descriptions[skillNumber] = description;
}

void GraphicsEngineIrrlicht::addInventoryLogo(int skill,const char* resource)
{
	switch (skill)
	{
	case 0:
		creation1->setImage(driver->getTexture(resource));
		break;	
	case 1:
		creation2->setImage(driver->getTexture(resource));
		break;	
	case 2:
		creation3->setImage(driver->getTexture(resource));
		break;	
	case 3:
		creation4->setImage(driver->getTexture(resource));
		break;	
	case 4:
		skill1->setImage(driver->getTexture(resource));
		break;	
	case 5:
		skill2->setImage(driver->getTexture(resource));
		break;
	case 6:
		skill3->setImage(driver->getTexture(resource));
		break;
	case 7:
		skill4->setImage(driver->getTexture(resource));
		break;
	}
}

void GraphicsEngineIrrlicht::deleteDescription()
{
	if(description != NULL)
	{
		description->remove();
		description = NULL;
	}
}

void GraphicsEngineIrrlicht::addDescription(int id)
{
	switch (id)
	{	
	case BUTTON_ID::SKILL1:
		description = guiEnviroment->addStaticText(descriptions[0].c_str(), irr::core::recti(30,115, 1500, 250),false,true,inventory);
		break;
	case BUTTON_ID::SKILL2:
		description = guiEnviroment->addStaticText(descriptions[1].c_str(), irr::core::recti(92,115, 1500, 250),false,true,inventory);
		break;
	case BUTTON_ID::SKILL3:
		description = guiEnviroment->addStaticText(descriptions[2].c_str(), irr::core::recti(154,115, 1500, 250),false,true,inventory);
		break;
	case BUTTON_ID::SKILL4:
		description = guiEnviroment->addStaticText(descriptions[3].c_str(), irr::core::recti(214,115, 1500, 250),false,true,inventory);
		break;
	case BUTTON_ID::SKILL5:
		description = guiEnviroment->addStaticText(descriptions[4].c_str(), irr::core::recti(317,115, 1500, 250),false,true,inventory);
		break;
	case BUTTON_ID::SKILL6:
		description = guiEnviroment->addStaticText(descriptions[5].c_str(), irr::core::recti(379,115, 1500, 250),false,true,inventory);
		break;
	case BUTTON_ID::SKILL7:
		description = guiEnviroment->addStaticText(descriptions[6].c_str(), irr::core::recti(441,115, 1500, 250),false,true,inventory);
		break;
	case BUTTON_ID::SKILL8:
		description = guiEnviroment->addStaticText(descriptions[7].c_str(), irr::core::recti(441,115, 1500, 250),false,true,inventory);
		break;
	}
}


void GraphicsEngineIrrlicht::setHudData(HUDData data)
{

	std::wstring aux = std::to_wstring(data.waves);
	panelWave->setText(aux.c_str());
	aux = std::to_wstring(data.clock.getElapsedTime()/60000);
	aux.append(L":");
	aux.append(std::to_wstring((data.clock.getElapsedTime()/1000)%60));
	panelClock->setText(aux.c_str());
	aux = std::to_wstring(data.jade);
	panelJade->setText(aux.c_str());
	aux = std::to_wstring(data.level);
	level->setText(aux.c_str());
	std::wstring aux2 = std::to_wstring(data.nextExp);
	aux = std::to_wstring(data.exp);
	aux.append(L"/");
	aux.append(aux2);
	exp->setText(aux.c_str());

	aux = std::to_wstring((int)data.health.x);
	aux.append(L"/");
	aux.append(std::to_wstring((int)data.health.y));
	hpText->setText(aux.c_str());



	aux = std::to_wstring((int)data.shield.x);
	aux.append(L"/");
	aux.append(std::to_wstring((int)data.shield.y));
	shText->setText(aux.c_str());
	
	expBar->setMaxSize(irr::core::dimension2du(110*(float)data.exp/(float)data.nextExp,10));

	if(data.health.x > 0)
	{
		if(!hpBar->isVisible())
		{
			hpBar->setVisible(true);
		}
		//hpBar->setMaxSize(irr::core::dimension2du(545*data.health.x/data.health.y,20));
		unsigned int length = Math::max_(151*data.health.x/data.health.y, 1);
		hpBar->setMaxSize(irr::core::dimension2du(length,65));
	}
	else
	{
		hpBar->setVisible(false);
	}
	if(data.shield.x > 0)
	{
		if(!shBar->isVisible())
		{
			shBar->setVisible(true);
		}
		
		unsigned int length = Math::max_(151*data.shield.x/data.shield.y, 1);
		shBar->setMaxSize(irr::core::dimension2du(length,65));
	}
	else
	{
		shBar->setVisible(false);
	}

	/*//if(data.jade < _wtoi(creation1->getText()))
	if(!data.skills[0])
	{
		if(creation1->isEnabled()) 
		{
			creation1->setEnabled(false);
	
		}
	}
	else
	{
		if(!creation1->isEnabled())
		{
			creation1->setEnabled(true);
	
		}
	}
	//if(data.jade < _wtoi(creation2->getText()) || !(data.level >= 2))
	if(!data.skills[1])
	{
		if(creation2->isEnabled())
		{
			creation2->setEnabled(false);
	
		}
	}
	else
	{
		if(!creation2->isEnabled())
		{
			creation2->setEnabled(true);
	
		}
	}
	//if(data.jade < _wtoi(creation3->getText()) || !(data.level >= 4))
	if(!data.skills[2])
	{
		if(creation3->isEnabled())
		{
			creation3->setEnabled(false);
		}
	}
	else
	{
		if(!creation3->isEnabled())
		{	
			creation3->setEnabled(true);
		}
	}
	//if(data.jade < _wtoi(creation4->getText()) || !(data.level >= 6))
	if(!data.skills[3])
	{
		if(creation4->isEnabled())
		{	
			creation4->setEnabled(false);
		} 
	}
	else
	{
		if(!creation4->isEnabled())
		{
			creation4->setEnabled(true);
		}
	}

	if(!data.skills[4])
	{
		if(skill1->isEnabled())
		{	
			skill1->setEnabled(false);
		} 
	}
	else
	{
		if(!skill1->isEnabled())
		{
			skill1->setEnabled(true);
		}
	}

	if(!data.skills[5])
	{
		if(skill2->isEnabled())
		{	
			skill2->setEnabled(false);
		} 
	}
	else
	{
		if(!skill2->isEnabled())
		{
			skill2->setEnabled(true);
		}
	}

	if(!data.skills[6])
	{
		if(skill3->isEnabled())
		{	
			skill3->setEnabled(false);
		} 
	}
	else
	{
		if(!skill3->isEnabled())
		{
			skill3->setEnabled(true);
		}
	}

	if(!data.skills[7])
	{
		if(skill4->isEnabled())
		{	
			skill4->setEnabled(false);
		} 
	}
	else
	{
		if(!skill4->isEnabled())
		{
			skill4->setEnabled(true);
		}
	}*/
}
	

void GraphicsEngineIrrlicht::addTitle()
{
	irr::gui::IGUIImage* title = guiEnviroment->addImage(irr::core::rect<irr::s32>(windowSize.x/2 - 220,windowSize.y/2-287, windowSize.x/2 - 226 + 580, windowSize.y/2-287 + 126));//420 para estar alineada a la derecha
	title->setImage(driver->getTexture(GUI::IMAGE_TITLE));

}

void GraphicsEngineIrrlicht::createMenuMain()
{
	addTitle();

	irr::gui::IGUIImage* help = guiEnviroment->addImage(irr::core::rect<irr::s32>(windowSize.x/2 - 430,windowSize.y/2+150, windowSize.x/2 + 430, windowSize.y/2+150 + 100));
	help->setImage(driver->getTexture(GUI::MENU_IMAGE_HELP_MAIN));

	createMenuButton(0, L"SINGLE PLAYER", BUTTON_ID::WAVES);
	createMenuButton(1, L"SETTINGS", BUTTON_ID::SETTINGS);
	createMenuButton(2, L"CREDITS", BUTTON_ID::CREDITS);
	createMenuButton(5, L"EXIT", BUTTON_ID::EXIT);

	/*
	createMenuButton(0, L"EXIT",BUTTON_ID::EXIT);
	createMenuButton(1, L"OLD TESTS",BUTTON_ID::OLD_TESTS);
	createMenuButton(2, L"SETTINGS",BUTTON_ID::SETTINGS);
	createMenuButton(3, L"WAVES",BUTTON_ID::WAVES);
	createMenuButton(4, L"BENCHMARK",BUTTON_ID::SQUADS);
	*/
}

void GraphicsEngineIrrlicht::createMenuGameOver(int levelRound,Clock wastedTime,int enemiesKilled)
{
	addTitle();

	irr::gui::IGUIImage* help = guiEnviroment->addImage(irr::core::rect<irr::s32>(windowSize.x/2 - 430,windowSize.y/2+150, windowSize.x/2 + 430, windowSize.y/2+150 + 100));
	help->setImage(driver->getTexture(GUI::MENU_IMAGE_HELP_MAIN));
	std::wstring aux(L"WAVES: ");
	
	irr::gui::IGUIStaticText* waves = guiEnviroment->addStaticText(L"",irr::core::rect<irr::s32>(windowSize.x/2 - 350,windowSize.y/2 - 130,windowSize.x,windowSize.y/2+40)); 
	waves->setOverrideFont(guiEnviroment->getFont("../assets/fonts/titillium-Bold.png"));
	aux.append(std::to_wstring(levelRound));
	waves->setText(aux.c_str());
	
	aux = (L"KILLS: ");
	irr::gui::IGUIStaticText* totalEnemiesKilled = guiEnviroment->addStaticText(L"",irr::core::rect<irr::s32>(windowSize.x/2 - 350,windowSize.y/2 - 80,windowSize.x,windowSize.y/2+90)); 
	totalEnemiesKilled->setOverrideFont(guiEnviroment->getFont("../assets/fonts/titillium-Bold.png"));
	aux.append(std::to_wstring(enemiesKilled));
	totalEnemiesKilled->setText(aux.c_str());
	
	irr::gui::IGUIStaticText* timeWasted = guiEnviroment->addStaticText(L"",irr::core::rect<irr::s32>(windowSize.x/2 - 350,windowSize.y/2 - 30,windowSize.x,windowSize.y/2+140)); 
	timeWasted->setOverrideFont(guiEnviroment->getFont("../assets/fonts/titillium-Bold.png"));

	aux = L"TIME: ";
	aux.append(std::to_wstring(wastedTime.getElapsedTime()/60000));
	aux.append(L":");
	int seconds = (wastedTime.getElapsedTime()/1000) % 60;
	if(seconds < 10)
	{
		aux.append(L"0");
	}
	aux.append(std::to_wstring(seconds));
	timeWasted->setText(aux.c_str());
	
	createMenuButton(2, L"MENU",BUTTON_ID::BACK);
}

void GraphicsEngineIrrlicht::addServerBrowserButton()
{
	createMenuButton(3, L"LOCAL NETWORK",BUTTON_ID::SERVER_BROWSER);
}

void GraphicsEngineIrrlicht::addReconnectButton()
{
	createMenuButton(3, L"RECONNECT",BUTTON_ID::RECONNECT);
	createMenuButton(4, L"DISCONNECT",BUTTON_ID::DISCONNECT);
}

void GraphicsEngineIrrlicht::createMenuServerBrowser()
{
	addTitle();

	irr::gui::IGUIImage* help = guiEnviroment->addImage(irr::core::rect<irr::s32>(windowSize.x/2 - 430,windowSize.y/2+150, windowSize.x/2 + 430, windowSize.y/2+150 + 100));
	help->setImage(driver->getTexture(GUI::MENU_IMAGE_HELP_BROWSER));

	int buttonHeight = GUI::MENU_BUTTON_HEIGHT;
	int buttonWidth = GUI::MENU_BUTTON_WIDTH;

	createMenuButton(3,L"REFRESH",BUTTON_ID::REFRESH);
	
	createMenuButton(5, L"JOIN", BUTTON_ID::JOIN);
	createMenuButton(2, L"BACK", BUTTON_ID::BACK);

	// El cliente tiene lanzado un server
	if ( gameManager->getNetworkEngine()->isHosting() )
	{
		createMenuButton(4, L"CLOSE GAME", BUTTON_ID::CLOSE_SERVER);
	}
	else
	{
		createMenuButton(4,L"CREATE GAME", BUTTON_ID::CREATE_GAME);
	}

	tableServers = guiEnviroment->addTable(irr::core::rect<irr::s32>(windowSize.x/2 - 355,windowSize.y/2-112,windowSize.x/2 - 355 + 400,windowSize.y/2-112+100),0,600,true);

	clearServerTable();
}

void GraphicsEngineIrrlicht::clearServerTable() //Limpia la tabla y anyade las columnas
{
	tableServers->clear();
	tableServers->addColumn(L"IP");
	tableServers->setColumnWidth(0,150);

	tableServers->addColumn(L"PLAYERS");
	tableServers->setColumnWidth(1,50);

	tableServers->addColumn(L"PING");
	tableServers->setColumnWidth(2,50);
}


void GraphicsEngineIrrlicht::addServerToBrowserTable( ServerInfo serverInfo )
{
	if(tableServers == NULL)
	{
		return;
	}
	int position = tableServers->getRowCount();
	tableServers->addRow(position);	
	tableServers->setCellText(position,0,serverInfo.ip.ToString(false));
	std::wstringstream string;
	string << serverInfo.players << "/4";
	tableServers->setCellText(position,1,string.str().c_str());
	string.str(L"");
	string << serverInfo.ping << " ms";
	tableServers->setCellText(position,2,string.str().c_str());

}


void GraphicsEngineIrrlicht::createMenuClientLobby()
{
	addTitle();

	irr::gui::IGUIImage* help = guiEnviroment->addImage(irr::core::rect<irr::s32>(windowSize.x/2 - 430,windowSize.y/2+150, windowSize.x/2 + 430, windowSize.y/2+150 + 100));
	help->setImage(driver->getTexture(GUI::MENU_IMAGE_HELP_LOBBY));

	int buttonHeight = GUI::MENU_BUTTON_HEIGHT;
	int buttonWidth = GUI::MENU_BUTTON_WIDTH;
	irr::gui::IGUIButton* fighter;
	irr::gui::IGUIButton* battleship;
	irr::gui::IGUIButton* bomber;
	irr::gui::IGUIButton* engineer;
	
	//guiEnviroment->addButton(irr::core::rect<irr::s32>(60,500,60 + 150,500 + 32), 0, 300,L"DISCONNECT");
	fighter = guiEnviroment->addButton(irr::core::rect<irr::s32>(windowSize.x/2 - 290,windowSize.y/2-150+120,windowSize.x/2 - 290 + 130,windowSize.y/2-150+220), 0, BUTTON_ID::FIGHTER,L"");
	fighter->setImage(driver->getTexture(GUI::MENU_IMAGE_LOBBY_FIGHTER));
	fighter->setUseAlphaChannel(true);
	battleship = guiEnviroment->addButton(irr::core::rect<irr::s32>(windowSize.x/2 - 290 + 150,windowSize.y/2-150+120,windowSize.x/2 - 290 + 150 + 130,windowSize.y/2-150+220), 0, BUTTON_ID::BATTLESHIP,L"");
	battleship->setImage(driver->getTexture(GUI::MENU_IMAGE_LOBBY_BATTLESHIP));
	battleship->setUseAlphaChannel(true);
	engineer = guiEnviroment->addButton(irr::core::rect<irr::s32>(windowSize.x/2 - 290 + 300,windowSize.y/2-150+120,windowSize.x/2 - 290 + 300 +130,windowSize.y/2-150+220), 0, BUTTON_ID::ENGINEER,L"");
	engineer->setImage(driver->getTexture(GUI::MENU_IMAGE_LOBBY_ENGINEER));
	engineer->setUseAlphaChannel(true);
	bomber = guiEnviroment->addButton(irr::core::rect<irr::s32>(windowSize.x/2 - 290 + 450,windowSize.y/2-150+120,windowSize.x/2 - 290 + 450 + 130,windowSize.y/2-150+220), 0, BUTTON_ID::BOMBER,L"");
	bomber->setImage(driver->getTexture(GUI::MENU_IMAGE_LOBBY_BOMBER));
	bomber->setUseAlphaChannel(true);
	createButton(windowSize.x/2-355, windowSize.y/2-150+230, buttonWidth, buttonHeight, BUTTON_ID::DISCONNECT, L"DISCONNECT");

	begin = createButton(windowSize.x/2-355 + 400, windowSize.y/2-150+230, buttonWidth, buttonHeight, BUTTON_ID::START, L"START");

	begin->setVisible(false);
	checkBoxReady = guiEnviroment->addCheckBox(false,irr::core::rect<irr::s32>(windowSize.x/2+210,windowSize.y/2-150,windowSize.x/2+300,windowSize.y/2-150 + 32), NULL, BUTTON_ID::READY_CHECKBOX, L"READY");
	
	tablePlayers = guiEnviroment->addTable(irr::core::rect<irr::s32>(windowSize.x/2-300,windowSize.y/2-150,windowSize.x/2+200,windowSize.y/2-150+150), NULL, BUTTON_ID::BROWSER_TABLE, true);
	
	std::vector<PlayerInfo> playerList;
	updateTablePlayers(playerList);
}

void GraphicsEngineIrrlicht::createMenuSelectCharacter()
{
	addTitle();

	irr::gui::IGUIImage* help = guiEnviroment->addImage(irr::core::rect<irr::s32>(windowSize.x/2 - 430,windowSize.y/2+150, windowSize.x/2 + 430, windowSize.y/2+150 + 100));
	help->setImage(driver->getTexture(GUI::MENU_IMAGE_HELP_LOBBY));

	int buttonHeight = GUI::MENU_BUTTON_HEIGHT;
	int buttonWidth = GUI::MENU_BUTTON_WIDTH;
	irr::gui::IGUIButton* fighter;
	irr::gui::IGUIButton* battleship;
	irr::gui::IGUIButton* bomber;
	irr::gui::IGUIButton* engineer;
	
	//guiEnviroment->addButton(irr::core::rect<irr::s32>(60,500,60 + 150,500 + 32), 0, 300,L"DISCONNECT");
	fighter = guiEnviroment->addButton(irr::core::rect<irr::s32>(windowSize.x/2 - 290,windowSize.y/2-150,windowSize.x/2 - 290 + 130,windowSize.y/2-150+220), 0, BUTTON_ID::FIGHTER,L"");
	fighter->setImage(driver->getTexture(GUI::MENU_IMAGE_LOBBY_FIGHTER));
	fighter->setUseAlphaChannel(true);
	battleship = guiEnviroment->addButton(irr::core::rect<irr::s32>(windowSize.x/2 - 290 + 150,windowSize.y/2-150,windowSize.x/2 - 290 + 150 + 130,windowSize.y/2-150+220), 0, BUTTON_ID::BATTLESHIP,L"");
	battleship->setImage(driver->getTexture(GUI::MENU_IMAGE_LOBBY_BATTLESHIP));
	battleship->setUseAlphaChannel(true);
	engineer = guiEnviroment->addButton(irr::core::rect<irr::s32>(windowSize.x/2 - 290 + 300,windowSize.y/2-150,windowSize.x/2 - 290 + 300 +130,windowSize.y/2-150+220), 0, BUTTON_ID::ENGINEER,L"");
	engineer->setImage(driver->getTexture(GUI::MENU_IMAGE_LOBBY_ENGINEER));
	engineer->setUseAlphaChannel(true);
	bomber = guiEnviroment->addButton(irr::core::rect<irr::s32>(windowSize.x/2 - 290 + 450,windowSize.y/2-150,windowSize.x/2 - 290 + 450 + 130,windowSize.y/2-150+220), 0, BUTTON_ID::BOMBER,L"");
	bomber->setImage(driver->getTexture(GUI::MENU_IMAGE_LOBBY_BOMBER));
	bomber->setUseAlphaChannel(true);
	createMenuButton(2, L"BACK", BUTTON_ID::BACK);
}
// Agrega un boton para reiniciar el mapa
void GraphicsEngineIrrlicht::createMenuPathfinding()
{
	//guiEnviroment->addButton(irr::core::rect<irr::s32>(50, 550, 50 + 150,550 + 32), 0, 400,L"Reiniciar mapa");
	createButton(50,550, 335, 65,  BUTTON_ID::RESTART_MAP, L"REINICIAR MAPA");
}

// Menu de creacion de partida/servidor
void GraphicsEngineIrrlicht::createMenuCreateGame()
{
	addTitle();

	irr::gui::IGUIImage* help = guiEnviroment->addImage(irr::core::rect<irr::s32>(windowSize.x/2 - 430,windowSize.y/2+150, windowSize.x/2 + 430, windowSize.y/2+150 + 100));
	help->setImage(driver->getTexture(GUI::MENU_IMAGE_HELP_BROWSER));

	irr::gui::IGUIStaticText* warningText = guiEnviroment->addStaticText(	L"You're about to host a game on your computer\nPerformance could be reduce because of this\nClick CREATE to launch the server",
									irr::core::rect<irr::s32>(windowSize.x/2 - 350,windowSize.y/2 - 100,windowSize.x,windowSize.y/2+40) );
	warningText->setOverrideFont(guiEnviroment->getFont("../assets/fonts/OpenSans.xml"));

	createMenuButton(5, L"CREATE", BUTTON_ID::LAUNCH_SERVER);
	createMenuButton(2, L"BACK",  BUTTON_ID::BACK);
}

void GraphicsEngineIrrlicht::createMenuInGame()
{
	int buttonWidth = GUI::MENU_BUTTON_WIDTH;
	int buttonHeight = GUI::MENU_BUTTON_HEIGHT;
	Vector2d position;
	

	// parent de los botones y la imagen de fondo
	irr::gui::IGUITab* parent = guiEnviroment->addTab(irr::core::rect<irr::s32>(0,0,2000,2000));
	inGameMenu = dynamic_cast<irr::gui::IGUIElement*>(parent);

	// Imagen de fondo
	position.x = (windowSize.x / 2) - (350 / 2); // 350 es el ancho de la image
	position.y = (windowSize.y / 2) - (300 / 2); // 300 es el alto de la imagen
	irr::gui::IGUIImage* background = guiEnviroment->addImage(irr::core::rect<irr::s32>(position.x, position.y, position.x + 350, position.y + 300), inGameMenu);
	background->setImage(driver->getTexture(GUI::MENU_IMAGE_IN_GAME_BG));

	// Botones
	position.x = (windowSize.x / 2) - (GUI::MENU_BUTTON_WIDTH / 2);
	position.y = (windowSize.y / 2) - (buttonHeight / 2);
	float verticalSpace = buttonHeight / 2;
	createButton(position.x,position.y - buttonHeight - verticalSpace, buttonWidth, buttonHeight,  BUTTON_ID::RESUME, L"RESUME", inGameMenu);
	createButton(position.x,position.y, buttonWidth, buttonHeight,  BUTTON_ID::BACK, L"MAIN MENU", inGameMenu);
	createButton(position.x,position.y + buttonHeight + verticalSpace, buttonWidth, buttonHeight,  BUTTON_ID::EXIT, L"EXIT GAME", inGameMenu);

	inGameMenu->setVisible(false);
}

void GraphicsEngineIrrlicht::toggleMenuInGame()
{
	if (inGameMenu == NULL)
	{
		createMenuInGame();
	}

	inGameMenu->setVisible( !(inGameMenu->isVisible()) );
}


void GraphicsEngineIrrlicht::updateTablePlayers(std::vector<PlayerInfo> playerList)
{
	if(tablePlayers == NULL)
	{
		return;
	}
	tablePlayers->clear();
	tablePlayers->addColumn(L"NAME");
	tablePlayers->addColumn(L"ID");
	tablePlayers->addColumn(L"PING");
	tablePlayers->addColumn(L"CHARACTER");
	tablePlayers->addColumn(L"READY");
	tablePlayers->setColumnWidth(0,100);
	tablePlayers->setColumnWidth(1,175);
	tablePlayers->setColumnWidth(2,50);
	tablePlayers->setColumnWidth(3,75);
	tablePlayers->setColumnWidth(4,75);

	for(std::size_t i=0; i < playerList.size(); i++)
	{
		tablePlayers->addRow(i);	
		tablePlayers->setCellText(i,0,playerList[i].name.C_String());
		tablePlayers->setCellText(i,1,playerList[i].guid.ToString());
		std::wstringstream string;
		string<<playerList[i].ping<<" ms";
		tablePlayers->setCellText(i,2,string.str().c_str());
		tablePlayers->setCellText(i,3,playerList[i].character.C_String());
		if(playerList[i].ready)
		{
			tablePlayers->setCellText(i,4,L"READY");
		}else
		{
			tablePlayers->setCellText(i,4,L"-");
		}

	}
}

void GraphicsEngineIrrlicht::updateResolution()
{
	resolutionButton->setText(gameSettings->getResolution().str().c_str());
}

void GraphicsEngineIrrlicht::clear()
{
	irr::core::vector3df skyboxRotation = skybox->getRotation(); //Para mantener el giro entre cambios de menus

	guiEnviroment->clear();
	clearIGUI();
	sceneManager->clear();
	loadSkybox(Vector3d(skyboxRotation.X, skyboxRotation.Y, skyboxRotation.Z));
	createDebugText();

	isMenuState = false;

	screenQuad->reset();
}

void GraphicsEngineIrrlicht::quit()
{
	device->closeDevice();
}

// Posicion del raton en el plano 3D
Vector2d GraphicsEngineIrrlicht::getMousePositionOnGround()
{
	irr::core::vector3df posIrr;
	EventManager* keyevent = gameManager->getEventManager();
	irr::core::line3df ray;

	ray = collisionManager->getRayFromScreenCoordinates(keyevent->mouseState.position,sceneManager->getActiveCamera());
	irr::core::plane3df groundPlane(irr::core::vector3df(0, 0, 0), irr::core::vector3df(0, 1, 0));

	groundPlane.getIntersectionWithLine(ray.start, ray.end - ray.start, posIrr); //Devuelve el resultado en target

	Vector2d pos = Vector2d(posIrr.X,posIrr.Z);

	return pos;
}


void GraphicsEngineIrrlicht::drawDebugLine(Vector3d start, Vector3d end, int color)
{
	debugLineList.push_back(DebugLine(start, end, color));
}

void GraphicsEngineIrrlicht::drawDebugLineList()
{
	driver->setMaterial(defaultMaterial);
	driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
	for (auto debugLine : debugLineList)
	{
		driver->setMaterial(debugMaterials[debugLine.color]);
		driver->draw3DLine(debugLine.start, debugLine.end);
	}
	debugLineList.clear();
}

Vector2d GraphicsEngineIrrlicht::getScreenCoordinatesFrom3DPosition(Vector3d position)
{
	irr::core::vector2di coordinates = collisionManager->getScreenCoordinatesFrom3DPosition(position);
	return Vector2d(coordinates.X, coordinates.Y);

}

void GraphicsEngineIrrlicht::setDebugText(const wchar_t* text)
{
	debugText->setText(text);
}

void GraphicsEngineIrrlicht::initializeDebugMaterials()
{
	irr::video::SMaterial red;
	irr::video::SMaterial green;
	irr::video::SMaterial blue;
	irr::video::SMaterial yellow;
	irr::video::SMaterial white;
	irr::video::SMaterial grey;
	irr::video::SMaterial brown;

	red.EmissiveColor = irr::video::SColor(255, 255, 78, 61);
	red.Thickness = 1.5;
	red.AmbientColor = irr::video::SColor(255, 0, 0, 0);

	green.EmissiveColor = irr::video::SColor(255, 82, 255, 59);
	green.Thickness = 1.5;
	green.AmbientColor = irr::video::SColor(255, 0, 0, 0);

	blue.EmissiveColor = irr::video::SColor(255, 62, 178, 255);
	blue.Thickness = 1.5;
	blue.AmbientColor = irr::video::SColor(255, 0, 0, 0);

	yellow.EmissiveColor = irr::video::SColor(255, 255, 161, 0);
	yellow.Thickness = 1.5;
	yellow.AmbientColor = irr::video::SColor(255, 0, 0, 0);

	white.EmissiveColor = irr::video::SColor(255, 250, 250, 250);
	white.Thickness = 1.5;
	white.AmbientColor = irr::video::SColor(255, 0, 0, 0);
	
	grey.EmissiveColor = irr::video::SColor(255, 50, 50, 50);
	grey.Thickness = 1.5;
	grey.AmbientColor = irr::video::SColor(255, 0, 0, 0);

	brown.EmissiveColor = irr::video::SColor(255, 139, 69, 19);
	brown.Thickness = 1.5;
	brown.AmbientColor = irr::video::SColor(255, 0, 0, 0);

	debugMaterials.push_back(red);
	debugMaterials.push_back(green);
	debugMaterials.push_back(blue);
	debugMaterials.push_back(yellow);
	debugMaterials.push_back(white);
	debugMaterials.push_back(grey);
	debugMaterials.push_back(brown);
}

void GraphicsEngineIrrlicht::changeNodeColor(NodeMesh* node, int color)
{
	NodeMeshIrrlicht* nodeMeshIrrlicht = dynamic_cast<NodeMeshIrrlicht *>(node);
	if(nodeMeshIrrlicht != NULL)
	{
		irr::scene::IMeshSceneNode* irrlichtNode = nodeMeshIrrlicht->getIrrlichtNode();
		irrlichtNode->getMaterial(0).AmbientColor = debugMaterials[color].EmissiveColor;


	}else
	{
		std::cout<<"ERROR: Se esta utilizando un nodo de TAG con el motor grafico de irrlicht\n";
	}
}
//Devuelve false si hay algo entre start y end en linea recta
bool GraphicsEngineIrrlicht::canSeeTarget( Vector2d start, Vector2d end )
{
	irr::core::line3df ray(start.x, 0, start.y, end.x, 0, end.y);
	irr::scene::ISceneNode* sceneNode = collisionManager->getSceneNodeFromRayBB(ray, 0 ,false,mapRoot);

	if(sceneNode != NULL)
	{
		return false; //Se ha chocado con algo
	}

	return true; //No se ha chocado con nada
}


void GraphicsEngineIrrlicht::createDebugMap(MapManager::CollisionMap &collisionMap)
{
	mapRoot = sceneManager->addEmptySceneNode();
	mapCube = sceneManager->addEmptySceneNode();
	mapCube->setParent(mapRoot); //añade nodo padre al mapa de cubos y lo une a la raiz de nodos
	mapCube->setVisible(false);
	mapArrow = sceneManager->addEmptySceneNode();
	mapArrow->setParent(mapRoot); //añade nodo padre al mapa de flecha y lo une a la raiz de nodos
	mapArrow->setVisible(false);
	for(int i=0;i<collisionMap.numFrames;i++)
	{
		for(int j=0;j<collisionMap.numFrames;j++)
		{
			MapManager* mapManager = gameManager->getMapManager();
			Vector2d positionOnGrid = mapManager->getFrameCenter(i,j);
			float scale = (float)mapManager->getMapSize()/160; //160 es MINIMAP_SIZE que aqui no esta la constante pro ser temporal
			float x = gameManager->getGraphicsEngine()->getWindowSize().x-180+(positionOnGrid.x/scale); 
			float y = gameManager->getGraphicsEngine()->getWindowSize().y-175+(-positionOnGrid.y/scale);
			//int id  = addGUISprite("../assets/images/minimap/point.png",Vector2d(x,y),Vector2d(17/scale,17/scale));

			if(collisionMap.map[i][j].type == 'W')
			{
				//Vector2d positionOnGrid= Vector2d(Math::floor(i*collisionMap.frameSize/collisionMap.frameSize)*collisionMap.frameSize+collisionMap.frameSize/2, (Math::floor(j*collisionMap.frameSize/collisionMap.frameSize)*collisionMap.frameSize+collisionMap.frameSize/2)*-1);
				addCube(positionOnGrid, Vector2d(0.95, 0.8), DebugColor::WHITE, true);	

				//Es temporal para dibujar el mapa donde va luego se borrara y sera una imagen

				//setColorGUISprite(id,DebugColor::BROWN);
				
			}else
			{
				//dibuja flecha		
				if(collisionMap.map[i][j].wallEjection.getLength() != 0)
				{

					float force = collisionMap.map[i][j].wallEjection.getLength() /10;					
					addArrow(positionOnGrid,collisionMap.map[i][j].wallEjection.getAngle(),Vector2d(force, force),DebugColor::RED,true);
				}
				
				//setColorGUISprite(id,DebugColor::GREY);
			}
		}
	}
}

void GraphicsEngineIrrlicht::putBegin(bool put)
{
	if(begin != NULL)
	{
		if(put)
		{
			begin->setVisible(true);
		}
		else
		{
			begin->setVisible(false);
		}
	}
}

void GraphicsEngineIrrlicht::setMenuPressed( int id )
{
	irr::gui::IGUIElement* element = guiEnviroment->getRootGUIElement()->getElementFromId(id);

	irr::gui::IGUIButton* button = dynamic_cast<irr::gui::IGUIButton*>(element);
	if(button != NULL)
	{
		button->setPressed();
	}
}


void GraphicsEngineIrrlicht::setMenuFocus(int id)
{
	if(id == -1)
	{
		irr::gui::IGUIButton * button = dynamic_cast<irr::gui::IGUIButton*>(guiEnviroment->getFocus());

		if(button != NULL)
		{
		button->setPressed(false);
		guiEnviroment->removeFocus(button);
		}
		
		return;
	}
	irr::gui::IGUIElement* element = guiEnviroment->getRootGUIElement()->getElementFromId(id);
	guiEnviroment->setFocus(element);
}

void GraphicsEngineIrrlicht::setNextMenuFocus(bool reverse)
{
	// start the search at the root of the current tab group
	irr::gui::IGUIElement *focus = guiEnviroment->getFocus();

	irr::gui::IGUIElement *startPos = focus ? focus->getTabGroup() : 0;
	int startOrder = -1;

	if (focus && !focus->isTabGroup())
	{
		startOrder = focus->getTabOrder();
		if (startOrder == -1)
		{
			// this element is not part of the tab cycle,
			// but its parent might be...
			irr::gui::IGUIElement *el = focus;
			while (el && el->getParent() && startOrder == -1)
			{
				el = el->getParent();
				startOrder = el->getTabOrder();
			}

		}
	}

	if (!startPos)
	{
		startPos = guiEnviroment->getRootGUIElement();
	}	// start at the root

	// find the element
	irr::gui::IGUIElement *closest = NULL;
	irr::gui::IGUIElement *first = NULL;
	startPos->getNextElement(startOrder, reverse, false, first, closest);

	irr::gui::IGUIElement *next = NULL;

	if (closest)
	{
		next = closest; // we found an element
	} else if (first)
	{
		next = first; // go to the end or the start
	} 

	if (next && next != focus)
	{
		guiEnviroment->setFocus(next);
	}
}

unsigned int GraphicsEngineIrrlicht::getUpdateTime()
{
	return updateTime;
}

unsigned int GraphicsEngineIrrlicht::getUpdateVSTime()
{
	return updateVSTime;
}

void GraphicsEngineIrrlicht::drawUpdateTimesRectangles(UpdateTimes ut)
{	
	//Actualizamos los rectangulos segun el tiempo de cada update
	updateTimeRectangleSize(ut.gameObjectManagerTime, gomRect);

	updateTimeRectangleSize(ut.collisionManagerTime, cmRect);

	updateTimeRectangleSize(ut.graphicEngineTime, geRect);

	updateTimeRectangleSize(ut.debugToolsTime, dbgRect);

	updateTimeRectangleSize(ut.verticalSyncTime, vsRect);
	
	updateTimeRectangleSize(ut.aiFlockingTime, aifRect);	
}

void GraphicsEngineIrrlicht::updateTimeRectangleSize( int time, irr::gui::IGUIImage* rect )
{
	if(time <= 0) 
	{
		rect->setMaxSize(irr::core::dimension2du(130,1));//No puedo poner el y a 0 porque se dibuja todo
	} else
	{
		rect->setMaxSize(irr::core::dimension2du(130,time*100/50));
	}
}


//anyade los rectangulos de los tiempos 
void GraphicsEngineIrrlicht::addDebugTimeRectangles()
{
	int x=windowSize.x/2 - 147;
	int y=100+65;
	int separation=GUI::DEBUG_RECTANGLE_SEPARATION;
	int heightRectangles=GUI::DEBUG_RECTANGLE_HEIGHT;
	
	gomRect = guiEnviroment->addImage(irr::core::rect<irr::s32>(x,y, x+10, y+heightRectangles), NULL, 27);//420 para estar alineada a la derecha
	gomRect->setImage(driver->getTexture(GUI::DEBUG_IMAGE_RECTANGLE));
	gomRect->setScaleImage(true);
	gomRect->setVisible(false);

	cmRect = guiEnviroment->addImage(irr::core::rect<irr::s32>(x+separation,y, x+separation+10, y+heightRectangles));//420 para estar alineada a la derecha
	cmRect->setImage(driver->getTexture(GUI::DEBUG_IMAGE_RECTANGLE));
	cmRect->setScaleImage(true);
	cmRect->setVisible(false);

	geRect = guiEnviroment->addImage(irr::core::rect<irr::s32>(x+separation*2,y, x+separation*2+10, y+heightRectangles));//420 para estar alineada a la derecha
	geRect->setImage(driver->getTexture(GUI::DEBUG_IMAGE_RECTANGLE));
	geRect->setScaleImage(true);
	geRect->setVisible(false);

	dbgRect = guiEnviroment->addImage(irr::core::rect<irr::s32>(x+separation*3,y, x+separation*3+10, y+heightRectangles));//420 para estar alineada a la derecha
	dbgRect->setImage(driver->getTexture(GUI::DEBUG_IMAGE_RECTANGLE));
	dbgRect->setScaleImage(true);
	dbgRect->setVisible(false);

	vsRect = guiEnviroment->addImage(irr::core::rect<irr::s32>(x+separation*4,y, x+separation*4+10, y+heightRectangles));//420 para estar alineada a la derecha
	vsRect->setImage(driver->getTexture(GUI::DEBUG_IMAGE_RECTANGLE));
	vsRect->setScaleImage(true);
	vsRect->setVisible(false);

	aifRect = guiEnviroment->addImage(irr::core::rect<irr::s32>(x+separation*5,y, x+separation*5+10, y+heightRectangles));//420 para estar alineada a la derecha
	aifRect->setImage(driver->getTexture(GUI::DEBUG_IMAGE_RECTANGLE));
	aifRect->setScaleImage(true);
	aifRect->setVisible(false);

}

void GraphicsEngineIrrlicht::setUpdateTimeRectanglesVisible(bool visible)
{
	gomRect->setVisible(visible);
	cmRect->setVisible(visible);
	geRect->setVisible(visible);
	dbgRect->setVisible(visible);
	vsRect->setVisible(visible);
	aifRect->setVisible(visible);
}

//Metodos para el miniMapa
int GraphicsEngineIrrlicht::addGUISprite(const char* spriteFilename, Vector2d position, Vector2d size)
{	
	int id  = getNextID();

	irr::gui::IGUIImage* point = guiEnviroment->addImage(irr::core::rect<irr::s32>(position.x,position.y, position.x+size.x, position.y+size.y),0,id);//420 para estar alineada a la derecha
	point->setImage(driver->getTexture(spriteFilename));	
	if(spriteFilename == GUI::MENU_IMAGE_LOADING)
	{
		point->setScaleImage(true);
		point->setMaxSize(irr::core::dimension2du(size.x,size.y));
	}
	return id;
}
void GraphicsEngineIrrlicht::setPositionGUISprite(int id, Vector2d position, Vector2d size)
{
	
	irr::gui::IGUIImage* point  = (irr::gui::IGUIImage*)guiEnviroment->getRootGUIElement()->getElementFromId(id); //
	if(point != NULL)
	{
			point->setRelativePosition(irr::core::rect<irr::s32>(position.x,position.y, position.x+size.x, position.y+size.y));

	}
}

void GraphicsEngineIrrlicht::removeGUISprite(int id)
{

	irr::gui::IGUIImage* point  = (irr::gui::IGUIImage*)guiEnviroment->getRootGUIElement()->getElementFromId(id); 

	if(point != NULL)
	{
		guiEnviroment->getRootGUIElement()->removeChild(point);

	}
}

void GraphicsEngineIrrlicht::setColorGUISprite(int id, int color)
{

	irr::gui::IGUIImage* point  = (irr::gui::IGUIImage*)guiEnviroment->getRootGUIElement()->getElementFromId(id); 

	if(point != NULL)
	{
		
		point->setColor(debugMaterials[color].EmissiveColor);
	}
	
}


void GraphicsEngineIrrlicht::setScaleGUISprite(int id, Vector2d scale)
{

	irr::gui::IGUIImage* point  = (irr::gui::IGUIImage*)guiEnviroment->getRootGUIElement()->getElementFromId(id); 

	if(point != NULL)
	{
		point->setScaleImage(true);
		point->setMaxSize(irr::core::dimension2du(scale.x,scale.y));
	}	

}

void GraphicsEngineIrrlicht::setVisibleGUISprite(int id,bool visible)
{

	irr::gui::IGUIImage* point  = (irr::gui::IGUIImage*)guiEnviroment->getRootGUIElement()->getElementFromId(id); 

	if(point != NULL)
	{
		point->setVisible(visible);	
	}	

}


//Gestiona los efectos de particulas que se insertan en el nodo (para no tener un monton de ifs en el addnodemesh)
void GraphicsEngineIrrlicht::addParticleToMesh(const char* meshFilename, irr::scene::IMeshSceneNode* irrlichtNode)
{
	if(meshFilename== Resources::MESH_PROJECTILE_MUZZLE) 
	{ 		
		fxEngine->addParticleToNode(irrlichtNode, FXEngine::ParticleEffect::SMOKE);				
		fxEngine->addParticleToNode(irrlichtNode, FXEngine::ParticleEffect::FIRE_AND_SMOKE_LOW);	
	}
	else if(meshFilename== Resources::MESH_PROJECTILE_ROCKET) 
	{ 				
		fxEngine->addParticleToNode(irrlichtNode, FXEngine::ParticleEffect::FIRE_AND_SMOKE_LOW);		
	}
	else if(meshFilename== Resources::MESH_PLAYER_FIGHTER) 
	{ 				
		fxEngine->addParticleToNode(irrlichtNode, FXEngine::ParticleEffect::FIGHTER_ENGINE);		
	}
	else if(meshFilename==Resources::MESH_PLAYER_ENGINEER) 
	{ 				
		fxEngine->addParticleToNode(irrlichtNode, FXEngine::ParticleEffect::ENGINER_RIGHT_ENGINE);		
		fxEngine->addParticleToNode(irrlichtNode, FXEngine::ParticleEffect::ENGINER_LEFT_ENGINE);		
	}
	else if(meshFilename==Resources::MESH_PLAYER_BOMBER)
	{ 				
		fxEngine->addParticleToNode(irrlichtNode, FXEngine::ParticleEffect::BOMBER_ENGINE); 
	}
	else if(meshFilename==Resources::MESH_PLAYER_BATTLESHIP) 
	{ 				
		fxEngine->addParticleToNode(irrlichtNode, FXEngine::ParticleEffect::BATTLESHIP_ENGINE); 		
	}
	else if(meshFilename==Resources::MESH_PROJECTILE_SPARK)
	{
		fxEngine->addParticleToNode(irrlichtNode, FXEngine::ParticleEffect::SPARK);
	}
	else if(meshFilename==Resources::MESH_PROJECTILE_FIRE)
	{
		fxEngine->addParticleToNode(irrlichtNode, FXEngine::ParticleEffect::FIRE_AND_SMOKE_MEDIUM);
	}
}

void GraphicsEngineIrrlicht::clearIGUI()
{
	menuFont = NULL;
	debugFont = NULL;

	debugText = NULL;
	begin = NULL;

	tableServers = NULL;
	tablePlayers = NULL;
	checkBoxReady = NULL;

	checkBoxFullScreen = NULL;
	checkBoxSync = NULL;
	volumen = NULL;
	resolutionButton = NULL;

	inventory = NULL;
	hpBar = NULL;
	shBar = NULL;
	hpText = NULL;
	shText = NULL;
	skill1 = NULL;
	skill2 = NULL;
	skill3 = NULL;
	skill4  = NULL;
	creation1 = NULL;
	creation2 = NULL;
	creation3 = NULL;
	creation4 = NULL;


	miniMap = NULL;

	topBar = NULL;
	panelWave = NULL;
	panelClock = NULL;
	panelJade = NULL;

	warningText = NULL;

	character = NULL;
	expBar = NULL;
	level = NULL;
	exp = NULL;

	//Imagenes/rectangulos de los tiempos de cada update (para debug)
	gomRect = NULL;
	cmRect = NULL;
	geRect = NULL;
	dbgRect = NULL;
	vsRect = NULL; //todavia no
	aifRect = NULL;

	// Parent de los elementos del menu ingame
	inGameMenu = NULL;
}

int GraphicsEngineIrrlicht::readShader(const char* meshFilename )
{
	int size = 0;
	std::string string;
	string += meshFilename;
	
	string[string.size()-1] = 'l';
	string[string.size()-2] = 't';
	string[string.size()-3] = 'm';

	std::ifstream file;
	file.open(string);

	if (!file) {
	//	std::cout << "ERROR:No pudo leerse el archivo\n";
		return -1;
	}else{

		std::string line;
		int size = 0;
		//leemos la hasta final de fichero si tiene 12 lineas es que tiene un shader asignado
		
		while(getline(file, line))
		{
			size++;
		}
		//Solo entra cuando la ultima linea sea shader algo
		if(line.find("shader") == 0)
		{
			//cortamos dejando el ultimo trozo que es el nombre
			line = line.substr(7,line.size());
			
			return atof(line.c_str()); //devolveremos el numero del shader correspondiente al enum
		}	

	}


	return -1;
}

int GraphicsEngineIrrlicht::readShaderTexture(const char* filename )
{


	irr::video::ITexture* texture = driver->getTexture(filename);


	
	/*ResourceTexture* image  = GameManager::getInstance()->getResourceManager()->loadTexture(filename,false,false,false);//->getTexture(fileName);

	std::cout<<"Size: "<<image->getImageSize()<<"\n";*/



/*	sf::Image image;
	Vector3d color;

	if(!image.loadFromFile(filename))
	{
		std::cout << "ERROR: No se pudo encontrar la imagen "<< filename << std::endl;
	}
	else
	{
		image.getSize().x;
		
		for(unsigned int i=0;i < image.getSize().x;i++)
		{
			color = Vector3d(image.getPixel(i,0).r,image.getPixel(i,0).g,image.getPixel(i,0).b);
			color.normalize();
			colorShaderR[i] = color.x;
			colorShaderG[i] = color.y;
			colorShaderB[i] = color.z;
			
		}

		/*texture = new sf::Texture();
		texture->loadFromImage(image);
		
	}*/



	return -1;


}


//recibe las urlsy crea un shader que agrega a la lista de shaders(hay que seguir un orden);
void GraphicsEngineIrrlicht::addShader(const char* vertFilename,const char* fragFilename)
{
	Shader* shader = new Shader(vertFilename,fragFilename);
	shaderList.push_back(shader);
}



//Crea un material y le pasamos un puntero al node Mesh que contiene la intensidad o lo que haga falta, esto se pasa a nuestra clase MyShaderCallback
irr::s32 GraphicsEngineIrrlicht::createMaterial( NodeMesh* nodeMesh, int shaderType, bool transparent, float radius)
{
	

	Shader* shader = shaderList[shaderType];
	
	// create materials
	irr::s32 newMaterialType = 0;

	switch (shaderType)
	{
	case Material::SHIELD:
		{	
			ShieldShaderCallBack* shaderCallback = new ShieldShaderCallBack();
			shaderCallback->setRadius(radius);
			shaderCallback->addNodeMesh(nodeMesh);
			return createMaterialForShader(shader, shaderCallback, transparent);	
			break;
		}
	case Material::TERRAIN:
		{
			//Agregamos textura, esto es de prueba, luego la pillaremos del mtl
		
			((NodeMeshIrrlicht*)nodeMesh)->getIrrlichtNode()->setMaterialTexture(0, driver->getTexture(Resources::TEXTURE_TERRAIN_MASK));
			((NodeMeshIrrlicht*)nodeMesh)->getIrrlichtNode()->setMaterialTexture(1, driver->getTexture(Resources::TEXTURE_TERRAIN_RED));
			((NodeMeshIrrlicht*)nodeMesh)->getIrrlichtNode()->setMaterialTexture(2, driver->getTexture(Resources::TEXTURE_TERRAIN_GREEN));
			((NodeMeshIrrlicht*)nodeMesh)->getIrrlichtNode()->setMaterialTexture(3, driver->getTexture(Resources::TEXTURE_TERRAIN_BLUE));
		
			/*auto node = ((NodeMeshIrrlicht*)nodeMesh)->getIrrlichtNode();
			irr::video::SMaterial mat = ((NodeMeshIrrlicht*)nodeMesh)->getIrrlichtNode()->getMaterial(0);*/


			TerrainShaderCallBack* shaderCallback = new TerrainShaderCallBack();
			shaderCallback->addNodeMesh(nodeMesh);
			return createMaterialForShader(shader, shaderCallback, transparent);

		break;
		}
	default:

		break;
	}	

	return newMaterialType;
	//return newMaterialType;

}

irr::s32 GraphicsEngineIrrlicht::createMaterialForShader( Shader* shader, irr::video::IShaderConstantSetCallBack* shaderCallback, bool transparent )
{
	const irr::video::E_GPU_SHADING_LANGUAGE shadingLanguage = irr::video::EGSL_DEFAULT;
	
	// create materials
	irr::s32 newMaterialType = 0;

	if(transparent)
	{

		newMaterialType = driver->getGPUProgrammingServices()->addHighLevelShaderMaterial(
			shader->getVertexFileName().c_str(),"main",irr::video::EVST_VS_1_1,
			shader->getFragmentFileName().c_str(), "main", irr::video::EPST_PS_1_1,
			shaderCallback,irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL,0,shadingLanguage);
		
	}else
	{
		newMaterialType = driver->getGPUProgrammingServices()->addHighLevelShaderMaterial(
			shader->getVertexFileName().c_str(),"main",irr::video::EVST_VS_1_1,
			shader->getFragmentFileName().c_str(), "main", irr::video::EPST_PS_1_1,
			shaderCallback,irr::video::EMT_SOLID,0,shadingLanguage);	
	}
	

	shaderCallback->drop();
	
	return newMaterialType;
}

void GraphicsEngineIrrlicht::setWarningMessage( const wchar_t* warningMessage )
{
	warningText->setText(warningMessage);
	warningTextClock.restart();
}

void GraphicsEngineIrrlicht::updateWarningText()
{
	if(warningText != NULL && warningTextClock.getElapsedTime() > 1000)
	{
		warningText->setText(L"");
		warningTextClock.restart();
		warningTextClock.pause();
	}
}

void GraphicsEngineIrrlicht::damageEffectScreenQuad( bool keep)
{
	screenQuad->damageEffect(keep);
}






