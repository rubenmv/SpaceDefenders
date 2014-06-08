#pragma once

#include "Resource.h"
#include "Vector2d.h"

#include <glew/glew.h>
#include <SFML/Graphics.hpp>

class GraphicsEngineTAG;

class ResourceTexture : public Resource
{
public:
	ResourceTexture(const char* filePath, bool bind = false, bool linear = false, bool repeat = true);
	~ResourceTexture(void);
	
	void bind();
	void unbind();

	sf::Texture* getSFMLTexture()
	{
		return this->texture;
	}


	Vector2d getImageSize()
	{

		return	imageSize;
	}

private:
	sf::Texture* texture;
	sf::Image image;
	Vector2d imageSize;
	// Para bindear en OpenGL
	GLuint textureID;

	// Indica si la textura se ha de bindear
	bool binding;
	// Indica si se aplica suavizado linear
	bool linear;
	// Indica si la textura se repite
	bool repeat;

	GraphicsEngineTAG* graphicsEngine;
};

