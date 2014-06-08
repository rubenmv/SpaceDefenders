#pragma once
#include "irrlicht/irrlicht.h"
#include <iostream>

#include "GameManager.h"
#include "GraphicsEngine.h"
#include "GraphicsEngineIrrlicht.h"


class ShieldShaderCallBack : public irr::video::IShaderConstantSetCallBack
{
public:

	//MyShaderCallBack::MyShaderCallBack( NodeMesh* nodeMesh );

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,	irr::s32 userData)
	{
	
		
		//Cogemos la capa de la textura
		int texture=0; 
		//mandamos un puntero de la textura
		services->setPixelShaderConstant("texture",(float*)(&texture),1);

		intensity = nodeMesh->getIntensity();
		services->setVertexShaderConstant("fIntensity",&intensity,1);
			
		//radius = nodeMesh->getRadius();
		services->setVertexShaderConstant("fRadius",&radius,1);

		//Pedimos los 4 impactos por ahora
		float*	impactListTemp =  nodeMesh->getImpactList();

		for (int i = 0; i<16;i++)
		{
			impactList[i] = impactListTemp[i];			
		}

		services->setVertexShaderConstant("mMatrixImpact", impactList, 16);
			

	}
	
	void setIntensity(float intensity)
	{
		this->intensity  = intensity;
	}

	void addNodeMesh(NodeMesh* nodeMesh)
	{
		this->nodeMesh = nodeMesh;
	}

	void setRadius(float radius)
	{
		this->radius = radius;	
	}

private:
	
	irr::f32 intensity;
	NodeMesh* nodeMesh;
	Vector2d positionProjectile;
	float impactList[16];
	float radius;

};

