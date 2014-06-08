#pragma once

#include "ResourceMesh.h"
class ResourceTexture;

class ResourceMaterial
{
public:
	ResourceMaterial( )
	{
		// Constructor por defecto para herencias
	}

	ResourceMaterial( ResourceMesh::material_t material, bool transparent );
	~ResourceMaterial();

	virtual void bind();
	virtual void unbind();

	virtual void setAmbientColor(unsigned int color);
protected:
	ResourceMesh::material_t material;
	ResourceTexture* texture;
	bool transparent;
};

