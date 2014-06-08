#include "ResourceTexture.h"
#include "GameManager.h"
#include "GraphicsEngineTAG.h"
#include <iostream>

#include <SFML/OpenGL.hpp>

ResourceTexture::ResourceTexture(const char* filePath, bool bind, bool linear, bool repeat)
{
	graphicsEngine = (GraphicsEngineTAG*)GameManager::getInstance()->getGraphicsEngine();
	//guardamos el filePath
	fileName = filePath;

	this->binding = bind;

	if(!image.loadFromFile(filePath))
	{
		std::cout << "ERROR: No se pudo encontrar la imagen "<< filePath << std::endl;
	}
	else
	{
		imageSize.x = image.getSize().x;
		imageSize.y = image.getSize().y;

		// Se carga la textura en OpenGL
		if (binding)
		{
			// Generamos la textura en OpenGL
			glGenTextures(1, &textureID);
			this->bind();

			GraphicsEngineTAG* graphicsEngine = (GraphicsEngineTAG*)GameManager::getInstance()->getGraphicsEngine();

			// Para graficas con soporte para storage de texturas
			// Uso esta funcion en vez de glew ya que este no soporta "GL_ARB_texture_storage" sino "ARB_texture_storage"
			// pero si "GL_ARB_framebuffer_object" lo cual es un lio absurdo
			if ( graphicsEngine->isExtensionSupported("GL_ARB_texture_storage") &&
				 graphicsEngine->isExtensionSupported("GL_ARB_framebuffer_object") )
			{
				//std::cout << "glTexStorage2D supported" << std::endl;
				GLsizei level = 4;
				glTexStorage2D(GL_TEXTURE_2D, level, GL_RGBA8, image.getSize().x, image.getSize().y);
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)image.getSize().x, (GLsizei)image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
				glGenerateMipmap(GL_TEXTURE_2D);  //Generate num_mipmaps number of mipmaps here.
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
			// Version sin soporte para glTexStorage2D
			else if ( graphicsEngine->isExtensionSupported("GL_ARB_framebuffer_object") )
			{
				//std::cout << "glTexStorage2D not supported" << std::endl;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)image.getSize().x, (GLsizei)image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
				glGenerateMipmap(GL_TEXTURE_2D);  // Genera los mipmaps
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
			// Version legacy, para casi cualquier version de opengl
			else
			{
				//std::cout << "legacy " << std::endl;
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)image.getSize().x, (GLsizei)image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
			}
			
			this->unbind();
		}
		else // Para elementos 2D se hace uso de la textura de SFML
		{
			texture = new sf::Texture();
			texture->loadFromImage(image);
		}
	}
}

ResourceTexture::~ResourceTexture(void)
{
	if (binding)
	{
		glDeleteTextures(1, &textureID);
	}
	delete texture;
	texture = NULL;
	graphicsEngine = NULL;
}

void ResourceTexture::bind()
{
	if (graphicsEngine->getTextures())
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		glEnable( GL_TEXTURE_2D );
	}
}

void ResourceTexture::unbind()
{
	glDisable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, NULL);
}
