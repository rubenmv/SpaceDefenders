#include "ResourceManager.h"

#include "GameObjectManager.h" // Para Resources::MESH_ERROR
#include "ModelLoader.h"
#include "ResourceTexture.h"
#include "ShaderProgramTAG.h"

#include <vector>
#include <string>

ResourceManager::ResourceManager()
{
	lastResource = NULL;
}

ResourceManager::~ResourceManager()
{
	clear();
}

void ResourceManager::clear()
{
	while (!resources.empty())
	{
		delete resources.back();
		resources.pop_back();
	}
	resources.clear();

	lastResource = NULL;
}

// Devuelve un ResourceMesh, lo carga si no estaba en memoria
ResourceMesh* ResourceManager::getMesh(const char* filePath)
{
	ResourceMesh* mesh = (ResourceMesh*) getResource(filePath);

	if(mesh == NULL)
	{
		mesh = loadMesh(filePath);
		resources.push_back(mesh);
	}

	return mesh;
}

// Carga un ResourceMesh y guarda una referencia a este
ResourceMesh* ResourceManager::loadMesh(const char* filePath)
{
	ResourceMesh* mesh = NULL;
	ModelLoader* modelLoader = new ModelLoader();

	mesh = modelLoader->loadOBJ(filePath);

	// Si hay algun problema con el modelo cargamos uno de error por defecto
	if (mesh == NULL)
	{
		mesh = modelLoader->loadOBJ(Resources::MESH_ERROR);
	}

	mesh->fileName = filePath; //guardamos el filePath

	return mesh;
}


// Devuelve un ResourceTexture, lo carga si no estaba en memoria
ResourceTexture* ResourceManager::getTexture(const char* filePath, bool bind, bool linear, bool repeat)
{
	ResourceTexture* texture = (ResourceTexture*) getResource(filePath);

	if(texture == NULL)
	{
		texture = new ResourceTexture(filePath, bind, linear, repeat);
		resources.push_back(texture);
	}

	return texture;
}

// Busca y devuelve o crea un shader. Para los shaders ya creados se toma como referencia el
// nombre del fichero del vertex shader (.vert)
ShaderProgramTAG* ResourceManager::getShader( const char* vertShader, const char* fragShader )
{
	ShaderProgramTAG* shader = (ShaderProgramTAG*) getResource(vertShader);

	if(shader == NULL)
	{
		shader = new ShaderProgramTAG();
		shader->loadFromFile(ShaderProgramTAG::VERTEX, vertShader);
		shader->loadFromFile(ShaderProgramTAG::FRAGMENT, fragShader);
		shader->linkProgram();
		// El filename hace referencia al .vert
		shader->fileName = vertShader;
		resources.push_back(shader);
	}

	return shader;
}

// Busca en la lista de recursos mediante el filePath
Resource* ResourceManager::getResource(const char* filePath)
{
	bool found = false;
	Resource* resource = NULL;
	
	// Primero comprobamos si el recurso acaba de ser accedido anteriormente
	if (lastResource != NULL)
	{
		if ( strcmp( lastResource->fileName.c_str(), filePath) == 0 )
		{
			found = true;
			resource = lastResource;
		}
	}
	// Si el recurso que se pide es diferente al ultimo que se pidio lo buscamos en el vector
	if (!found && !resources.empty())
	{
		for (size_t i = 0; i < resources.size() && !found; i++)
		{
			if( strcmp( resources[i]->fileName.c_str(), filePath) == 0 )
			{
				found = true;
				resource = resources[i];
				lastResource = resource;
			}
		}
	}

	return resource;
}



