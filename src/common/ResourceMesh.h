#pragma once

#include "Resource.h"
#include <string>
#include <vector>
#include <map>

#include <glew/glew.h>

class ResourceTexture;
class ResourceMaterial;

// Contiene la estructura de una malla
// y realiza el dibujado de esta
class ResourceMesh : public Resource
{
public:
	// Tipos de datos para el modelo
	typedef struct
	{
		std::string name;

		float ambient[3];
		float diffuse[3];
		float specular[3];
		float transmittance[3];
		float emission[3];
		float shininess;
		float ior; // indice de refraccion
		int shader; // ID del shader a utilizar

		std::string ambient_texname;
		std::string diffuse_texname;
		std::string specular_texname;
		std::string normal_texname;
		std::map<std::string, std::string> unknown_parameter;
	} material_t;

	typedef struct
	{
		std::vector<float>          positions;
		std::vector<float>          normals;
		std::vector<float>          texcoords;
		std::vector<unsigned int>   indices;
	} mesh_t;

	typedef struct
	{
		std::string  name;
		material_t   material;
		mesh_t       mesh;
	} shape_t;

	ResourceMesh(shape_t model);
	virtual~ResourceMesh();

	shape_t	model;

	// Dibuja la malla en OpenGL
	void draw();

	// Genera los VBO
	void createVBO();

private:
	// Indica si se utilizan buffers VBO
	bool useVBO;

	// IDs para los buffers VBO
	GLuint positionsID;
	GLuint indicesID;
	GLuint normalsID;
	GLuint texcoordsID;
};
