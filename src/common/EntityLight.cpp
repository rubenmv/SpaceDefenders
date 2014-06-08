#include "EntityLight.h"

EntityLight::EntityLight()
{
	active = false;
	position = Vector3d(0,0,0);
	glLight = GL_LIGHT0; // Por defecto la 0
	type = (GLfloat)Type::DIRECTIONAL;
}

EntityLight::~EntityLight()
{
	glDisable(glLight);
}

void EntityLight::beginDraw()
{
	if (active)
	{
		// Luz basica
		glEnable(glLight);
		
		// Posicion
		GLfloat pos[] = {position.x, position.y, position.z, type};
		glLightfv(glLight, GL_POSITION, pos);
	}
	else
	{
		glDisable(glLight);
	}
}

void EntityLight::endDraw()
{

}

