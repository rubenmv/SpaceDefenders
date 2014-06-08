#pragma once

#include "Vector2d.h"
#include "Vector3d.h"

#include "Entity.h"


namespace Material 
{
	enum MaterialEnum
	{
		SHIELD = 0,
		TERRAIN,
		OTHER,
		_MATERIAL_COUNT
	};
}

class NodeMesh
{
public:
	NodeMesh(){};
	virtual ~NodeMesh(){};
	
	virtual void setPosition(Vector3d position) = 0;

	virtual void setPosition(Vector2d position) = 0;

	virtual void setRotation(float rotation) = 0;

	virtual void setRotation(Vector3d inclination) = 0;

	virtual void setScale(Vector3d scale) = 0;

	virtual void setScale(float factor) = 0;

	virtual void setColor(unsigned int color) = 0;

	virtual void setVisible(bool visible) = 0;

	//Cambia la intensidad del shader del escudo
	virtual void setIntensity(float intensity) = 0;

	virtual float getIntensity() = 0;

	virtual void setImpactList( float* impactList ) = 0;

	virtual float* getImpactList() = 0;


};
