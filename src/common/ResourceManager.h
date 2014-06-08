#pragma once

#include <vector>

class Resource;
class ResourceMesh;
class ResourceTexture;
class ResourceSoundBuffer;
class ShaderProgramTAG;

class ResourceManager
{
public:
	ResourceManager();
	virtual~ResourceManager();

	// Limpia todos los recursos
	void clear();
	// Devuelve un ResourceMesh/ResourceTexture, lo carga si no estaba en memoria
	ResourceMesh* getMesh(const char* filePath);
	// El binding solo se debe activar para las texturas que no van dirigidas al GUI o HUD (2D), las que se dibujan
	// mediante windows->draw(textura)
	ResourceTexture* getTexture(const char* filePath, bool bind = false, bool linear = false, bool repeat = true);
	// Devuelve un shader program para TAG
	ShaderProgramTAG* getShader(const char* vertShader, const char* fragShader);

private:
	// Carga un ResourceMesh/ResourceTexture y guarda una referencia a este
	ResourceMesh* loadMesh(const char* filePath);

	// Guarda una referencia al ultimo recurso pedido para mayor rapidez
	// evita busquedas seguidas del mismo recurso
	Resource* lastResource;

	// Lista de recursos cargados
	std::vector<Resource*> resources;
	Resource* getResource(const char* filePath);
};