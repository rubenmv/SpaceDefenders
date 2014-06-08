#include "MaterialShield.h"

#include "GameManager.h"
#include "ResourceManager.h"

#include "ResourceTexture.h"
#include "ShaderProgramTAG.h"
#include "EntityMesh.h" // Para recoger valores de intensidad, etc.

#include <SFML/OpenGL.hpp>
#include "GameObjectManager.h"

MaterialShield::MaterialShield( ResourceMesh::material_t material, bool transparent, EntityMesh* parent, float radius )
{
	this->parent = parent;
	this->material = material;
	this->transparent = transparent;

	ResourceManager* resourceManager = GameManager::getInstance()->getResourceManager();
	// Textura
	if(!material.diffuse_texname.empty())
	{
		// Indicamos que active el binding OpenGL, smooth nearest y repeat
		texture = resourceManager->getTexture(material.diffuse_texname.c_str(), true);
	}

	shader = resourceManager->getShader(Resources::SHADER_SHIELD_VERT, Resources::SHADER_SHIELD_FRAG);
	shader->bind();
	intensity = glGetUniformLocation(shader->getProgramID(), "fIntensity");
	impactList = glGetUniformLocation(shader->getProgramID(), "mMatrixImpact");
	this->radius = radius; 
	uRadius = glGetUniformLocation(shader->getProgramID(), "fRadius");			
	shader->unbind();
}

MaterialShield::~MaterialShield()
{
	// La textura la limpia el gestor de recursos
	texture = NULL;
	shader = NULL;
	parent = NULL;
}

void MaterialShield::bind()
{
	if (transparent)
	{
		glDepthMask(GL_FALSE);
	}

	if (shader != NULL)
	{
		// Actualiza el shader y lo activa
		shader->bind();

		glUniform1f(intensity, parent->getIntensity());
		glUniformMatrix4fv(impactList, 1, false, parent->getImpactList());
		glUniform1f(uRadius, radius);
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

void MaterialShield::unbind()
{
	// Asi se evita que modelos sin textura apliquen la ultima textura
	if (texture != NULL)
	{
		texture->unbind();
	}

	if (shader != NULL)
	{
		// Actualiza el shader y lo activa
		shader->unbind();
	}

	if (transparent)
	{
		glDepthMask(GL_TRUE);
	}
}

void MaterialShield::setAmbientColor( unsigned int color )
{
	material.ambient[0] = 0;
	material.ambient[0] = 0;
	material.ambient[0] = 0;
}
