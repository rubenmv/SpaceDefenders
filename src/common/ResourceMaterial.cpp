#include "ResourceMaterial.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "ResourceTexture.h"

#include <SFML/OpenGL.hpp>
#include <iostream>

ResourceMaterial::ResourceMaterial( ResourceMesh::material_t material, bool transparent )
{
	this->transparent = transparent;
	this->material = material;
	// Textura
	if(!material.diffuse_texname.empty())
	{
		ResourceManager* resourceManager = GameManager::getInstance()->getResourceManager();
		// Indicamos que active el binding OpenGL, smooth nearest y repeat
		texture = resourceManager->getTexture(material.diffuse_texname.c_str(), true);
	}
}

ResourceMaterial::~ResourceMaterial()
{
	// La textura la limpia el gestor de recursos
	texture = NULL;
}

void ResourceMaterial::bind()
{
	if (transparent)
	{
		glDepthMask(GL_FALSE);
	}

	if (texture != NULL)
	{
		texture->bind();
	}

	glColor3f(material.ambient[0], material.ambient[1], material.ambient[2]);
	//glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient );
	//glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, material.specular );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, material.emission );
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess );
}

void ResourceMaterial::unbind()
{
	// Asi se evita que modelos sin textura apliquen la ultima textura
	if (texture != NULL)
	{
		texture->unbind();
	}

	if (transparent)
	{
		glDepthMask(GL_TRUE);
	}
}

void ResourceMaterial::setAmbientColor( unsigned int color )
{
	/*
	material.ambient[0] = 0;
	material.ambient[0] = 0;
	material.ambient[0] = 0;
	*/
}
