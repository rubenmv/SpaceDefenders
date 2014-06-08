#include "ShaderProgramTAG.h"

#include <fstream>
#include <iostream>

ShaderProgramTAG::ShaderProgramTAG()
{
	programID = glCreateProgram();
}

ShaderProgramTAG::~ShaderProgramTAG()
{
	glDeleteProgram(programID);
}

bool ShaderProgramTAG::loadFromFile( ShaderProgramTAG::Type type, const char* filePath )
{
	GLuint shader = 0;
	GLuint shaderType;

	switch (type)
	{
	case ShaderProgramTAG::VERTEX:
		shaderType = GL_VERTEX_SHADER;
		break;
	case ShaderProgramTAG::FRAGMENT:
		shaderType = GL_FRAGMENT_SHADER;
		break;
	case ShaderProgramTAG::GEOMETRY:
		shaderType = GL_GEOMETRY_SHADER;
		break;
	default:
		std::cerr << "loadFromFile(): tipo de shader no identificado" << std::endl;
		return false;
	}

	shader = compileFromSource(shaderType, filePath);

	if(shader != 0) {
		// Agrega el shader al programa
		glAttachShader(programID, shader);
		// Esto no elimina el shader en si, ya que esta en el programa
		glDeleteShader(shader);
	}

	return true;
}

void ShaderProgramTAG::bind()
{
	glUseProgram(programID);
}

void ShaderProgramTAG::unbind()
{
	glUseProgram(0);
}

std::string ShaderProgramTAG::readFile( const char* filePath )
{
	std::string contents;

	std::ifstream in(filePath, std::ios::in);
	if (in)
	{
		in.seekg(0, std::ios::end);
		contents.resize( static_cast<unsigned int>(in.tellg()) );
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();		
	}

	return contents;
}

GLuint ShaderProgramTAG::compileFromSource(GLenum type, const char *filePath)
{
	std::string source;
	GLuint shader;
	GLint length, result;

	// Lee el fichero a un string
	source = readFile(filePath);
	if(source.empty())
	{
		std::cerr << "Hubo un problema cargando desde el fichero: " << filePath << std::endl;
		return 0;
	}

	// Crea el shader y lo compila
	shader = glCreateShader(type);
	length = source.size();

	const char *c_str = source.c_str();
	glShaderSource(shader, 1, &c_str, &length);
	glCompileShader(shader);

	// Comprueba si la compilacion ha sido correcta
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE) {
		char *log;

		/* get the shader info log */
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		log = (char*)malloc(length);
		glGetShaderInfoLog(shader, length, &result, log);

		/* print an error message and the info log */
		std::cerr << "compileFromSource(): No se puede compilar" << filePath << ": " << log << std::endl;
		free(log);

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void ShaderProgramTAG::linkProgram()
{
	GLint result;
	/* link the program and make sure that there were no errors */
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	if(result == GL_FALSE) {
		GLint length;
		char *log;

		/* get the program info log */
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
		log = (char*)malloc(length);
		glGetProgramInfoLog(programID, length, &result, log);

		/* print an error message and the info log */
		std::cerr << "linkProgram(): Fallo al linkar el programa: " << log << std::endl;
		free(log);

		/* delete the program */
		glDeleteProgram(programID);
		programID = 0;
	}
}

int ShaderProgramTAG::getUniformLocation( const char* variable )
{
	if (programID != 0)
	{
		return glGetUniformLocation(programID, variable);
	}
	
	return 0;
}

void ShaderProgramTAG::setUniformf( int location, float var )
{
	glUniform1f( location, var );
}

void ShaderProgramTAG::setUniformf( int location, float var1, float var2 )
{
	glUniform2f( location, var1, var2 );
}

void ShaderProgramTAG::setUniformf( const char* name, float var )
{
	glUniform1f( glGetUniformLocation(programID, name), var );
}

void ShaderProgramTAG::setUniformf( const char* name, float var1, float var2 )
{
	glUniform2f( glGetUniformLocation(programID, name), var1, var2 );
}

void ShaderProgramTAG::setUniformi( int location, int var )
{
	glUniform1i(location, var); 
}

void ShaderProgramTAG::setUniformi( const char* name, int var )
{
	glUniform1i( glGetUniformLocation(programID, name), var );
}
