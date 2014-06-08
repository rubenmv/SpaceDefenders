#pragma once

#include "irrlicht/irrlicht.h"
#include <iostream>


#include "GameManager.h"
#include "GraphicsEngine.h"
#include "GraphicsEngineIrrlicht.h"

class FilterShaderCallBack : public irr::video::IShaderConstantSetCallBack
{
public:
	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,	irr::s32 userData)
	{
		
		//Cogemos la capa de la textura
		int mask=1; 
	}

	void addNodeMesh(NodeMesh* nodeMesh)
	{
		this->nodeMesh = nodeMesh;
	}

private:

	NodeMesh* nodeMesh;
};

