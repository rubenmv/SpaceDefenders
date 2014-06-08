#include "MaterialTerrain.h"

#include "GameManager.h"
#include "ResourceManager.h"

#include "ResourceTexture.h"
#include "ShaderProgramTAG.h"
#include "EntityMesh.h" // Para recoger valores de intensidad, etc.

#include <SFML/OpenGL.hpp>
#include "GameObjectManager.h"

MaterialTerrain::MaterialTerrain( ResourceMesh::material_t material, bool transparent, EntityMesh* parent )
{
	this->parent = parent;
	this->material = material;
	this->transparent = transparent;

	ResourceManager* resourceManager = GameManager::getInstance()->getResourceManager();

	// Recogemos los id de las texturas
	textures[0] = resourceManager->getTexture("../assets/models/map/mask.png", true);
	textures[1] = resourceManager->getTexture("../assets/models/map/red_tile.png", true);
	textures[2] = resourceManager->getTexture("../assets/models/map/green_tile.png", true);
	textures[3] = resourceManager->getTexture("../assets/models/map/blue_tile.png", true);

	shader = NULL;

	shader = resourceManager->getShader(Resources::SHADER_TERRAIN_VERT, Resources::SHADER_TERRAIN_FRAG);
}

MaterialTerrain::~MaterialTerrain()
{
	// La textura la limpia el gestor de recursos
	texture = NULL;
	shader = NULL;
	parent = NULL;
}

void MaterialTerrain::bind()
{
	if (transparent)
	{
		glDepthMask(GL_FALSE);
	}

	shader->bind();

	// Mask
	glActiveTexture( GL_TEXTURE0 );
	textures[0]->bind();
	texLoc = glGetUniformLocation(shader->getProgramID(), "mask");
	glUniform1i(texLoc, 0);

	// Red
	glActiveTexture( GL_TEXTURE1 ); 
	textures[1]->bind();
	texLoc = glGetUniformLocation(shader->getProgramID(), "red_tile");
	glUniform1i(texLoc, 1);

	// Green
	glActiveTexture( GL_TEXTURE2 ); 
	textures[2]->bind();
	texLoc = glGetUniformLocation(shader->getProgramID(), "green_tile");
	glUniform1i(texLoc, 2);

	// Blue
	glActiveTexture( GL_TEXTURE3 ); 
	textures[3]->bind();
	texLoc = glGetUniformLocation(shader->getProgramID(), "blue_tile");
	glUniform1i(texLoc, 3);
	
	glColor3f(material.ambient[0], material.ambient[1], material.ambient[2]);
	//glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient );
	//glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, material.specular );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, material.emission );
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess );
}

void MaterialTerrain::unbind()
{
	// Debemos desactivar las texturas una a una para que no afecten a otros objetos
	// La textura que hace unbind da igual, todas hacen lo mismo
	glActiveTexture( GL_TEXTURE1 ); 
	textures[1]->unbind();
	glActiveTexture( GL_TEXTURE2 ); 
	textures[2]->unbind();
	glActiveTexture( GL_TEXTURE3 ); 
	textures[3]->unbind();
	// La 0 la dejamos la ultima porque es la que queremos activa
	glActiveTexture( GL_TEXTURE0 );
	textures[0]->unbind();

	shader->unbind();

	if (transparent)
	{
		glDepthMask(GL_TRUE);
	}
}

void MaterialTerrain::setAmbientColor( unsigned int color )
{
	material.ambient[0] = 0;
	material.ambient[0] = 0;
	material.ambient[0] = 0;
}
