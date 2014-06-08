#include "EntityMesh.h"

#include "GameManager.h"
#include "ResourceManager.h"
#include "NodeMesh.h"
#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "MaterialShield.h"
#include "MaterialTerrain.h"

#include <iostream>

EntityMesh::EntityMesh(const char* meshFileName, bool transparent,float radius)
{
	ResourceManager* resourceManager = GameManager::getInstance()->getResourceManager();
	// Pide el recurso de malla
	mesh = resourceManager->getMesh(meshFileName);

	intensity = 0;

	if ( mesh != NULL )
	{
		int shaderType = mesh->model.material.shader;

		if (shaderType < 0) // Material por defecto
		{
			material = new ResourceMaterial( mesh->model.material, transparent );
		}
		else // Material con shaders
		{
			switch (shaderType)
			{
			case Material::SHIELD:
				material = new MaterialShield( mesh->model.material, transparent, this, radius );
				break;
			case Material::TERRAIN:
				material = new MaterialTerrain(mesh->model.material, transparent, this);
				break;
			}
		}
	}
}

EntityMesh::~EntityMesh()
{
	// El recurso malla sigue cargado en memoria para otras entidades
	mesh = NULL;
	// El material es unico por entidad (pero la textura que contiene se comparte entre materiales)
	if (material != NULL)
	{
		delete material;
		material = NULL;
	}
}

void EntityMesh::beginDraw()
{
	if (mesh != NULL)
	{
		material->bind();
		mesh->draw();
		material->unbind();
	}
}

void EntityMesh::endDraw()
{
}

void EntityMesh::setColor( unsigned int color )
{
	material->setAmbientColor(color);
}

//Cambia la intensidad del shader del escudo
void EntityMesh::setIntensity(float intensity)
{
	this->intensity = intensity;
}

float EntityMesh::getIntensity()
{
	return intensity;
}

//Recibe lista de impactos
void EntityMesh::setImpactList( float* impactList )
{
	for (int i = 0; i<16;i++)
	{
		this->impactList[i] = impactList[i];
	}	
}

float* EntityMesh::getImpactList() 
{
	return impactList;
}