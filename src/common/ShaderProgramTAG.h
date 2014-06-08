#pragma once

#include "Resource.h"

#include <string>
#include <glew/glew.h>

class ShaderProgramTAG : public Resource
{
public:
	ShaderProgramTAG();
	~ShaderProgramTAG();

	enum Type
	{
		VERTEX,
		FRAGMENT,
		GEOMETRY
	};

	GLuint getProgramID()
	{
		return programID;
	}

	// Carga un shader desde fichero y lo linka al programa
	bool loadFromFile(ShaderProgramTAG::Type type, const char* filePath);
	// Linka un programa y comprueba que no haya errores
	void linkProgram();
	// Carga el programa para su uso
	void bind();
	// Descarga el programa despues de su uso
	void unbind();

	int getUniformLocation(const char* variable);
	// Setters para los uniforms, solo los que se usen
	void setUniformf(const char* name, float var);
	void setUniformf(int location, float var);
	void setUniformf(const char* name, float var1, float var2);
	void setUniformf(int location, float var1, float var2);
	void setUniformi(const char* name, int var);
	void setUniformi(int location, int var);

private:
	unsigned int programID;
	// Lee un fichero en un string y lo devuelve
	std::string readFile(const char* filePath);
	// Compila un shader leido desde fichero
	GLuint compileFromSource(GLenum type, const char *filePath);
	// Agrega un shader a un programa
	void shaderAttachFromFile(GLuint program, GLenum type, const char *filePath);
};
