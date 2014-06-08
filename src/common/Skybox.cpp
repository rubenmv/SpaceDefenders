#include "Skybox.h"
#include "ShaderProgramTAG.h"
#include "Vector2d.h"
#include <SFML/Window.hpp>

Skybox::Skybox( const char* top, const char* bottom, const char* left, const char* right, const char* front, const char* back )
{
	sides[SKY_LEFT].loadFromFile(left); // x negativo
	sides[SKY_RIGHT].loadFromFile(right); // x positivo
	sides[SKY_BOTTOM].loadFromFile(bottom); // y negativo
	sides[SKY_TOP].loadFromFile(top); // y positivo
	sides[SKY_BACK].loadFromFile(back); // z negativo
	sides[SKY_FRONT].loadFromFile(front);	// z positivo

	// Crea las texturas OpenGL
	genTextures();
	// Inicializa el shader
	initShader();
	// Crea y genera los buffers de vertices
	genBuffers();
}

Skybox::~Skybox()
{
	// Limpiar los buffers de indices, vertices y texturas
	glDeleteBuffers(1, &verticesID);
	glDeleteBuffers(1, &indicesID);
	glDeleteTextures(1, &textureID);

	delete shader;
	shader = NULL;
}

void Skybox::genTextures()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Todas las imagenes tienen que tener el mismo size
	Vector2d size(sides[SKY_FRONT].getSize().x, sides[SKY_FRONT].getSize().y);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, sides[SKY_RIGHT].getPixelsPtr()); 
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, sides[SKY_LEFT].getPixelsPtr()); 
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, sides[SKY_TOP].getPixelsPtr()); 
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, sides[SKY_BOTTOM].getPixelsPtr()); 
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, sides[SKY_FRONT].getPixelsPtr()); 
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, sides[SKY_BACK].getPixelsPtr()); 
}

void Skybox::initShader()
{
	shader = new ShaderProgramTAG;
	shader->loadFromFile(ShaderProgramTAG::VERTEX, "../assets/shaders/skybox/skybox.vert");
	shader->loadFromFile(ShaderProgramTAG::FRAGMENT, "../assets/shaders/skybox/skybox.frag");
	shader->linkProgram();
}

void Skybox::genBuffers()
{
	GLuint program = shader->getProgramID();

	// Recoge la posicion de los vertices desde el programa
	vertex = glGetAttribLocation(program, "vertex");

	// VBO para el cubo, vertices e indices
	GLfloat cube_vertices[] = {
		-1.0,  1.0,  1.0,
		-1.0, -1.0,  1.0,
		1.0, -1.0,  1.0,
		1.0,  1.0,  1.0,
		-1.0,  1.0, -1.0,
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0,  1.0, -1.0,
	};

	glGenBuffers(1, &verticesID);
	glBindBuffer(GL_ARRAY_BUFFER, verticesID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertex);
	glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLushort cube_indices[] = {
		0, 1, 2, 3,
		3, 2, 6, 7,
		7, 6, 5, 4,
		4, 5, 1, 0,
		0, 3, 7, 4,
		1, 2, 6, 5,
	};

	indicesSize = sizeof(cube_indices);

	glGenBuffers(1, &indicesID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(vertex);
}

void Skybox::draw()
{
	glPushMatrix();
		unsigned int deltaTime = rotationClock.getElapsedTime();
		// Aplica la rotacion
		glRotatef((GLfloat)(deltaTime * rotation.x), 1, 0, 0);
		glRotatef((GLfloat)(deltaTime * rotation.y), 0, 1, 0);
		glRotatef((GLfloat)(deltaTime * rotation.z), 0, 0, 1);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);

		glEnable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		glBindBuffer(GL_ARRAY_BUFFER, verticesID);
		glEnableVertexAttribArray(vertex);
		glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

		shader->bind();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
		glDrawElements(GL_QUADS, indicesSize/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

		shader->unbind();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_CULL_FACE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(vertex);
		glDisable(GL_TEXTURE_CUBE_MAP);
	glPopMatrix();
}
