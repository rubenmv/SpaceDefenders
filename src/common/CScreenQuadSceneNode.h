#pragma once

#include "irrlicht/irrlicht.h"
#include "Shader.h"
#include "FilterShaderCallBack.h"

class CScreenQuadSceneNode : public irr::scene::ISceneNode
{
public:
	enum ParticleEffect
	{
		FIRE,
		FIRE_LOW,
		FIRE_AND_SMOKE_LOW,
		FIRE_AND_SMOKE_MEDIUM,
		FIRE_AND_SMOKE_HEAVY,
		FIGHTER_ENGINE,
		ENGINER_RIGHT_ENGINE,
		ENGINER_LEFT_ENGINE,
		BOMBER_ENGINE,
		BATTLESHIP_ENGINE,
		SMOKE,
		SMOKE_LOW,
		SPARK,
		PARTICLE_EFFECT_COUNT
	};

	enum PostEffect
	{
		DAMAGE,		// Efecto de vignette + oscurecido por daño al jugador
		GLOW,		// Bordes brillantes
		MINIME,		//
		BW,			// Blanco y negro
		BLUR,		// Emborronado, duplica y agrega offset
		BILLBOARD,	// El de circulitos
		NONE		// Sin efectos, el passthrough
	};

	CScreenQuadSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* sceneManager, irr::s32 id);
	virtual~CScreenQuadSceneNode();

	const irr::core::aabbox3df& getBoundingBox() const
	{
		return aabb;
	}

	void render();

	irr::u32 getMaterialCount()
	{
		return 1;	//There is only one material
	}

	irr::video::SMaterial& getMaterial(irr::u32 i)
	{
		return material;//We always return the same material, so there is no need for more.
	}

	void damageEffect( bool keep );

	//Carga el material, pero el nombre lo he cogido por si lo pasamos a FXIrrlicht cuando funcione
	void bindPostProcessing();

	void reset();

private:
	static const float FADEIN_STEP;
	static const float FADEOUT_STEP;

	irr::core::aabbox3df aabb;					//An axis aligned bounding box. Actually not needed.
	irr::video::SMaterial material;				
	irr::video::S3DVertex2TCoords vertices[4]; 	//The vertices of the Scene Node.

	// Shader activo
	int activeEffect;
	
	// Los shaders en este vector tienen que crearse ordenados segun
	// el enum de FXEngine.h (PostEffect)
	std::vector<irr::s32> shaders;
	// Para el efecto de damage
	float radius;
	float intensity;
	bool finished; // Indica si el efecto ha terminado
	FilterShaderCallBack* shaderCallback;
	bool keepActive; // Para saber si el efecto es permanente
};