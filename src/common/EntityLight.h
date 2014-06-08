#pragma once

#include "Entity.h"
#include <SFML/OpenGL.hpp>

#include "Vector3d.h"

class EntityLight: public Entity
{
public:
	enum Type
	{
		DIRECTIONAL,	// 0
		POINT			// 1
	};

	EntityLight();
	virtual ~EntityLight();

	bool isActive()
	{
		return active;
	}

	void setActive(bool active)
	{
		this->active = active;
	}

	void setType(int type)
	{
		this->type = (GLfloat)type;
	}

	void setPosition(Vector3d position)
	{
		this->position = position;
	}

	void setGLLight(GLenum glLight)
	{
		this->glLight = glLight;
	}

	// Ambiente
	void setAmbientColor(float r, float g, float b, float a)
	{
		ambientColor[0] = r;
		ambientColor[1] = g;
		ambientColor[2] = b;
		ambientColor[3] = a;

		glLightfv(glLight, GL_AMBIENT, ambientColor);
	}

	// Difusa
	void setDiffuseColor(float r, float g, float b, float a)
	{
		diffuseColor[0] = r;
		diffuseColor[1] = g;
		diffuseColor[2] = b;
		diffuseColor[3] = a;

		glLightfv(glLight, GL_DIFFUSE, diffuseColor);
	}

	// Especular
	void setSpecularColor(float r, float g, float b, float a)
	{
		specularColor[0] = r;
		specularColor[1] = g;
		specularColor[2] = b;
		specularColor[3] = a;

		glLightfv(glLight, GL_SPECULAR, specularColor);
	}

	void beginDraw();
	void endDraw();

private:
	bool active;
	GLfloat type; // 0.0 = direccional, 1.0 = punto
	Vector3d position;
	GLenum glLight;

	// Propiedades
	GLfloat ambientColor[4];
	GLfloat diffuseColor[4];
	GLfloat specularColor[4];
};

