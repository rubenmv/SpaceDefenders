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
		int mask=0; 

		//mandamos un puntero de la textura
		services->setPixelShaderConstant("fbo_texture",(&mask),1);

		
		
		services->setVertexShaderConstant("u_resolution", reinterpret_cast<irr::f32*>(&resolution), 2);

		services->setVertexShaderConstant("u_intensity",&intensity,1);

		services->setVertexShaderConstant("u_radius",&radius,1);


	}

	
	void setIntensity( float intensity ) 
	{
		this->intensity = intensity;
	}

	void setRadius( float radius ) 
	{
		this->radius = radius;
	}

	void setResolution( irr::core::dimension2d<irr::u32> resolution ) 
	{
		this->resolution = resolution;
	}




private:
	
	// Para el efecto de damage
	float radius;
	float intensity;
	irr::core::dimension2d<irr::u32> resolution;
};

