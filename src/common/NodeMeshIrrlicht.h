#pragma once

#include "NodeMesh.h"
#include "irrlicht/irrlicht.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "ShieldShaderCallBack.h"

class NodeMeshIrrlicht: public NodeMesh
{
public:
	NodeMeshIrrlicht(irr::scene::IMeshSceneNode* irrlichtNode):irrlichtNode(irrlichtNode){};
	virtual ~NodeMeshIrrlicht()
	{
		irrlichtNode->remove();
	}

	irr::scene::IMeshSceneNode* getIrrlichtNode()
	{
		return(irrlichtNode);
	}

	// Establece la posicion en el espacio 3d
	void setPosition(Vector3d position)
	{
		irrlichtNode->setPosition(position);
	}

	// Establece la posicion en el plano 2d
	void setPosition(Vector2d position)
	{
		irrlichtNode->setPosition(position.asVector3d());
	}

	// Rota en el eje 'y'
	void setRotation(float rotation)
	{
		irrlichtNode->setRotation(Vector3d(0, rotation, 0));
	}

	// Agrega otra rotacion a la rotacion actual (normalmente x,0,z)
	void setRotation(Vector3d inclination)
	{
		irrlichtNode->setRotation(inclination);
	}

	// Escala cada eje por separado
	void setScale(Vector3d scale)
	{
		irrlichtNode->setScale(scale);
	}

	// Escala todos los ejes por igual
	void setScale(float factor)
	{
		irrlichtNode->setScale(irrlichtNode->getScale()*factor);
	}

	// Establece el color del nodo
	void setColor(unsigned int color)
	{
		GameManager::getInstance()->getGraphicsEngine()->changeNodeColor(this, color);
	}

	void setVisible(bool visible)
	{
		irrlichtNode->setVisible(visible);
	}

	//Cambia la intensidad del shader del escudo
	virtual void setIntensity(float intensity)
	{
		this->intensity = intensity;
	}

	virtual float getIntensity()
	{
		return intensity;
	}

	//Recibe lista de impactos
	virtual void setImpactList( float* impactList )
	{
		for (int i = 0; i<16;i++)
		{
			this->impactList[i] = impactList[i];
		}	

	}
	virtual float* getImpactList() 
	{
		return impactList;
	}	

	

private:

	irr::scene::IMeshSceneNode* irrlichtNode;
	float intensity;
	//Lista de impactos
	float impactList[16];

};
