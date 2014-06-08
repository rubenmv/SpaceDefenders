#pragma once

#include <string>
#include <glew/glew.h> // Se usa un VBO
#include <SFML/Graphics.hpp>
#include "Clock.h"
#include "Vector3d.h"
#include "Math.h"
	
class ShaderProgramTAG;

class Skybox
{
public:
	Skybox(	const char* top,
			const char* bottom,
			const char* left,
			const char* right,
			const char* front,
			const char* back
		  );

	virtual ~Skybox();

	// Dibuja el skybox
	void draw();

	void setRotation(float x, float y, float z)
	{
		rotation = Vector3d(x, y, z);
	}

private:
	enum Skybox_Sides // Para identificar mas facilmente
	{
		SKY_RIGHT,
		SKY_LEFT,
		SKY_FRONT,
		SKY_BACK,
		SKY_TOP,
		SKY_BOTTOM,
		SIDES_COUNT
	};

	sf::Image sides[SIDES_COUNT]; // top, bottom, left, right, front, back
	ShaderProgramTAG* shader;
	// IDs de buffers
	GLuint verticesID;
	GLuint indicesID;
	GLuint textureID;
	// Posicion del vertice para compartir con el shader
	GLint vertex;
	// Numero de indices, para el drawElements
	unsigned int indicesSize;
	// Reloj para la rotacion
	Clock rotationClock;
	// Rotation a aplicar
	Vector3d rotation;

	// Crea las texturas en OpenGL
	void genTextures();
	// Linka el programa con los shaders
	void initShader();
	// Crea y genera los buffers de vertices
	void genBuffers();
};

