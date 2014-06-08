#pragma once
#include <irrlicht/irrlicht.h>

class FXEngine
{
public:
	enum ParticleEffect {
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

	enum PostEffect {
		DAMAGE,		// Efecto de vignette + oscurecido por daño al jugador
		GLOW,		// Bordes brillantes
		MINIME,		//
		BW,			// Blanco y negro
		BLUR,		// Emborronado, duplica y agrega offset
		BILLBOARD,	// El de circulitos
		NONE		// Sin efectos, el passthrough
	};

	FXEngine() {}
	virtual ~FXEngine() {}

	virtual void damageEffect( bool keep ) {};
	// Estos no los pongo como puros por si irrlicht resulta que no los vaya a utilizar
	virtual void bindPostProcessing() {}
	virtual void unbindPostProcessing() {}
	//virtual void addParticleToNode(irr::scene::IMeshSceneNode* irrlichtNode, ParticleEffect effect) {}
};


