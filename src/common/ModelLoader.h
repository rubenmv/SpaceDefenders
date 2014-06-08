#pragma once

#include "ResourceMesh.h"

class ModelLoader
{
public:
	ModelLoader();
	virtual ~ModelLoader();

	// Carga una malla en formato OBJ
	ResourceMesh* loadOBJ(const char *meshFileName);
};


