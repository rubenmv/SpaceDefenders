#include "ResourceMesh.h"

#include "ResourceManager.h"
#include "GameManager.h"
#include "GraphicsEngineTAG.h"
#include "ResourceTexture.h"
#include "ResourceMaterial.h"

#include <SFML/OpenGL.hpp>


ResourceMesh::ResourceMesh( shape_t model )
{
	positionsID = 0;
	indicesID = 0;
	normalsID = 0;
	texcoordsID = 0;

	this->model = model;

	GraphicsEngineTAG* graphicsEngine = (GraphicsEngineTAG*)GameManager::getInstance()->getGraphicsEngine();
	useVBO = graphicsEngine->isVBOSupported();

	// Generar los VBO
	if (useVBO)
	{
		createVBO();
	}
}

ResourceMesh::~ResourceMesh()
{
	glDeleteBuffers(1, &positionsID);
	glDeleteBuffers(1, &indicesID);
	glDeleteBuffers(1, &normalsID);
	glDeleteBuffers(1, &texcoordsID);
}

void ResourceMesh::createVBO()
{
	// VBOs
	glGenBuffers(1, &positionsID);
	glBindBuffer(GL_ARRAY_BUFFER, positionsID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*model.mesh.positions.size(), &model.mesh.positions[0], GL_STATIC_DRAW);

	if ( !model.mesh.normals.empty() )
	{
		glGenBuffers(1, &normalsID);
		glBindBuffer(GL_ARRAY_BUFFER, normalsID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*model.mesh.normals.size(), &model.mesh.normals[0], GL_STATIC_DRAW);
	}
	
	if ( !model.mesh.texcoords.empty() )
	{
		glGenBuffers(1, &texcoordsID);
		glBindBuffer(GL_ARRAY_BUFFER, texcoordsID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*model.mesh.texcoords.size(), &model.mesh.texcoords[0], GL_STATIC_DRAW);
	}

	glGenBuffers(1, &indicesID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*model.mesh.indices.size(), &model.mesh.indices[0], GL_STATIC_DRAW);

	// bind with 0, so, switch back to normal pointer operation
	glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
}

// Dibujado en OpenGL
void ResourceMesh::draw()
{
	if ( !model.mesh.positions.empty() )
	{
		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_NORMAL_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );

		if (useVBO)
		{
			glBindBuffer(GL_ARRAY_BUFFER, positionsID);
			glVertexPointer( 3, GL_FLOAT, 0, 0 );	// Posiciones

			if ( !model.mesh.normals.empty() )
			{
				glBindBuffer(GL_ARRAY_BUFFER, normalsID);
				glNormalPointer( GL_FLOAT, 0, 0 );		// Normales
			}

			if ( !model.mesh.texcoords.empty() )
			{
				glBindBuffer(GL_ARRAY_BUFFER, texcoordsID);
				glTexCoordPointer( 2, GL_FLOAT, 0, 0 );	// uv
			}

			// GL_ELEMENT_ARRAY_BUFFER para indices indica que cada indice apunta a
			// un GL_ARRAY_BUFFER (eso creo)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
			glDrawElements( GL_TRIANGLES, model.mesh.indices.size(), GL_UNSIGNED_INT, (void*)0 );

			// bind with 0, so, switch back to normal pointer operation
			glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
		}
		else
		{
			glVertexPointer( 3, GL_FLOAT, 0, &model.mesh.positions[0] );	// Posiciones

			if ( !model.mesh.normals.empty() )
			{
				glNormalPointer( GL_FLOAT, 0, &model.mesh.normals[0] );		// Normales
			}

			if ( !model.mesh.texcoords.empty() )
			{
				glTexCoordPointer( 2, GL_FLOAT, 0, &model.mesh.texcoords[0] );	// uv
			}

			glDrawElements( GL_TRIANGLES, model.mesh.indices.size(), GL_UNSIGNED_INT, &model.mesh.indices[0] );
		}
	
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_VERTEX_ARRAY );
	}	
}
