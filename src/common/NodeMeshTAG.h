#pragma once

#include "NodeMesh.h"
#include "Entity.h"

#include "GameManager.h"
#include "GraphicsEngine.h"
#include "SceneNode.h"
#include "EntityTransform.h"
#include "EntityMesh.h"


// Malla que guarda los vertices, normales y caras
class NodeMeshTAG: public NodeMesh
{
public:
	NodeMeshTAG(SceneNode* node)
	{
		this->node = node;
		mesh = (EntityMesh*)node->getEntity();
		transform = (EntityTransform*)node->getParent()->getEntity();
	}

	virtual ~NodeMeshTAG()
	{
		delete node;

		node = NULL;
		mesh = NULL;
		transform = NULL;
	}

	// Establece la posicion 3D del gameObject
	void setPosition(Vector3d position)
	{
		// Recoge el nodo de transformacion y aplica la transformacion
		transform->setTranslation(position);
	}

	// Establece la posicion 2D del gameObject
	void setPosition(Vector2d position)
	{
		transform->setTranslation(position);
	}

	// Establece la rotacion (en y) del gameObject
	void setRotation(float rotation)
	{
		transform->setRotation(Vector3d(0, rotation, 0));
	}

	// Establece la inclinacion del gameObject
	void setRotation(Vector3d inclination)
	{
		transform->setRotation(inclination);
	}

	// Escala cada eje por separado
	void setScale(Vector3d scale)
	{
		transform->setScale(scale);
	}

	// Escala todos los ejes por igual
	void setScale(float factor)
	{
		transform->setScale(factor);
	}

	void setColor(unsigned int color)
	{
		mesh->setColor(color);
	}

	void setNode(SceneNode* node)
	{
		this->node = node;
	}

	void setVisible(bool visible)
	{
		//Se cambia el visible del nodo de transformacion, por eso se accede al parent
		node->setVisible(visible);
	}
	
	//Cambia la intensidad del shader del escudo
	void setIntensity(float intensity)
	{
		mesh->setIntensity(intensity);
	}

	float getIntensity()
	{
		return mesh->getIntensity();
	}

	//Recibe lista de impactos
	void setImpactList( float* impactList )
	{
		mesh->setImpactList(impactList);
	}

	float* getImpactList() 
	{
		return mesh->getImpactList();
	}

/*	virtual float getRadius()
	{
		return 1; //radius;
	}*/

private:
	SceneNode* node;
	EntityMesh* mesh;
	EntityTransform* transform; // Entidad transformacion asociada
};

