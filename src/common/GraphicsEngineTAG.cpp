#pragma once

#include "GraphicsEngineTAG.h"

#include <glew/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "SceneManager.h"
#include "GameManager.h"
#include "GameObjectManager.h"
#include "AIEngine.h"
#include "SceneNode.h"
#include "EntityMesh.h"
#include "EntityTransform.h"
#include "EntityCamera.h"
#include "EntityLight.h"
#include "NodeMeshTAG.h"
#include "NodeCameraTAG.h"
#include "NodeTextTAG.h"

#include "GUIManager.h"
#include "GUIButton.h"
#include "GUISprite.h"
#include "GUIText.h"
#include "GUITable.h"

#include "ShaderProgramTAG.h"
#include "FXEngineTAG.h"

using namespace GUI;

/*
NOTA IMPORTANTE:
Como en OpenGL invertimos la x a la hora de dibujar, para "engañar" a la vista
y que el resultado sea el mismo que en Irrlicht, debemos invertir la x cuando
se dibuja, se convierten coordenadas de mundo a pantalla o viceversa...
*/

GraphicsEngineTAG::GraphicsEngineTAG(void)
{
	sceneManager = new SceneManager();
	gameManager = GameManager::getInstance();
	guiManager = new GUIManager();
	fxEngine = NULL; // Esto lo creamos con la ventana
	eventManager = NULL;
	gameSettings = NULL;

	nextID = 1000;

	clearGUI();
	
	updateTime = -1;

	gridVBO = 0;
	gridNumPoints = 0;

	// Flags
	wireframe = false;
	culling = true;
	textures = true;

	initializeDebugMaterials();

	// Render to texture
	renderToTexture = true;
	fboSupport = false;

	renderedTexture = 0;
	depthStencilBuffer = 0;
	framebuffer = 0;
	uAttrCoord = 0;
	uAttrTex = 0;
	vboQuadVert = 0;
	shaderRender = NULL;

}

GraphicsEngineTAG::~GraphicsEngineTAG(void)
{
	glDeleteBuffers(1, &gridVBO);

	delete guiManager;
	guiManager = NULL;
	delete sceneManager;
	sceneManager = NULL;
	delete fxEngine;
	fxEngine = NULL;
	delete window;
	window = NULL;

	eventManager = NULL;
	gameManager = NULL;
	gameSettings = NULL;

	// DEBUG
	debugText = NULL;

	// Inicializa las variables del GUI
	clearGUI();

	updateVSTime = 0;

	// Render to texture
	delete shaderRender;
	shaderRender = NULL;

	if (renderToTexture)
	{
		glDeleteTextures(1, &renderedTexture);
		glDeleteBuffers(1, &vboQuadVert);

		if (fboSupport)
		{
			glDeleteRenderbuffers(1, &depthStencilBuffer);
			glDeleteFramebuffers(1, &framebuffer);
		}
	}
}

void GraphicsEngineTAG::clear()
{
	sceneManager->clear();
	guiManager->clear();
	clearGUI();

	createDebugText();

	//Reinicia FXEngineTag
	fxEngine->reset();
}

void GraphicsEngineTAG::quit()
{
	this->running = false;
	window->close();
}

void GraphicsEngineTAG::setWireframe(bool value)
{
	wireframe = value;
	if (wireframe)
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	else
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
}
void GraphicsEngineTAG::setCulling(bool value)
{
	culling = value;
	if (culling)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}
void GraphicsEngineTAG::setTextures(bool value)
{
	textures = value;
}

void GraphicsEngineTAG::createFramebuffer()
	{
		// create a texture object
		glGenTextures(1, &renderedTexture);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, window->getSize().x, window->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	//glTexImage2DMultisample(GL_TEXTURE_2D, 0, GL_RGBA8, window->getSize().x, window->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		// Shader para dibujar con el fbo
		// Pass through para el framebuffer
		shaderRender = new ShaderProgramTAG();
		shaderRender->loadFromFile(ShaderProgramTAG::Type::VERTEX, Resources::SHADER_POST_PASS_VERT);
		shaderRender->loadFromFile(ShaderProgramTAG::Type::FRAGMENT, Resources::SHADER_POST_PASS_FRAG);
		shaderRender->linkProgram();
		shaderRender->bind();

		uAttrCoord = glGetAttribLocation(shaderRender->getProgramID(), "v_coord");
		uAttrTex = glGetUniformLocation(shaderRender->getProgramID(), "fbo_texture");

	shaderRender->setUniformi(uAttrTex, 0);

		shaderRender->unbind();

		// VBO para el quad del render
		GLfloat vboVertices[] = { -1, -1, 1, -1, -1, 1, 1, 1 };
		glGenBuffers(1, &vboQuadVert);
		glBindBuffer(GL_ARRAY_BUFFER, vboQuadVert);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vboVertices), vboVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Soporte para framebuffer
		if ( isExtensionSupported("GL_ARB_framebuffer_object")  )
		{
			fboSupport = true;

			// Inicializacion del framebuffer
			// create a framebuffer object
			glGenFramebuffers(1, &framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

			// create a renderbuffer object to store depth info
			glGenRenderbuffers(1, &depthStencilBuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, window->getSize().x, window->getSize().y);
			
			// attach the texture to FBO color attachment point
			glFramebufferTexture2D(GL_FRAMEBUFFER,	// 1. fbo target: GL_FRAMEBUFFER
				GL_COLOR_ATTACHMENT0,				// 2. attachment point
				GL_TEXTURE_2D,						// 3. tex target: GL_TEXTURE_2D
				renderedTexture,					// 4. tex ID
				0);									// 5. mipmap level: 0(base)

			// attach the renderbuffer to depth attachment point
			glFramebufferRenderbuffer(GL_FRAMEBUFFER,	// 1. fbo target: GL_FRAMEBUFFER
				GL_DEPTH_ATTACHMENT,					// 2. attachment point
				GL_RENDERBUFFER,						// 3. rbo target: GL_RENDERBUFFER
				depthStencilBuffer);					// 4. rbo ID

			// attach the renderbuffer to depth attachment point
			glFramebufferRenderbuffer(GL_FRAMEBUFFER,	// 1. fbo target: GL_FRAMEBUFFER
				GL_STENCIL_ATTACHMENT,					// 2. attachment point
				GL_RENDERBUFFER,						// 3. rbo target: GL_RENDERBUFFER
				depthStencilBuffer);					// 4. rbo ID

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
		else
		{
			fboSupport = false;
		}
}

// Crea el contexto y la ventana OpenGL
bool GraphicsEngineTAG::createWindow(EventManager* eventManager, GameSettings* gameSettings)
{
	this->gameSettings = gameSettings;

	sf::ContextSettings settings;
	settings.depthBits = (unsigned int)gameSettings->getColor();
	settings.stencilBits = 8;  // 8 bits stencil buffer
	settings.antialiasingLevel = 8;

	// Asociamos el manejador de eventos con el GUIManager
	this->eventManager = eventManager;
	eventManager->setGUIManager(guiManager);

	int windowStyle;
	if (gameSettings->getFullScreen())
	{
		 windowStyle = sf::Style::Fullscreen;
	}
	else
	{
		windowStyle = sf::Style::Close;
	}

	windowSize.x = gameSettings->getWidth();
	windowSize.y = gameSettings->getHeight();

	window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y),
					"Space Defenders",
					windowStyle,
					settings);

	// VSync
	if (gameSettings->getSync())
	{
		window->setVerticalSyncEnabled(true);
	}

	// Inicializa varios parametros de opengl
	initOpenGL();

	// Inicializa Glew (despues de OpenGL)
	glewStatus = glewInit();

	// Carga las imagenes del skybox
	loadSkybox();

	this->running = true;
	isMenuState = false;

	createDebugText();

	//window->setMouseCursorVisible(false);

	// Comprobaciones de soporte de caracteristicas
	// Comprueba el soporte para VBO
	if (isExtensionSupported("GL_ARB_vertex_buffer_object"))
	{
		useVBO = true;
	}
	else
	{
		useVBO = false;
	}

	// Inicializacion de lo necesario para renderizar la escena a textura
	if (renderToTexture)
	{
		createFramebuffer();
	}

	// Iniciamos el motor de efectos una vez tenemos las dimensiones de la ventana
	fxEngine = new FXEngineTAG();

	// Imprimimos info varia sobre la creacion del contexto OpenGL
	// y extensiones
	logWindowInit();
	
	return true;
}

bool GraphicsEngineTAG::checkFramebufferStatus()
{
	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		std::cout << "Framebuffer complete." << std::endl;
		return true;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
		return false;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		std::cout << "[ERROR] Framebuffer incomplete: Unsupported by FBO implementation." << std::endl;
		return false;
	default:
		std::cout << "[ERROR] Framebuffer incomplete: Unknown error." << std::endl;
		return false;
	}
}

void GraphicsEngineTAG::logWindowInit()
{
	sf::ContextSettings settings;
	// Info sobre la ventana creada
	settings = window->getSettings();
	std::cout << "Depth bits: " << settings.depthBits << std::endl;
	std::cout << "Stencil bits: " << settings.stencilBits << std::endl;
	std::cout << "Antialiasing level: " << settings.antialiasingLevel << std::endl;
	std::cout << "OpenGL Version: " << settings.majorVersion << "." << settings.minorVersion << std::endl << std::endl;

	if (glewStatus == GLEW_OK)
	{
		std::cout << "GLEW OK" << std::endl;
	}
	else
	{
		std::cout << "Failed to initialize GLEW, " << glewGetErrorString(glewStatus) << std::endl;
	}

	if (useVBO)
	{
		std::cerr << "Encontrado GL_ARB_vertex_buffer_object: VBO activo" << std::endl;
	}
	else
	{
		std::cerr << "No encontrado GL_ARB_vertex_buffer_object: La tarjeta grafica no soporta VBO" << std::endl;
	}

	if (renderToTexture)
	{
		std::cout << "Render to texture ACTIVO...";

		if (fboSupport)
		{
			std::cout << "GL_ARB_framebuffer_object soportado. Usando frame buffer object." << std::endl;
			// check FBO status
			checkFramebufferStatus();
		}
		else
		{
			std::cout << "GL_ARB_framebuffer_object NO soportado. Usando glCopyTexSubImage2D." << std::endl;
		}
	}
	else
	{
		std::cout << "Render to texture NO ACTIVO. Se hace render directo." << std::endl;
	}

	std::cout << "-----------------------------------------------------" << std::endl;
	std::cout << std::endl << std::endl;
}

// Inizializa varios parametros para openGL
void GraphicsEngineTAG::initOpenGL()
{
	// INICIALIZA VARIOS PARAMETROS DE OPENGL
	// Color del fondo
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Habilita el Z-Buffer para lectura/escritura
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	// Cull triangles which normal is not towards the camera
	setCulling(true);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glClearDepth(1.f);

	// ILUMINACION GENERAL Y MATERIALES -----------------------------------------
	GLfloat ambientColorScene[] = {0.39, 0.43, 0.47, 1.0}; //Color(0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColorScene);
	glEnable(GL_LIGHTING);
	// --------------------------------------------------------------------------

	// Para la iluminacion y color de los materiales (necesario para usar glcolor con lighting activo)
	// GL_AMBIENT_AND_DIFFUSE hace que el uso de glcolor sea mas rapido que glmaterial pero tenemos
	// que tener en cuenta que el ambient y diffuse van a ser el mismo
	// IMPORTANTE: Si solo ponemos GL_AMBIENT en tarjetas nvidia y ATI no se ve nada.
	glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
	
	glEnable( GL_COLOR_MATERIAL );

	// Esto activa el blending, que es necesario para renderizar texturas con canal alpha (transparencia)
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Configura el viewport al tamaño de la ventana
	glViewport(0, 0, window->getSize().x, window->getSize().y);

	// PROJECTION
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Parametros para el frustum
	GLdouble zNear = 0.1;
	GLdouble zFar = 1000;
	GLdouble fov = 72;
	GLdouble ratio = double((window->getSize().x)/double(window->getSize().y));
	GLdouble fH = tan( double(fov / 360.0 * Math::PI) ) * zNear;
	GLdouble fW = fH * ratio;
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);

	// MODELVIEW
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

// Apila estados de OpenGL
void GraphicsEngineTAG::pushGLStates()
{
	glPushClientAttrib(	GL_ACCUM_BUFFER_BIT |
						GL_COLOR_BUFFER_BIT	|
						GL_LIGHTING_BIT |
						GL_POINT_BIT | // GL_POINT_SMOOTH state, point size. NECESARIO
						GL_STENCIL_BUFFER_BIT |
						GL_TEXTURE_BIT | // Texture coord, Minification filter and magnification filter, etc.
						//GL_TRANSFORM_BIT | // GL_NORMALIZE
						GL_VIEWPORT_BIT
						//GL_CURRENT_BIT // Este guarda varias para las primitivas con glBegin/glEnd
						);

	glPushAttrib( GL_ENABLE_BIT	);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
}
// Desapila los estados de OpenGL
void GraphicsEngineTAG::popGLStates()
{
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	// Desapilando atributos
	glPopClientAttrib();
	glPopAttrib();
}

// Llama al dibujado de la escena y otros elementos
void GraphicsEngineTAG::drawAll()
{
	sceneManager->draw(); // Luces, camaras y objetos 3D

	pushGLStates();
		window->resetGLStates();
		// Elementos 2D del GUI
		guiManager->draw(window);
	popGLStates();
}


// Llama a los eventos y al dibujado de los distintos elementos de la escena
void GraphicsEngineTAG::draw(void)
{
	updateWarningText();

	if (running)
	{
		// EVENTOS
		// Eventos de raton, teclado y GUI
		sf::Event event;
		if (window->pollEvent(event))
		{
			gameManager->getEventManager()->pollEventSFML(event);
		}
		/*
		// Eventos especificos del motor de TAG, para activar/desactivar caracteristicas
		// Combinacion de teclas con shift izquierdo
		if (eventManager->isKeyPressed(KEY::KEY_LSHIFT))
		{
			// !loquesea funciona como si fuera un toggle
			// Wireframe
			if(eventManager->isKeyDown(KEY::KEY_KEY_W))
			{
				setWireframe(!wireframe);
			}
			// Culling
			if(eventManager->isKeyDown(KEY::KEY_KEY_C))
			{
				setCulling(!culling);
			}
			// Texturas
			if(eventManager->isKeyDown(KEY::KEY_KEY_T))
			{
				setTextures(!textures);
			}
		}
		*/
		// El EventManager puede haber cerrado la ventana
		if (!running)
		{
			return;
		}
	
		updateClock.restart(); // No queremos contar la captura de eventos

		// RENDERIZADO
		// Sea cual sea el modo de renderizado, primero hay que limpiar los buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glLoadIdentity();

		if ( !wireframe && renderToTexture)
		{
			if (fboSupport) // FBO
			{
				// set the rendering destination to FBO
				glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

				// Ahora estamos en otro framebuffer que tambien hay que limpiar sus buffers por separado
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

				// Llama al render de los objetos de la escena y el GUI
				drawAll();

				// back to normal window-system-provided framebuffer
				glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind

				glEnable( GL_TEXTURE_2D );
				
				glBindTexture(GL_TEXTURE_2D, renderedTexture);
				glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);

				glDisable( GL_TEXTURE_2D );
			}
			else // Modo compatibilidad hardware antiguo
			{
				glPushAttrib(GL_COLOR_BUFFER_BIT | GL_PIXEL_MODE_BIT); // for GL_DRAW_BUFFER and GL_READ_BUFFER
				glDrawBuffer(GL_BACK);
				glReadBuffer(GL_BACK);

				// Llama al render de los objetos de la escena y el GUI
				drawAll();

				// copy the framebuffer pixels to a texture
				glBindTexture(GL_TEXTURE_2D, renderedTexture);
				glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, window->getSize().x, window->getSize().y);
				glBindTexture(GL_TEXTURE_2D, 0);

				glPopAttrib(); // GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
			}
			
			glBindTexture(GL_TEXTURE_2D, renderedTexture);

			// Shader que aplica la textura del render de la escena sobre un quad
			shaderRender->bind();
			
			if (!gameManager->isMenuState())
			{
				// Si hay algun shader de postprocesado, reemplazara el de render por defecto
				fxEngine->bindPostProcessing();
			}

			// Esto es siempre 0...
			//shaderRender->setUniformi(uAttrTex, 0); //GL_TEXTURE

			glEnableVertexAttribArray(uAttrCoord);

			glBindBuffer(GL_ARRAY_BUFFER, vboQuadVert);
			glVertexAttribPointer(
				uAttrCoord,  // attribute
				2,                  // number of elements per vertex, here (x,y)
				GL_FLOAT,           // the type of each element
				GL_FALSE,           // take our values as-is
				0,                  // no extra data between each position
				0                   // offset of first element
				);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glDisableVertexAttribArray(uAttrCoord);

			// Esto va a desactivar cualquier shader activo
			shaderRender->unbind();

			glBindTexture(GL_TEXTURE_2D, 0);	
		}
		// Render normal de toda la vida
		else
		{
			// Llama al render de los objetos de la escena y el GUI
			drawAll();
		}

		// Muestra por pantalla
		window->display();
		
		updateTime = updateClock.getElapsedTime(); // Tiempo de la funcion Draw
		setWindowsCaption();
	}
}

void GraphicsEngineTAG::drawGUI(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	
	pushGLStates();
	window->resetGLStates();
	guiManager->draw(window);
	popGLStates();
	

	window->display();
	setWindowsCaption();
}

// Agrega un nodo de tipo malla a la escena
NodeMesh* GraphicsEngineTAG::addNodeMesh(const char* meshFilename, bool transparent, int debugColor /*= DebugColor::RED */, Vector2d position /*= Vector2d(-500, -500)*/, float rotation /*= 0*/, float scale /*= 1*/)
{
	// Crear el nodo de escena para la malla y su entidad
	SceneNode* sceneNodeMesh = new SceneNode();
	EntityMesh* entityMesh = new EntityMesh(meshFilename, transparent,scale); // Esta entidad llama al resourceManager para guardar la malla
	sceneNodeMesh->setEntity(entityMesh);

	// Crear el nodo de transformacion y su entidad
	SceneNode* sceneNodeTransform = new SceneNode();
	EntityTransform* entityTransform = new EntityTransform();
	sceneNodeTransform->setEntity(entityTransform);

	// Nodo transformacion como padre del mesh y este como hijo
	sceneNodeMesh->setParent(sceneNodeTransform);
	sceneNodeTransform->addChild(sceneNodeMesh);

	// Finalmente colgar el nodo de transformacion de la raiz de la escena
	sceneManager->addBranch(sceneNodeTransform);

	// Crear el NodeMeshTAG y asociar con el nodo hoja del arbol (mesh)
	// IMPORTANTE: Debe ser la hoja para que el NodeMeshTAG la elimine en su destructor!!!
	NodeMeshTAG* nodeMeshTAG = new NodeMeshTAG(sceneNodeMesh);

	// Inicializar las transformaciones
	nodeMeshTAG->setPosition(position);
	nodeMeshTAG->setRotation(rotation);
	nodeMeshTAG->setScale(scale);

	return nodeMeshTAG;
}

// Agrega un nodo tipo camara a la escena
NodeCamera* GraphicsEngineTAG::addNodeCamera(Vector3d position /*= Vector3d(20, 50, -20)*/, Vector3d target /*= Vector3d(0, 0, 0)*/)
{
	// Crear el nodo de escena para la camara y su entidad
	SceneNode* sceneNodeCamera = new SceneNode();
	EntityCamera* entityCamera = new EntityCamera(); // Esta entidad llama al resourceManager para guardar la malla
	sceneNodeCamera->setEntity(entityCamera);

	// Crear el nodo de transformacion y su entidad
	SceneNode* sceneNodeTransform = new SceneNode();
	EntityTransform* entityTransform = new EntityTransform();
	sceneNodeTransform->setEntity(entityTransform);

	sceneNodeCamera->setParent(sceneNodeTransform);
	sceneNodeTransform->addChild(sceneNodeCamera);

	// Finalmente colgar el nodo de transformacion de la raiz de la escena
	sceneManager->addBranch(sceneNodeTransform);

	// Crear el NodeMeshTAG y asociar con el nodo hoja del arbol
	NodeCameraTAG* nodeCameraTAG = new NodeCameraTAG();
	nodeCameraTAG->setNode(sceneNodeCamera);

	// Inicializar las transformaciones
	nodeCameraTAG->setPosition(position);
	nodeCameraTAG->setTargetPosition(target);

	// La camara se registra en la lista de camaras de la escena
	sceneManager->setActiveCamera(sceneNodeCamera);

	return nodeCameraTAG;
}

// Agrega un nodo tipo texto a la escena
NodeText* GraphicsEngineTAG::addNodeText()
{
	int id = getNextID();
	// Solo NodeTextTAG implementa TEntity para pasarselo a un SceneNode
	GUIText* guiText = guiManager->addText(Vector2d(0,0), L"", id);
	//GUIText* guiText = new GUIText(id);
	NodeTextTAG* nodeText = new NodeTextTAG( guiText, guiManager );

	return nodeText;
}

// Crea un nodo de luz
void GraphicsEngineTAG::initWorld()
{
	// Crear el nodo de escena para la camara y su entidad
	SceneNode* sceneNodeLight = new SceneNode();
	EntityLight* entityLight = new EntityLight(); // Esta entidad llama al resourceManager para guardar la malla
	sceneNodeLight->setEntity(entityLight);

	// Crear el nodo de transformacion y su entidad
	SceneNode* sceneNodeTransform = new SceneNode();
	EntityTransform* entityTransform = new EntityTransform();
	sceneNodeTransform->setEntity(entityTransform);
	// Los asociamos
	sceneNodeLight->setParent(sceneNodeTransform);
	sceneNodeTransform->addChild(sceneNodeLight);

	// Finalmente colgar el nodo de transformacion de la raiz de la escena
	sceneManager->addBranch(sceneNodeTransform);

	// Opciones de la luz global de la escena
	entityLight->setActive(true);
	entityLight->setGLLight(GL_LIGHT0);
	entityLight->setType(EntityLight::Type::DIRECTIONAL);
	entityLight->setAmbientColor(0.0, 0.0, 0.0, 1.0);
	entityLight->setDiffuseColor(0.78, 0.78, 0.78, 1.0);
	entityLight->setSpecularColor(0.0, 0.0, 0.0, 1.0);
	// Posicion/direccion relativa de la luz
	entityTransform->setTranslation(Vector3d(1, 5, 1));
	//entityTransform->setRotation(Vector3d(-45, 180, 0));

	// La luz se registra en la lista de luces de la escena
	sceneManager->addLight(sceneNodeLight);

	createHUD();
}

void GraphicsEngineTAG::initializeDebugMaterials()
{
	float* red = new float[3];
	red[0] = 1;
	red[1] = 0;
	red[2] = 0;

	debugMaterials.push_back(red);

	//std::cout << "red: " << debugMaterials[0][0] << ", " << debugMaterials[0][1] << ", " << debugMaterials[0][2] << std::endl;

	/*
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
	*/
}

// Agrega un title
void GraphicsEngineTAG::addTitle()
{
	guiManager->addImage(Vector2d(windowSize.x/2 - 220,windowSize.y/2 - 287), GUI::IMAGE_TITLE);
}

void GraphicsEngineTAG::addHelp( const char* menu )
{
	GUISprite* sprite = NULL;

	Vector2d position(windowSize.x/2 - 430,windowSize.y/2+150);

	if (menu == "MAIN")
	{
		guiManager->addImage(position, GUI::MENU_IMAGE_HELP_MAIN );
	}
	else if (menu == "SERVER_BROWSER")
	{
		guiManager->addImage(position, GUI::MENU_IMAGE_HELP_BROWSER );
	}
	else if (menu == "LOBBY")
	{
		guiManager->addImage(position, GUI::MENU_IMAGE_HELP_LOBBY );
	}
}

void GraphicsEngineTAG::createMenuMain()
{
	addTitle();
	addHelp("MAIN");

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
void GraphicsEngineTAG::createMenuSettings()
{
	addTitle();
	int buttonHeight = GUI::MENU_BUTTON_HEIGHT;
	int buttonWidth = GUI::MENU_BUTTON_WIDTH;

	bool fullscreen = gameSettings->getFullScreen();
	bool vsync = gameSettings->getSync();
	bool irrlicht = gameSettings->isIrrlicht();
	bool physics = gameSettings->usePhysics();
	bool aliasing = gameSettings->useAliasing();
	bool music = gameSettings->getVolumen();

	float vertical = windowSize.y/2+60-150;
	float horizontal = 100;

	checkBoxFullScreen = guiManager->addCheckBox(fullscreen, horizontal + windowSize.x/2 - 355,vertical + 20, BUTTON_ID::FULLSCREEN, L"Fullscreen" );
	checkBoxSync = guiManager->addCheckBox(vsync, horizontal + windowSize.x/2 + 45,vertical + 20, BUTTON_ID::V_SYNC, L"VSync" );
	checkBoxIrrlicht = guiManager->addCheckBox(irrlicht, horizontal + windowSize.x/2 - 355,vertical+90, BUTTON_ID::IRRLICHT, L"Use Irrlicht" );
	checkBoxPhysics = guiManager->addCheckBox(physics, horizontal + windowSize.x/2 + 45,vertical+90, BUTTON_ID::PHYSICS, L"Use Physics" );
	checkBoxAliasing = guiManager->addCheckBox(aliasing, horizontal + windowSize.x/2 - 355,vertical+55, BUTTON_ID::ANTIALIASING, L"Use AntiAliasing" );
	checkBoxMusic = guiManager->addCheckBox(music, horizontal + windowSize.x/2 + 45,vertical+55, BUTTON_ID::MUSIC, L"Music" );

	vertical = windowSize.y/2+95-250;

	GUIButton* button = guiManager->addButton(windowSize.x/2-177 -70, vertical, 65, 65, BUTTON_ID::RESOLUTION_BEFORE);
	button->setImage(GUI::MENU_IMAGE_ARROW_LEFT);
	button->setPressedImage(GUI::MENU_IMAGE_ARROW_LEFT_PRESSED);

	GUIButton* button2 = guiManager->addButton(windowSize.x/2 + 177 + 5, vertical, 65, 65, BUTTON_ID::RESOLUTION_NEXT);
	button2->setImage(GUI::MENU_IMAGE_ARROW_RIGHT);
	button2->setPressedImage(GUI::MENU_IMAGE_ARROW_RIGHT_PRESSED);
	
	resolutionButton = createButton(windowSize.x/2-170, windowSize.y/2+95-250, buttonWidth, buttonHeight,  gameSettings->getResolution().str().c_str(), BUTTON_ID::RESOLUTION_NEXT);
	createMenuButton(2, L"BACK", BUTTON_ID::SETTINGS_BACK);
	createMenuButton(5, L"APPLY", BUTTON_ID::APPLY);
}

void GraphicsEngineTAG::createMenuCredits()
{
	addTitle();
	addHelp("CREDITS");

	GUIText* creditsText = guiManager->addText(Vector2d(windowSize.x/2 - 350, windowSize.y/2 - 150), L"Space Defenders 2013-2014\nMartin Malaga, Alejandro; Garcia Cusso, Fernando\nGarcia Guilabert, Kevin; Martinez Vilar, Ruben\nRamírez Bernabeu, Oscar");
	creditsText->setFont(GUI::MENU_BUTTON_FONT);
	creditsText->setFontSize(GUI::MENU_BUTTON_FONT_SIZE);

	createMenuButton(2, L"BACK",  BUTTON_ID::BACK);
}

void GraphicsEngineTAG::createMenuCreateGame()
{
	addTitle();
	addHelp("SERVER_BROWSER");

	GUIText* warningText = guiManager->addText(Vector2d(windowSize.x/2 - 350, windowSize.y/2 - 100), L"You're about to host a game on your computer\nPerformance could be reduce because of this\nClick CREATE to launch the server");
	warningText->setFont(GUI::MENU_BUTTON_FONT);
	warningText->setFontSize(GUI::MENU_BUTTON_FONT_SIZE);

	createMenuButton(5, L"CREATE", BUTTON_ID::LAUNCH_SERVER);
	createMenuButton(2, L"BACK",  BUTTON_ID::BACK);
}

void GraphicsEngineTAG::createMenuInGame()
{
	int buttonWidth = GUI::MENU_BUTTON_WIDTH;
	int buttonHeight = GUI::MENU_BUTTON_HEIGHT;
	Vector2d position;
	
	// Imagen de fondo
	position.x = (windowSize.x / 2) - (350 / 2); // 350 es el ancho de la image
	position.y = (windowSize.y / 2) - (300 / 2); // 300 es el alto de la imagen
	inGameMenu.push_back( guiManager->addImage(position, GUI::MENU_IMAGE_IN_GAME_BG) );

	// Botones
	position.x = (windowSize.x / 2) - (GUI::MENU_BUTTON_WIDTH / 2);
	position.y = (windowSize.y / 2) - (buttonHeight / 2);
	float verticalSpace = buttonHeight / 2;

	inGameMenu.push_back( createButton(position.x,position.y - buttonHeight - verticalSpace, buttonWidth, buttonHeight, L"RESUME", BUTTON_ID::RESUME) );
	inGameMenu.push_back( createButton(position.x, position.y, buttonWidth, buttonHeight, L"MAIN MENU", BUTTON_ID::BACK) );
	inGameMenu.push_back( createButton(position.x,position.y + buttonHeight + verticalSpace, buttonWidth, buttonHeight, L"EXIT GAME", BUTTON_ID::EXIT) );
}

void GraphicsEngineTAG::toggleMenuInGame()
{
	int size = inGameMenu.size();
	if ( size == 0 )
	{
		createMenuInGame();
	}

	for (int i = 0; i < size; i++)
	{
		inGameMenu[i]->setVisible( !(inGameMenu[i]->getVisible()) );
	}
}

void GraphicsEngineTAG::createMenuGameOver(int levelRound, Clock wastedTime, int enemiesKilled)
{
	
	addTitle();
	addHelp("MAIN");
	std::wstring aux(L"Oleadas Totales : ");

	// Ronda
	std::wstring wstr = L"WAVES: ";
	wstr.append(std::to_wstring(levelRound));
	GUIText* text = guiManager->addText( Vector2d(windowSize.x/2 - 350, windowSize.y/2 - 130), wstr.c_str() );
	text->setFont("../assets/fonts/titillium-Bold.otf");
	text->setFontSize(24);
	text->setColor(255, 255, 255);

	// Enemigos destruidos
	text = guiManager->addText( Vector2d(windowSize.x/2 - 350, windowSize.y/2 - 80), L"KILLS : " );
	wstr = L"KILLS: ";
	wstr.append( std::to_wstring(enemiesKilled) );
	text->setString( wstr.c_str() );
	text->setFont("../assets/fonts/titillium-Bold.otf");
	text->setFontSize(24);
	text->setColor(255, 255, 255);
	
	// Tiempo
	text = guiManager->addText( Vector2d(windowSize.x/2 - 350, windowSize.y/2 - 30), L"TIME : " );
	wstr = L"TIME: ";
	wstr.append( std::to_wstring(wastedTime.getElapsedTime()/60000) );
	wstr.append(L":");
	int seconds = (wastedTime.getElapsedTime()/1000) % 60;
	if(seconds < 10)
	{
		wstr.append(L"0");
	}
	wstr.append(std::to_wstring(seconds));
	text->setString( wstr.c_str() );
	text->setFont("../assets/fonts/titillium-Bold.otf");
	text->setFontSize(24);
	text->setColor(255, 255, 255);

	
	createMenuButton(2, L"MENU",BUTTON_ID::BACK);
}

GUIButton* GraphicsEngineTAG::createButton(int x, int y, int width, int height, const wchar_t *text, int id)
{
	GUIButton* button = guiManager->addMenuButton(x, y, width, height, id, text);

	//button->setUseAlphaChannel(true);
	button->setImage( GUI::MENU_IMAGE_BUTTON_NORMAL );
	button->setPressedImage( GUI::MENU_IMAGE_BUTTON_HOVER );

	return button;
}
void GraphicsEngineTAG::createMenuButton(int buttonNumber, const wchar_t* text, int id)
{
	int maxButtons = GUI::MENU_MAX_BUTTONS;
	int columns = GUI::MENU_COLUMNS;
	int buttonWidth = GUI::MENU_BUTTON_WIDTH;
	int buttonHeight = GUI::MENU_BUTTON_HEIGHT;
	int buttonPosition = buttonNumber;

	// Reinicia el contador
	if (buttonNumber > (maxButtons - 1) )
	{
		buttonPosition -= maxButtons;
	}
	//El centro de la pantalla menos la mitad de lo que ocupan todas las columnas
	int leftPosition = buttonPosition/3 * (buttonWidth + 50) + windowSize.x / 2 - (buttonWidth*columns + 50 * (columns - 1))/2; 

	int startTopPosition = windowSize.y/2-112;
	int space = buttonHeight + 15;

	int topPosition = startTopPosition + space * (buttonNumber % 3);

	guiManager->addMenuButton(leftPosition, topPosition, buttonWidth, buttonHeight, id, text);
}

void GraphicsEngineTAG::addReconnectButton()
{
	createMenuButton(3, L"RECONNECT",BUTTON_ID::RECONNECT);
	createMenuButton(4, L"DISCONNECT",BUTTON_ID::DISCONNECT);
}

void GraphicsEngineTAG::addServerBrowserButton()
{
	createMenuButton(3, L"LOCAL NETWORK",BUTTON_ID::SERVER_BROWSER);
}

void GraphicsEngineTAG::setMenuSettings()
{
	
	//irr::scene::ICameraSceneNode *camera = sceneManager->addCameraSceneNodeMaya(NULL, -500);
	//camera->setFOV(60 * Math::DEGTORAD);

	isMenuState = true;
}
void GraphicsEngineTAG::initMenuScreen()
{
	clear(); // Limpia la pantalla
	addTitle();
	addHelp("MAIN");
}
void GraphicsEngineTAG::createMenuServerBrowser()
{
	addTitle();
	addHelp("SERVER_BROWSER");

	int buttonHeight = GUI::MENU_BUTTON_HEIGHT;
	int buttonWidth = GUI::MENU_BUTTON_WIDTH;

	createMenuButton(3, L"REFRESH", BUTTON_ID::REFRESH);
	createMenuButton(5, L"JOIN", BUTTON_ID::JOIN);
	createMenuButton(2, L"BACK", BUTTON_ID::BACK);

	// El cliente tiene lanzado un server
	if ( gameManager->getNetworkEngine()->isHosting() )
	{
		createMenuButton(4, L"CLOSE GAME", BUTTON_ID::CLOSE_SERVER);
	}
	else
	{
		createMenuButton(4, L"CREATE GAME",   BUTTON_ID::CREATE_GAME);
	}

	tableServers = guiManager->addTable(windowSize.x/2 - 355,windowSize.y/2-112, 600);

	clearServerTable();
}
void GraphicsEngineTAG::createMenuPathfinding()
{

}
void GraphicsEngineTAG::createMenuClientLobby()
{
	addTitle();
	addHelp("LOBBY");

	int buttonHeight = GUI::MENU_BUTTON_HEIGHT;
	int buttonWidth = GUI::MENU_BUTTON_WIDTH;

	GUIButton* fighter;
	GUIButton* battleship;
	GUIButton* bomber;
	GUIButton* engineer;

	//guiManager->addButton(irr::core::rect<irr::s32>(60,500,60 + 150,500 + 32), 0, 300,L"DISCONNECT");
	fighter = guiManager->addButton(windowSize.x/2 - 290,windowSize.y/2-150+120, 130, 100, BUTTON_ID::FIGHTER);
	fighter->setImage(GUI::MENU_IMAGE_LOBBY_FIGHTER);

	battleship = guiManager->addButton(windowSize.x/2 - 290 + 150,windowSize.y/2-150+120, 130, 100, BUTTON_ID::BATTLESHIP);
	battleship->setImage(GUI::MENU_IMAGE_LOBBY_BATTLESHIP);

	engineer = guiManager->addButton(windowSize.x/2 - 290 + 300,windowSize.y/2-150+120, 130, 100, BUTTON_ID::ENGINEER);
	engineer->setImage(GUI::MENU_IMAGE_LOBBY_ENGINEER);

	bomber = guiManager->addButton(windowSize.x/2 - 290 + 450,windowSize.y/2-150+120, 130, 100, BUTTON_ID::BOMBER);
	bomber->setImage(GUI::MENU_IMAGE_LOBBY_BOMBER);

	createButton(windowSize.x/2-355, windowSize.y/2-150+230, buttonWidth, buttonHeight, L"DISCONNECT", BUTTON_ID::DISCONNECT);

	begin = createButton(windowSize.x/2-355 + 400, windowSize.y/2-150+230, buttonWidth, buttonHeight, L"START", BUTTON_ID::START);

	begin->setVisible(false);

	checkBoxReady = guiManager->addCheckBox(false, windowSize.x/2+210,windowSize.y/2-150, BUTTON_ID::READY_CHECKBOX, L"READY");

	tablePlayers = guiManager->addTable(windowSize.x/2-300,windowSize.y/2-150, BUTTON_ID::BROWSER_TABLE);
		
	std::vector<PlayerInfo> playerList;
	updateTablePlayers(playerList);
}

void GraphicsEngineTAG::createMenuSelectCharacter()
{
	addTitle();
	addHelp("LOBBY");

	int buttonHeight = GUI::MENU_BUTTON_HEIGHT;
	int buttonWidth = GUI::MENU_BUTTON_WIDTH;

	GUIButton* fighter;
	GUIButton* battleship;
	GUIButton* bomber;
	GUIButton* engineer;

	float verticalPos = windowSize.y/2 - 100;

	fighter = guiManager->addButton(windowSize.x/2 - 290,verticalPos, 150, 100, BUTTON_ID::FIGHTER);
	fighter->setImage(GUI::MENU_IMAGE_LOBBY_FIGHTER);

	battleship = guiManager->addButton(windowSize.x/2 - 290 + 150,verticalPos, 150, 100, BUTTON_ID::BATTLESHIP);
	battleship->setImage(GUI::MENU_IMAGE_LOBBY_BATTLESHIP);

	engineer = guiManager->addButton(windowSize.x/2 - 290 + 300,verticalPos, 150, 100, BUTTON_ID::ENGINEER);
	engineer->setImage(GUI::MENU_IMAGE_LOBBY_ENGINEER);

	bomber = guiManager->addButton(windowSize.x/2 - 290 + 450,verticalPos, 150, 100, BUTTON_ID::BOMBER);
	bomber->setImage(GUI::MENU_IMAGE_LOBBY_BOMBER);

	createMenuButton(2, L"BACK", BUTTON_ID::BACK);
}

void GraphicsEngineTAG::clearServerTable()
{
	tableServers->clear();
	tableServers->addColumn(L"IP");
	tableServers->setColumnWidth(0,150);

	tableServers->addColumn(L"PLAYERS");
	tableServers->setColumnWidth(1,75);

	tableServers->addColumn(L"PING");
	tableServers->setColumnWidth(2,50);
}
void GraphicsEngineTAG::addServerToBrowserTable(ServerInfo serverInfo)
{
	if(tableServers == NULL)
	{
		return;
	}

	int position = tableServers->getRowCount();
	tableServers->addRow();

	tableServers->setCellText( position, 0, stringToWString(serverInfo.ip.ToString(false)));

	std::wstringstream string;
	string << serverInfo.players << "/4";

	tableServers->setCellText(position,1,string.str().c_str());
	string.str(L"");
	string << serverInfo.ping << " ms";
	tableServers->setCellText(position,2,string.str().c_str());
}

void GraphicsEngineTAG::updateTablePlayers(std::vector<PlayerInfo> playerList)
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
	tablePlayers->setColumnWidth(3,80);
	tablePlayers->setColumnWidth(4,75);

	for(std::size_t i=0; i < playerList.size(); i++)
	{
		std::string playerName = playerList[i].name.C_String();
		std::string guid = playerList[i].guid.ToString();
		std::string character = playerList[i].character.C_String();

		tablePlayers->addRow();	
		tablePlayers->setCellText(i, 0, stringToWString(playerName) );
		tablePlayers->setCellText(i, 1, stringToWString(guid) );

		std::wstringstream ping;
		ping << playerList[i].ping << " ms";
		tablePlayers->setCellText(i, 2, ping.str().c_str());
		tablePlayers->setCellText(i, 3, stringToWString(character) );

		if(playerList[i].ready)
		{
			tablePlayers->setCellText(i, 4, L"READY");
		}else
		{
			tablePlayers->setCellText(i, 4, L"-");
		}

	}
}

void GraphicsEngineTAG::updateResolution()
{
	resolutionButton->setText(gameSettings->getResolution().str().c_str());
}

// Devuelve la posicion del raton sobre el plano en el entorno 3D
Vector2d GraphicsEngineTAG::getMousePositionOnGround()
{

	EventManager* eventManager = gameManager->getEventManager();
	float winX = eventManager->mouseState.position.x;
	float winY = eventManager->mouseState.position.y;
	// Tenemos en cuenta que estamos en MODELVIEW y se acaba de cargar la identidad
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	GLint realy;  /*  OpenGL y coordinate position  */
	GLdouble wx, wy, wz;  /*  returned world x, y, z coords  */
	GLdouble wx2, wy2, wz2;  /*  returned world x, y, z coords  */
	glGetIntegerv (GL_VIEWPORT, viewport);
	glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
	/*  note viewport[3] is height of window in pixels  */
	realy = viewport[3] - (GLint)winY - 1;
	gluUnProject ((GLdouble)winX, (GLdouble) realy, 0.0,
		mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
	gluUnProject ((GLdouble)winX, (GLdouble) realy, 1.0,
		mvmatrix, projmatrix, viewport, &wx2, &wy2, &wz2);

	// line of sight intersection with y = 0 plane
	double f = wy / ( wy2 - wy );
	float x2d = wx - f * (wx2 - wx );
	float z2d = wz - f * (wz2 - wz );

	// Invertimos las x, ver nota al inicio de este fichero
	return Vector2d(-x2d, z2d);
}

// Obtiene las coordenadas 2D sobre la pantalla a partir de una posicion 3D en el mundo
Vector2d GraphicsEngineTAG::getScreenCoordinatesFrom3DPosition(Vector3d position)
{
	// Tenemos en cuenta que estamos en MODELVIEW y se acaba de cargar la identidad
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	GLdouble wx, wy, wz;  /*  returned world x, y, z coords  */
	glGetIntegerv (GL_VIEWPORT, viewport);
	glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
	// Aqui hay que invertir la x, ver nota al principio del fichero
	gluProject(-position.x, position.y, position.z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
	
	// Corregir la 'y' ya que project devuelve la posicion desde abajo hacia arriba de la pantalla
	wy = windowSize.y - wy;

	return Vector2d((float)wx, (float)wy);
}

void GraphicsEngineTAG::changeNodeColor(NodeMesh* node, int debugColor)
{

}

void GraphicsEngineTAG::drawDebugLine(Vector3d start, Vector3d end, int color /*= DebugColor::WHITE*/)
{
	//glColor3f(1.0, 1.0, 1.0);
	glLineWidth(1.5f);

		glBegin(GL_LINES);
		glVertex3f(-start.x, start.y, start.z);
		glVertex3f(-end.x, end.y, end.z);
		glEnd();
	}

// Dibuja un grid mediante lineas. Usa buffer VBO
void GraphicsEngineTAG::drawGrid(float size, float separation)
{
	int gridLines = size / separation; // num de lineas a dibujar
	float ground = 0;


	// Generamos el VBO
	if ( gridVBO == 0 )
	{
		std::vector<float> gridPoints;

		for (int i = 0; i < gridLines; i++) {
			// Linea
			// Punto 1
			gridPoints.push_back(0);
			gridPoints.push_back(ground);
			gridPoints.push_back(-i * separation);
			// Punto 2
			gridPoints.push_back(-size);
			gridPoints.push_back(ground);
			gridPoints.push_back(-i * separation);

			// Linea
			// Punto 1
			gridPoints.push_back(-i * separation);
			gridPoints.push_back(ground);
			gridPoints.push_back(0);
			// Punto 2
			gridPoints.push_back(-i * separation);
			gridPoints.push_back(ground);
			gridPoints.push_back(-size);
		}

		gridNumPoints = gridPoints.size();

		// Crea el VBO
		glGenBuffers(1, &gridVBO);
		glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*gridNumPoints, &gridPoints[0], GL_STATIC_DRAW);

		gridPoints.clear();
	}

	glPushMatrix();

	glEnableClientState(GL_VERTEX_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
	glVertexPointer( 3, GL_FLOAT, 0, 0 );	// Posiciones

	// Color rojo
	glColor3f(1.0, 0.3, 0.24);

	glDrawArrays(GL_LINES, 0, gridNumPoints);

	// bind with 0, so, switch back to normal pointer operation
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}


NodeMesh* GraphicsEngineTAG::addCube(Vector2d position, Vector2d size, int color, bool isMap /*= false*/)
{
	Vector3d scale = Vector3d(size.x, size.x, size.x);
	// Crea los nodos del arbol y los agrega como una nueva rama
	NodeMesh* nodeMesh = addNodeMesh(Resources::MESH_CUBE);
	nodeMesh->setPosition(position);
	nodeMesh->setScale(scale);

	return nodeMesh;
}

void GraphicsEngineTAG::createDebugMap(MapManager::CollisionMap &collisionMap)
{
	// Deshabilitado por problemas de rendimiento
	/*
	MapManager* mapManager = gameManager->getMapManager();
	int size = mapManager->collisionMap.frameSize - 0.1; // Para dejar un espacio entre cubos

	for(int i = 0; i < collisionMap.numFrames; i++)
	{
		for(int j = 0; j < collisionMap.numFrames; j++)
		{
			Vector2d positionOnGrid = mapManager->getFrameCenter(i,j);
			if( !mapManager->isEmpty(i, j) )
			{
				addCube(positionOnGrid, Vector2d(size, size), DebugColor::GREY, true);	
			}
		}
	}
	*/
}

void GraphicsEngineTAG::setMenuFocus(int id)
{

}

void GraphicsEngineTAG::setMenuPressed( int id )
{

}


void GraphicsEngineTAG::setNextMenuFocus(bool reverse /*= false*/)
{
	/*
	// start the search at the root of the current tab group
	irr::gui::IGUIElement *focus = guiManager->getFocus();

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
		startPos = guiManager->getRootGUIElement();
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
		guiManager->setFocus(next);
	}
	*/
}

void GraphicsEngineTAG::createDebugText()
{
	debugText = guiManager->addText(Vector2d(20, 60), L"");
}

void GraphicsEngineTAG::setDebugText(const wchar_t* text)
{
	debugText->setString(text);
}

void GraphicsEngineTAG::createInventory()
{
	inventory = guiManager->addImage(Vector2d(windowSize.x / 2 - 290, windowSize.y - 150), GUI::HUD_IMAGE_INVENTORY);
		
	//cambio vida y escudo
	GUISprite *hpBarEmpty, *shBarEmpty; //barras de vida y escudo vacias (las declaro aqui porque no se van a tocar en ningun lado)
	hpBarEmpty = guiManager->addImage(Vector2d(windowSize.x / 2 - 160, 25), GUI::HUD_IMAGE_HP_EMPTY);
	shBarEmpty = guiManager->addImage(Vector2d(windowSize.x / 2 - 5, 25), GUI::HUD_IMAGE_SH_EMPTY);
		
	hpBar = guiManager->addImage(Vector2d(windowSize.x / 2 - 160, 25), GUI::HUD_IMAGE_HP);
	shBar = guiManager->addImage(Vector2d(windowSize.x / 2 - 5, 25), GUI::HUD_IMAGE_SH);
		
	//hpBar->setScaleImage(false);
	//shBar->setScaleImage(false);
	
	shText = guiManager->addText( Vector2d(windowSize.x/2+270-180, 20+60), L"" );
	hpText = guiManager->addText( Vector2d(windowSize.x/2+270-430, 20+60), L"" );
		
	Vector2d parentPosition = inventory->getPosition();

	Vector2d iconSize(GUI::HUD_ICON_WIDTH, GUI::HUD_ICON_HEIGHT);

	// En irrlicht. Los siguientes botones tienen activado el setNotClipped: creation1->setNotClipped(true); etc.
	// Ademas todos los botones tiene como parent a "inventory"
	creation1  = guiManager->addButton(parentPosition.x + 30, parentPosition.y + 62, iconSize.x, iconSize.y, BUTTON_ID::SKILL1, L"1");
	creation1->setImage(GUI::HUD_IMAGE_ICON_TURRET_2);
	creation2 = guiManager->addButton(parentPosition.x + 92, parentPosition.y + 62, iconSize.x, iconSize.y, BUTTON_ID::SKILL2, L"2");
	creation2->setImage(GUI::HUD_IMAGE_ICON_MINE);
	creation3  = guiManager->addButton(parentPosition.x + 154, parentPosition.y + 62, iconSize.x, iconSize.y, BUTTON_ID::SKILL3, L"3");
	creation3->setImage(GUI::HUD_IMAGE_ICON_WALL);
	creation4 = guiManager->addButton(parentPosition.x + 214, parentPosition.y + 62, iconSize.x, iconSize.y, BUTTON_ID::SKILL4, L"4");
	creation4->setImage(GUI::HUD_IMAGE_ICON_TURRET_2);
	skill1 = guiManager->addButton(parentPosition.x + 317, parentPosition.y + 62, iconSize.x, iconSize.y, BUTTON_ID::SKILL5, L"5");
	skill1->setImage(GUI::HUD_IMAGE_ICON_TRIPLE);
	skill2 = guiManager->addButton(parentPosition.x + 379, parentPosition.y + 62, iconSize.x, iconSize.y, BUTTON_ID::SKILL6, L"6");
	skill2->setImage(GUI::HUD_IMAGE_ICON_MISSILE);
	skill3 = guiManager->addButton(parentPosition.x + 441, parentPosition.y +  62, GUI::HUD_ICON_WIDTH, 50, BUTTON_ID::SKILL7, L"7");
	skill3->setImage(GUI::HUD_IMAGE_ICON_RAY);
	skill4 = guiManager->addButton(parentPosition.x + 501, parentPosition.y +  62, GUI::HUD_ICON_WIDTH, 50, BUTTON_ID::SKILL8, L"8");
	skill4->setImage(GUI::HUD_IMAGE_ICON_RAY);
}

void GraphicsEngineTAG::addInventoryLogo(int skill,const char* resource)
{
	switch (skill)
	{
	case 0:
		creation1->setImage(resource);
		break;	
	case 1:
		creation2->setImage(resource);
		break;	
	case 2:
		creation3->setImage(resource);
		break;	
	case 3:
		creation4->setImage(resource);
		break;	
	case 4:
		skill1->setImage(resource);
		break;	
	case 5:
		skill2->setImage(resource);
		break;
	case 6:
		skill3->setImage(resource);
		break;
	case 7:
		skill4->setImage(resource);
		break;
	}
}

bool GraphicsEngineTAG::canSeeTarget( Vector2d start, Vector2d end )
{
	return true;
}

void GraphicsEngineTAG::putBegin(bool put)
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

void GraphicsEngineTAG::createHudCharacter()
{
	character = NULL;

	switch(gameManager->getGameObjectManager()->getMainPlayer()->getName())
	{
	case GameObject::N_BATTLESHIP_PLAYER:
		guiManager->addImage(Vector2d(0, windowSize.y - 210) , GUI::HUD_IMAGE_ICON_BATTLESHIP );
		break;
	case GameObject::N_BOMBER_PLAYER:
		guiManager->addImage(Vector2d(0, windowSize.y - 210) , GUI::HUD_IMAGE_ICON_BOMBER );
		break;
	case GameObject::N_FIGHTER_PLAYER:
		guiManager->addImage(Vector2d(0, windowSize.y - 210) , GUI::HUD_IMAGE_ICON_FIGHTER );
		break;
	case GameObject::N_ENGINEER_PLAYER:
		guiManager->addImage(Vector2d(0, windowSize.y - 210) , GUI::HUD_IMAGE_ICON_ENGINEER );
		break;
	}

	expBar = guiManager->addImage(Vector2d(32, windowSize.y - 16), GUI::HUD_IMAGE_EXPBAR);
	//expBar->setScaleImage(true);
	character = guiManager->addImage(Vector2d(0, windowSize.y - 210), GUI::HUD_IMAGE_PJ);
		
	level = guiManager->addText(Vector2d(11, windowSize.y - 21), L"");
	exp = guiManager->addText(Vector2d(50, windowSize.y - 20), L"");

	//Texo rojo de warning

	warningText = guiManager->addText( Vector2d(windowSize.x / 2 -75, windowSize.y - 200),L"");
	warningText->setFont("../assets/fonts/titillium-Bold.otf");
	warningText->setFontSize(20);
	warningText->setColor(186, 61, 61);
}

void GraphicsEngineTAG::setHudData( HUDData data )
{
	std::wstring aux = std::to_wstring(data.waves);
	panelWave->setString(aux.c_str());
	aux = std::to_wstring(data.clock.getElapsedTime()/60000);
	aux.append(L":");
	aux.append(std::to_wstring((data.clock.getElapsedTime()/1000)%60));
	panelClock->setString(aux.c_str());
	aux = std::to_wstring(data.jade);
	panelJade->setString(aux.c_str());
	aux = std::to_wstring(data.level);
	level->setString(aux.c_str());
	std::wstring aux2 = std::to_wstring(data.nextExp);
	aux = std::to_wstring(data.exp);
	aux.append(L"/");
	aux.append(aux2);
	exp->setString(aux.c_str());

	aux = std::to_wstring((int)data.health.x);
	aux.append(L"/");
	aux.append(std::to_wstring((int)data.health.y));
	hpText->setString(aux.c_str());

	aux = std::to_wstring((int)data.shield.x);
	aux.append(L"/");
	aux.append(std::to_wstring((int)data.shield.y));
	shText->setString(aux.c_str());

	//expBar->setMaxSize(irr::core::dimension2du(110*(float)data.exp/(float)data.nextExp,10));

	if(data.health.x > 0)
	{
		if(!hpBar->getVisible())
		{
			hpBar->setVisible(true);
		}
		unsigned int length = Math::max_(151*data.health.x/data.health.y, 1);
		//hpBar->setScale(length, 65);
		hpBar->setMaxSize(Vector2d(length,65));
	}
	else
	{
		hpBar->setVisible(false);
	}
	if(data.shield.x > 0)
	{
		if(!shBar->getVisible())
		{
			shBar->setVisible(true);
		}

		unsigned int length = Math::max_(151*data.shield.x/data.shield.y, 1);
		shBar->setMaxSize(Vector2d(length, 65));
	}
	else
	{
		shBar->setVisible(false);
	}

	/*if(!data.skills[0])
	{
		if(creation1->getEnabled()) 
		{
			creation1->setEnabled(false);
	
		}
	}
	else
	{
		if(!creation1->getEnabled())
		{
			creation1->setEnabled(true);
	
		}
	}
	//if(data.jade < _wtoi(creation2->getText()) || !(data.level >= 2))
	if(!data.skills[1])
	{
		if(creation2->getEnabled())
		{
			creation2->setEnabled(false);
	
		}
	}
	else
	{
		if(!creation2->getEnabled())
		{
			creation2->setEnabled(true);
	
		}
	}
	//if(data.jade < _wtoi(creation3->getText()) || !(data.level >= 4))
	if(!data.skills[2])
	{
		if(creation3->getEnabled())
		{
			creation3->setEnabled(false);
		}
	}
	else
	{
		if(!creation3->getEnabled())
		{	
			creation3->setEnabled(true);
		}
	}
	//if(data.jade < _wtoi(creation4->getText()) || !(data.level >= 6))
	if(!data.skills[3])
	{
		if(creation4->getEnabled())
		{	
			creation4->setEnabled(false);
		} 
	}
	else
	{
		if(!creation4->getEnabled())
		{
			creation4->setEnabled(true);
		}
	}

	if(!data.skills[4])
	{
		if(skill1->getEnabled())
		{	
			skill1->setEnabled(false);
		} 
	}
	else
	{
		if(!skill1->getEnabled())
		{
			skill1->setEnabled(true);
		}
	}

	if(!data.skills[5])
	{
		if(skill2->getEnabled())
		{	
			skill2->setEnabled(false);
		} 
	}
	else
	{
		if(!skill2->getEnabled())
		{
			skill2->setEnabled(true);
		}
	}

	if(!data.skills[6])
	{
		if(skill3->getEnabled())
		{	
			skill3->setEnabled(false);
		} 
	}
	else
	{
		if(!skill3->getEnabled())
		{
			skill3->setEnabled(true);
		}
	}

	if(!data.skills[7])
	{
		if(skill4->getEnabled())
		{	
			skill4->setEnabled(false);
		} 
	}
	else
	{
		if(!skill4->getEnabled())
		{
			skill4->setEnabled(true);
		}
	}*/
}

void GraphicsEngineTAG::createHudTopBar()
{
	GUISprite* panel = guiManager->addImage( Vector2d(windowSize.x / 2 - 158, 2), GUI::HUD_IMAGE_PANEL);
	
	// Tiempo
	panelClock = guiManager->addText( Vector2d(windowSize.x / 2 - 15 - 120, 8 + 15), L"" );
	panelClock->setFont("../assets/fonts/titillium-Bold.otf");
	panelClock->setFontSize(20);
	panelClock->setColor(250, 250, 250);
	
	// Jade
	panelJade = guiManager->addText( Vector2d(windowSize.x / 2 - 30, 8 + 20), L"" );
	panelJade->setFont("../assets/fonts/titillium-Bold.otf");
	panelJade->setFontSize(20);
	panelJade->setColor(250, 250, 250);

	// Ronda
	panelWave = guiManager->addText( Vector2d(windowSize.x / 2 + 95, 8 + 15), L"" );
	panelWave->setFont("../assets/fonts/titillium-Bold.otf");
	panelWave->setFontSize(20);
	panelWave->setColor(250, 250, 250);

	

	
	//panelJade->setTextAlignment(irr::gui::EGUIA_LOWERRIGHT,irr::gui::EGUIA_CENTER);
}

void GraphicsEngineTAG::createHUD()
{
	createHudCharacter();
	createHudMiniMap();
	createHudTopBar();
	createInventory();
}

void GraphicsEngineTAG::createHudMiniMap()
{
	miniMap = guiManager->addImage(Vector2d(windowSize.x - 208, windowSize.y - 200), GUI::HUD_IMAGE_MINIMAP);
}

unsigned int GraphicsEngineTAG::getUpdateTime()
{
	return updateTime;
}

unsigned int GraphicsEngineTAG::getUpdateVSTime()
{
	return updateVSTime;
}

void GraphicsEngineTAG::drawUpdateTimesRectangles( UpdateTimes ut )
{
	//Actualizamos los rectangulos segun el tiempo de cada update
	updateTimeRectangleSize(ut.gameObjectManagerTime, gomRect);

	updateTimeRectangleSize(ut.collisionManagerTime, cmRect);

	updateTimeRectangleSize(ut.graphicEngineTime, geRect);

	updateTimeRectangleSize(ut.debugToolsTime, dbgRect);

	updateTimeRectangleSize(ut.verticalSyncTime, vsRect);

	updateTimeRectangleSize(ut.aiFlockingTime, aifRect);	
}

void GraphicsEngineTAG::updateTimeRectangleSize( int time, GUISprite* rect )
{
	if(time <= 0) 
	{
		rect->setMaxSize(Vector2d(10,1));
	}
	else
	{
		rect->setMaxSize(Vector2d(10,time*100/50));
	}
}

void GraphicsEngineTAG::addDebugTimeRectangles()
{
	int x=windowSize.x/2 - 147;
	int y=100+65;
	int separation = GUI::DEBUG_RECTANGLE_SEPARATION;
	int recWidth = GUI::DEBUG_RECTANGLE_WIDTH;
	int recHeight = GUI::DEBUG_RECTANGLE_HEIGHT;

	const char* imageFile = GUI::DEBUG_IMAGE_RECTANGLE;

	gomRect = guiManager->addImage(Vector2d(x, y), imageFile, 27);
	gomRect->setMaxSize(Vector2d(recWidth, recHeight));
	gomRect->setVisible(false);

	cmRect = guiManager->addImage(Vector2d(x+separation, y), imageFile);
	cmRect->setMaxSize(Vector2d(recWidth, recHeight));
	cmRect->setVisible(false);

	geRect = guiManager->addImage(Vector2d(x+separation*2, y), imageFile);
	geRect->setMaxSize(Vector2d(recWidth, recHeight));
	geRect->setVisible(false);

	dbgRect = guiManager->addImage(Vector2d(x+separation*3, y), imageFile);
	dbgRect->setMaxSize(Vector2d(recWidth, recHeight));
	dbgRect->setVisible(false);

	vsRect = guiManager->addImage(Vector2d(x+separation*4, y), imageFile);
	vsRect->setMaxSize(Vector2d(recWidth, recHeight));
	vsRect->setVisible(false);

	aifRect = guiManager->addImage(Vector2d(x+separation*5, y), imageFile);
	aifRect->setMaxSize(Vector2d(recWidth, recHeight));
	aifRect->setVisible(false);


}

void GraphicsEngineTAG::setUpdateTimeRectanglesVisible( bool visible )
{
	gomRect->setVisible(visible);
	cmRect->setVisible(visible);
	geRect->setVisible(visible);
	dbgRect->setVisible(visible);
	vsRect->setVisible(visible);
	aifRect->setVisible(visible);
}

//Inicializa el skybox
void GraphicsEngineTAG::loadSkybox(Vector3d skyboxRotation)
{
	Skybox* skybox = sceneManager->addSkybox(
									"../assets/images/skybox/nebula_top_TAG.jpg", // Temporalmente hasta que sepamos como girarlo
									"../assets/images/skybox/nebula_bottom_TAG.jpg", // Temporalmente hasta que sepamos como girarlo
									"../assets/images/skybox/nebula_left.jpg",
									"../assets/images/skybox/nebula_right.jpg",
									"../assets/images/skybox/nebula_front.jpg",
									"../assets/images/skybox/nebula_back.jpg");

	// En irrlicht tambien esta animado en z, pero ahora mismo las caras de top y bottom estan mal orientadas
	// de esta manera no se nota. Ademas la velocidad de rotacion no es exactamente la misma que irrlicht
	skybox->setRotation(0.0003, 0, 0.0003);
}

void GraphicsEngineTAG::setWindowsCaption()
{
	if(fpsClock.getElapsedTime() > 500)
	{
		std::stringstream text;
		text << "Space Defenders FPS:";
		text << (int)(1/gameManager->getDeltaTime());
		window->setTitle(text.str());
		fpsClock.restart();
	}
}

// MINIMAPA -------------------------------------------------------------------------------------
int GraphicsEngineTAG::addGUISprite(const char* spriteFilename, Vector2d position, Vector2d size)
{
	int id  = getNextID();
	GUISprite* image = guiManager->addImage(position, spriteFilename, id);

	if(spriteFilename == GUI::MENU_IMAGE_LOADING)
	{
		image->setScale( size.x, size.y );
	}

	return id;
}

void GraphicsEngineTAG::setPositionGUISprite(int id, Vector2d position, Vector2d size)
{

	GUIElement*  sprite = guiManager->getElementFromId(id);
	
	if(sprite != NULL)
	{
		sprite->setPosition(position.x,position.y);
	}
	
}

void GraphicsEngineTAG::removeGUISprite(int id)
{
	guiManager->removeChildFromId(id);
	
}

void GraphicsEngineTAG::setColorGUISprite(int id, int color)
{
	GUISprite*  sprite = (GUISprite*)guiManager->getElementFromId(id);

	if(sprite != NULL)
	{
		sprite->setColor(color);
	}
}


void GraphicsEngineTAG::setScaleGUISprite(int id, Vector2d scale)
{
	GUISprite*  sprite = (GUISprite*)guiManager->getElementFromId(id);

	if(sprite != NULL)
	{
		sprite->setScale(scale.x,scale.y);
	}
}

void GraphicsEngineTAG::setVisibleGUISprite(int id,bool visible)
{
	GUIElement*  sprite = guiManager->getElementFromId(id);

	if(sprite != NULL)
	{
		sprite->setVisible(visible);	
	}
	
}

void GraphicsEngineTAG::clearGUI()
{
	// DEBUG
	debugText = NULL;

	// SETTINGS
	checkBoxFullScreen = NULL;
	checkBoxSync = NULL;
	checkBoxIrrlicht = NULL;
	checkBoxMusic = NULL;
	checkBoxReady = NULL;
	resolutionButton = NULL;

	// SERVER BROWSER
	tableServers = NULL;
	tablePlayers = NULL;

	begin = NULL;

	character = NULL;
	expBar = NULL;
	level = NULL;
	exp = NULL;
	miniMap = NULL;

	panelWave = NULL;
	panelClock = NULL;
	panelJade = NULL;
	warningText = NULL;

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

	inGameMenu.clear(); // El GUIManager elimina el contenido
}

// Comprueba si una extension es soportada por la grafica (copiado de opengl.org)
bool GraphicsEngineTAG::isExtensionSupported( const char* szTargetExtension )
{
	const unsigned char *pszExtensions = NULL;
	const unsigned char *pszStart;
	unsigned char *pszWhere, *pszTerminator;

	// Extension names should not have spaces
	pszWhere = (unsigned char *) strchr( szTargetExtension, ' ' );
	if( pszWhere || *szTargetExtension == '\0' )
		return false;

	// Get Extensions String
	pszExtensions = glGetString( GL_EXTENSIONS );

	// Search The Extensions String For An Exact Copy
	pszStart = pszExtensions;
	for(;;)
	{
		pszWhere = (unsigned char *) strstr( (const char *) pszStart, szTargetExtension );
		if( !pszWhere )
			break;
		pszTerminator = pszWhere + strlen( szTargetExtension );
		if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' )
			if( *pszTerminator == ' ' || *pszTerminator == '\0' )
				return true;
		pszStart = pszTerminator;
	}
	return false;
}

void GraphicsEngineTAG::addDescription(int id)
{
	Vector2d parentPosition = inventory->getPosition();

	float horizontalSpace = 32;
	Vector2d position = parentPosition + Vector2d(horizontalSpace, 120);

	if (description != NULL)
	{
		deleteDescription();
	}

	switch (id)
	{	
	case BUTTON_ID::SKILL1:
		description = guiManager->addText(position, descriptions[0].c_str());
		break;
	case BUTTON_ID::SKILL2:
		position.x += horizontalSpace * 2;
		description = guiManager->addText(position, descriptions[1].c_str());
		break;
	case BUTTON_ID::SKILL3:
		position.x += horizontalSpace * 3;
		description = guiManager->addText(position, descriptions[2].c_str());
		break;
	case BUTTON_ID::SKILL4:
		position.x += horizontalSpace * 4;
		description = guiManager->addText(position, descriptions[3].c_str());
		break;
	case BUTTON_ID::SKILL5:
		position.x += horizontalSpace * 6;
		description = guiManager->addText(position, descriptions[4].c_str());
		break;
	case BUTTON_ID::SKILL6:
		position.x += horizontalSpace * 7;
		description = guiManager->addText(position, descriptions[5].c_str());
		break;
	case BUTTON_ID::SKILL7:
		position.x += horizontalSpace * 8;
		description = guiManager->addText(position, descriptions[6].c_str());
		break;
	case BUTTON_ID::SKILL8:
		position.x += horizontalSpace * 9;
		description = guiManager->addText(position, descriptions[7].c_str());
		break;
	}
}

void GraphicsEngineTAG::deleteDescription()
{
	if(description != NULL)
	{
		guiManager->removeChild(description);
		description = NULL;
	}
}

void GraphicsEngineTAG::addDescriptionText(int skillNumber, std::wstring description)
{
	descriptions[skillNumber] = description;
}

void GraphicsEngineTAG::setWarningMessage( const wchar_t* warningMessage )
{
	warningText->setString(warningMessage);

	warningTextClock.start();
}

void GraphicsEngineTAG::updateWarningText()
{
	if(warningText != NULL && warningTextClock.getElapsedTime() > 1000)
	{
		warningText->setString(L"");
		warningTextClock.restart();
		warningTextClock.pause();
	}
}
