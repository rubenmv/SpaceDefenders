#pragma once

#include "irrlicht/irrlicht.h"
#include <iostream>


#include "GameManager.h"
#include "GraphicsEngine.h"
#include "GraphicsEngineIrrlicht.h"

class TerrainShaderCallBack : public irr::video::IShaderConstantSetCallBack
{
public:
	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,	irr::s32 userData)
	{
		
		//Cogemos la capa de la textura
		int mask=0; 

		//mandamos un puntero de la textura
		services->setPixelShaderConstant("mask",(&mask),1);

		//Cogemos la capa de la textura
		int red_tile=1; 
		//mandamos un puntero de la textura
		services->setPixelShaderConstant("red_tile",(&red_tile),1);

		//Cogemos la capa de la textura
		int green_tile=2; 
		//mandamos un puntero de la textura
		services->setPixelShaderConstant("green_tile",(&green_tile),1);

		//Cogemos la capa de la textura
		int blue_tile=3; 
		//mandamos un puntero de la textura
		services->setPixelShaderConstant("blue_tile",(&blue_tile),1);


	}

	void addNodeMesh(NodeMesh* nodeMesh)
	{
		this->nodeMesh = nodeMesh;
	}

private:

	NodeMesh* nodeMesh;
};

