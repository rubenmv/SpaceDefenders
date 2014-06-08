#pragma once

#include "FXEngine.h"
#include <irrlicht/irrlicht.h>

class GraphicsEngineIrrlicht;

class FXEngineIrrlicht : public FXEngine
{
public:
	FXEngineIrrlicht(irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver* driver);
	virtual ~FXEngineIrrlicht();

	void addParticleToNode(irr::scene::IMeshSceneNode* irrlichtNode, ParticleEffect effect);
	void damageEffect( bool keep );
private:
	irr::scene::ISceneManager* sceneManager;
	irr::video::IVideoDriver* driver;
};


