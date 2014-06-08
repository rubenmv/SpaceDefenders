#include "ModelLoader.h"

#include "tiny_obj_loader.h"

#include <iostream>
#include <fstream>

class ResourceMesh;

ModelLoader::ModelLoader(){}

ModelLoader::~ModelLoader(){}

// Carga un modelo OBJ desde fichero
ResourceMesh* ModelLoader::loadOBJ(const char* meshFileName)
{
	ResourceMesh* resourceMesh = NULL;
	
	std::ifstream fileIn(meshFileName);

	// Path base del modelo donde encontrar el mtl
	std::string path = meshFileName;
	std::string bp;
	// Extraemos el path
	for (int i = path.size()-1 ; i > 0; i--)
	{
		if (path.at(i) == '/')
		{
			bp = path.substr(0, i+1);
			break;
		}
	}
	// Y de nuevo a const char*
	const char* basePath = bp.c_str();

	if(!fileIn.is_open())
	{
		std::cout << "No se puede abrir el fichero: " << meshFileName << std::endl;
		return NULL;
	}
	else
	{
		std::vector<ResourceMesh::shape_t> tempModel;
		// Carga el modelo OBJ con el tiny_obj_loader al modelo del recurso
		// Parametros: Modelo donde se guarda, direccion/filename del obj, basepath para el mtl
		std::string err = tinyobj::LoadObj(tempModel, meshFileName, basePath);
		if (!err.empty())
		{
			std::cerr << err << std::endl;
			return NULL;
		}
		// IMPORTANTE: Debemos asegurarnos que el modelo fue exportado todo como un mismo objeto
		resourceMesh = new ResourceMesh( tempModel[0] );
		tempModel.clear();

		fileIn.close();
	}

	return resourceMesh;
}
