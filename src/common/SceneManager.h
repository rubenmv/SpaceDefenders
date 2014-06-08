#pragma once

#include <iostream>

#include "SceneNode.h"
#include "Skybox.h"

class EntityCamera;
class EntityLight;

// Mantiene los estados de proyeccion de openGL
// Guarda la raiz de la escena y llama al dibujado
class SceneManager
{
public:

	static int nodeCount;

	enum PassMode
	{
		NONE,
		LIGHTS,		// Camaras
		CAMERAS,
		OBJECTS			// Objetos, mallas, texturas...
	} passMode;

	SceneManager();
	virtual~SceneManager();
	
	// Agrega una nueva rama al nodo raiz
	void addBranch(SceneNode* node)
	{
		node->setParent(root);
		root->addChild(node);
	}

	void setActiveCamera(SceneNode* camera)
	{
		activeCamera = camera;
	}

	// Agrega una luz a la lista
	void addLight(SceneNode* light)
	{
		lights.push_back(light);
	}

	// Llama al dibujado del grafo, comenzando por el nodo raiz
	void draw();
	void clear();

	// Inicializa el skybox
	Skybox* addSkybox(	const char* top,
						const char* bottom,
						const char* left,
						const char* right,
						const char* front,
						const char* back
						);
private:
	// Nodo raiz del grafo de escena
	SceneNode* root;

	// Lista de luces
	std::vector<SceneNode*> lights;
	// Camara activa
	SceneNode* activeCamera;

	Skybox* skybox;
};