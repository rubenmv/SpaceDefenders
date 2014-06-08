#pragma once

#include "ResourceMaterial.h"

class EntityMesh;
class ShaderProgramTAG;

class MaterialTerrain : public ResourceMaterial
{
public:
	MaterialTerrain( ResourceMesh::material_t material, bool transparent, EntityMesh* parent );
	~MaterialTerrain();

	void bind();
	void unbind();

	void setAmbientColor(unsigned int color);

private:
	ShaderProgramTAG* shader;
	ResourceTexture* textures[4]; // Mask, R, G, B
	EntityMesh* parent;
	// Para recoger la localizacion para la textura activa
	GLuint texLoc;
};

