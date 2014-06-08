#pragma once

#include "ResourceMaterial.h"

class EntityMesh;
class ShaderProgramTAG;

class MaterialShield : public ResourceMaterial
{
public:
	MaterialShield( ResourceMesh::material_t material, bool transparent, EntityMesh* parent, float radius = 1 );
	~MaterialShield();

	void bind();
	void unbind();

	void setAmbientColor(unsigned int color);

private:
	ShaderProgramTAG* shader;
	// IDs donde guardar las localizaciones de las uniform
	int intensity;
	int impactList;
	int positionProjectile;
	float radius;
	float uRadius;
	EntityMesh* parent;
};

