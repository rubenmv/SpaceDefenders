#include "SceneManager.h"

#include "SceneNode.h"
#include "EntityTransform.h"
#include "EntityCamera.h"
#include "EntityLight.h"
#include "Vector3d.h"

int SceneManager::nodeCount = 0;

SceneManager::SceneManager()
{
	root = new SceneNode();
	root->setEntity(NULL);
	root->setParent(NULL);
	activeCamera = NULL;

	skybox = NULL;
}

SceneManager::~SceneManager()
{
	// Limpia el arbol
	clear();

	if (root != NULL)
	{
		delete root;
		root = NULL;
	}
	if (skybox != NULL)
	{
		delete skybox;
		skybox = NULL;
	}
}

// Llama al dibujado del grafo, comenzando por el nodo raiz
void SceneManager::draw()
{
	// Primero el skybox, asi no hace falta pasarle posicion de la camara
	if (skybox != NULL)
	{
		skybox->draw();
	}

	SceneNode* node = NULL;

	// CAMARA
	passMode = PassMode::CAMERAS;
	if (activeCamera != NULL)
	{
		// Recogemos la camara activa
		node = activeCamera;

		// Subimos hasta la raiz
		while (node->getParent() != root)
		{
			node = node->getParent();
		}

		// node->parent == root, comenzamos el dibujado
		node->draw();
	}

	// Aqui acumulamos las transformaciones para las luces
	// que se aplican directamente en la hoja
	Vector3d position;

	// LUCES
	passMode = PassMode::LIGHTS;
	EntityTransform* transformEntity = NULL;
	// Para cada luz activa, subir hasta la raiz
	for (size_t i = 0; i < lights.size(); i++)
	{
		node = lights[i];

		// Recoger la entidad para saber si la luz esta activa
		EntityLight* lightEntity = (EntityLight*)node->getEntity();
		if (lightEntity->isActive())
		{
			// Subimos hasta la raiz sumando las transformaciones
			// da igual que se sumen hacia arriba ya que es un vector3
			// que al final aplicamos en la entidad de la hoja
			while (node->getParent() != root)
			{
				node = node->getParent();
				transformEntity = (EntityTransform*)node->getEntity();
				position += transformEntity->getTranslation();
			}

			// Aplicamos las transformaciones acumuladas
			lightEntity->setPosition(position);
			// Y llamamos directamente al dibujado de la entidad
			lightEntity->beginDraw();
		}
	}

	// OBJETOS
	passMode = PassMode::OBJECTS;
	// No llamamos al draw del nodo raiz ya que su entity es NULL
	// Directamente iteramos por sus hijos
	std::vector<SceneNode*> children = root->getChildren();
	if (!children.empty())
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->draw();
		}
	}
	passMode = PassMode::NONE;
}

// Limpia el arbol completo
void SceneManager::clear()
{
	// Recordar que le root no tiene parent por lo que no se borrara a si mismo
	root->clear();
	// La camara ya ha sido borrada del arbol
	activeCamera = NULL;
	lights.clear(); // El root->clear ya ha eliminado (delete) los nodos de luz
}

Skybox* SceneManager::addSkybox( const char* top, const char* bottom, const char* left, const char* right, const char* front, const char* back )
{
	skybox = new Skybox(top, bottom, left, right, front, back);

	return skybox;
}


