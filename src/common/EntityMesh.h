#pragma once

#include "Entity.h"

class ResourceMesh;
class ResourceMaterial;

class EntityMesh: public Entity
{
public:
	EntityMesh(const char* meshFileName, bool transparent, float radius = 1);
	virtual ~EntityMesh();

	void beginDraw();
	void endDraw();

	void setColor(unsigned int color);

	//Cambia la intensidad del shader del escudo
	void setIntensity(float intensity);

	float getIntensity();

	//Recibe lista de impactos
	void setImpactList( float* impactList );

	float* getImpactList();

private:
	ResourceMesh* mesh;
	ResourceMaterial* material;

	float intensity;
	//Lista de impactos
	float impactList[16];
};
