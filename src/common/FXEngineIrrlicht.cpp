#pragma once

#include "FXEngineIrrlicht.h"
#include "GameObjectManager.h"
#include "GameManager.h"
#include "GraphicsEngineIrrlicht.h"

FXEngineIrrlicht::FXEngineIrrlicht( irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver* driver )
{
	this->sceneManager = sceneManager;
	this->driver = driver;
}

FXEngineIrrlicht::~FXEngineIrrlicht()
{
	sceneManager = NULL;
	driver = NULL;
}

//Asocia un efecto de particulas a un nodo
void FXEngineIrrlicht::addParticleToNode(irr::scene::IMeshSceneNode* irrlichtNode, ParticleEffect effect)
{	
	irr::scene::IParticleSystemSceneNode* ps;
	irr::scene::IParticleSystemSceneNode* ps2;
	irr::scene::IParticleEmitter* em;
	irr::scene::IParticleEmitter* em2;
	irr::scene::IParticleAffector* paf;

	float pX = -2;
	float pY = 0;
	float pZ = 0;

	if (effect >= 0 && effect < FXEngine::ParticleEffect::PARTICLE_EFFECT_COUNT)
	{
		// Sistema de particulas basico para todos los efectos
		ps = sceneManager->addParticleSystemSceneNode(false,irrlichtNode);
		ps->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		ps->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
		ps->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);

		switch (effect)
		{
		case FXEngine::ParticleEffect::FIRE:

			ps->setScale(irr::core::vector3df(0.01,0.01,0.01));
			ps->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_FIREBALL));		



			em = ps->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor							
				irr::core::vector3df(0.0f,0.03f,0.0f),   // direccion inicial			
				80,120,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				200,500,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(2.f,2.f),         // tam minimo				
				irr::core::dimension2df(5.f,5.f));        // tam maximo

			//em->setMaxParticlesPerSecond(1);

			ps->setEmitter(em); // asociamos el emisor al sistema de particulas
			em->drop(); 

			//para que las particulas no desaparezcan de golpe creamos un affector con un fadeout:
			paf = ps->createFadeOutParticleAffector(); 

			ps->addAffector(paf); 
			paf->drop();
			break;

		case FXEngine::ParticleEffect::FIRE_LOW:


			ps->setScale(irr::core::vector3df(0.01,0.01,0.01));


			ps->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_FIREBALL));		



			em = ps->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor							
				irr::core::vector3df(0.0f,0.03f,0.0f),   // direccion inicial			
				80,100,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				200,400,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(2.f,2.f),         // tam minimo				
				irr::core::dimension2df(3.f,3.f));        // tam maximo

			//em->setMaxParticlesPerSecond(1);

			ps->setEmitter(em); // asociamos el emisor al sistema de particulas
			em->drop(); 

			//para que las particulas no desaparezcan de golpe creamos un affector con un fadeout:
			paf = ps->createFadeOutParticleAffector(); 

			ps->addAffector(paf); 
			paf->drop();
			break;

		case FXEngine::ParticleEffect::SMOKE:


			ps->setScale(irr::core::vector3df(0.01,0.01,0.01));


			ps->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_SMOKE));


			em = ps->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor			
				irr::core::vector3df(0.0f,0.03f,0.0f),   // direccion inicial
				80,120,                             // frecuencia de emision (min y max)
				irr::video::SColor(0,255,255,255),       // color mas oscuro
				irr::video::SColor(0,255,255,255),       // color mas brillante
				200,500,0,                         // duracion minima y maxima, angulo
				irr::core::dimension2df(2.f,2.f),         // tam minimo		
				irr::core::dimension2df(5.f,5.f));        // tam maximo		

			ps->setEmitter(em); // asociamos el emisor al sistema de particulas
			em->drop(); 

			//para que las particulas no desaparezcan de golpe creamos un affector con un fadeout:
			paf = ps->createFadeOutParticleAffector(); 

			ps->addAffector(paf); 
			paf->drop();
			break;

		case FXEngine::ParticleEffect::SMOKE_LOW:


			ps->setScale(irr::core::vector3df(0.01,0.01,0.01));


			ps->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_SMOKE));


			em = ps->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor			
				irr::core::vector3df(0.0f,0.03f,0.0f),   // direccion inicial
				80,120,                             // frecuencia de emision (min y max)
				irr::video::SColor(0,255,255,255),       // color mas oscuro
				irr::video::SColor(0,255,255,255),       // color mas brillante
				200,500,0,                         // duracion minima y maxima, angulo
				irr::core::dimension2df(1.f,1.f),         // tam minimo		
				irr::core::dimension2df(2.f,2.f));        // tam maximo		

			ps->setEmitter(em); // asociamos el emisor al sistema de particulas
			em->drop(); 

			//para que las particulas no desaparezcan de golpe creamos un affector con un fadeout:
			paf = ps->createFadeOutParticleAffector(); 

			ps->addAffector(paf); 
			paf->drop();
			break;
		case FXEngine::ParticleEffect::SPARK:



			ps->setScale(irr::core::vector3df(0.01,0.01,0.01));


			ps->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_SPARK));



			em = ps->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor			
				irr::core::vector3df(0.0f,0.03f,0.0f),   // direccion inicial
				80,120,                             // frecuencia de emision (min y max)
				irr::video::SColor(0,255,255,255),       // color mas oscuro
				irr::video::SColor(0,255,255,255),       // color mas brillante
				200,500,600,                         // duracion minima y maxima, angulo
				irr::core::dimension2df(2.f,2.f),         // tam minimo		
				irr::core::dimension2df(10.f,10.f));        // tam maximo		

			ps->setEmitter(em); // asociamos el emisor al sistema de particulas
			em->drop(); 

			//para que las particulas no desaparezcan de golpe creamos un affector con un fadeout:
			paf = ps->createFadeOutParticleAffector(); 

			ps->addAffector(paf); 
			paf->drop();
			break;

			break;

		case FXEngine::ParticleEffect::FIRE_AND_SMOKE_LOW:


			ps->setScale(irr::core::vector3df(0.01,0.01,0.01));


			ps->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_FIREBALL));					



			ps2 = sceneManager->addParticleSystemSceneNode(false,irrlichtNode);

			ps2->setScale(irr::core::vector3df(0.01,0.01,0.01));
			ps2->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			ps2->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
			ps2->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_SMOKE));				
			ps2->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR); 

			//FUEGO
			em = ps->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor							
				//irr::core::vector3df(0.0f,0.03f,0.0f),   // direccion inicial			
				irr::core::vector3df(0.0f,0.0f,0.0f),   // direccion inicial			
				200,350,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				100,150,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(5.f,5.f),         // tam minimo				
				irr::core::dimension2df(5.f,5.f));        // tam maximo

			//HUMO
			em2 = ps2->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor							
				irr::core::vector3df(0.0f,0.01f,0.0f),   // direccion inicial			
				100,150,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				400,600,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(1.f,1.f),         // tam minimo				
				irr::core::dimension2df(3.f,3.f));        // tam maximo

			ps->setEmitter(em); // asociamos el emisor al sistema de particulas
			em->drop(); 

			ps2->setEmitter(em2);
			em2->drop();

			//para que las particulas no desaparezcan de golpe creamos un affector con un fadeout:
			paf = ps->createFadeOutParticleAffector(); 

			ps->addAffector(paf); 
			ps2->addAffector(paf);
			paf->drop();
			break;

		case FXEngine::ParticleEffect::FIRE_AND_SMOKE_MEDIUM:


			ps->setScale(irr::core::vector3df(0.01,0.01,0.01));


			ps->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_FIREBALL));					



			ps2 = sceneManager->addParticleSystemSceneNode(false,irrlichtNode);

			ps2->setScale(irr::core::vector3df(0.01,0.01,0.01));
			ps2->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			ps2->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
			ps2->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_SMOKE));					
			ps2->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR); 

			//FUEGO
			em = ps->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor							
				//irr::core::vector3df(0.0f,0.03f,0.0f),   // direccion inicial			
				irr::core::vector3df(0.0f,0.0f,0.0f),   // direccion inicial			
				250,320,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				100,150,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(5.f,5.f),         // tam minimo				
				irr::core::dimension2df(8.f,8.f));        // tam maximo

			//HUMO
			em2 = ps2->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor							
				irr::core::vector3df(0.0f,0.01f,0.0f),   // direccion inicial			
				100,150,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				400,600,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(1.f,1.f),         // tam minimo				
				irr::core::dimension2df(3.f,3.f));        // tam maximo


			ps->setEmitter(em); // asociamos el emisor al sistema de particulas
			em->drop(); 

			ps2->setEmitter(em2);
			em2->drop();

			//para que las particulas no desaparezcan de golpe creamos un affector con un fadeout:
			paf = ps->createFadeOutParticleAffector(); 

			ps->addAffector(paf); 
			ps2->addAffector(paf);
			paf->drop();
			break;

		case FXEngine::ParticleEffect::FIRE_AND_SMOKE_HEAVY:


			ps->setScale(irr::core::vector3df(0.01,0.01,0.01));


			ps->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_FIREBALL));					


			ps2 = sceneManager->addParticleSystemSceneNode(false,irrlichtNode);

			ps2->setScale(irr::core::vector3df(0.01,0.01,0.01));
			ps2->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			ps2->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
			ps2->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_SMOKE));					
			ps2->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR); 

			//FUEGO
			em = ps->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor							
				//irr::core::vector3df(0.0f,0.03f,0.0f),   // direccion inicial			
				irr::core::vector3df(0.0f,0.0f,0.0f),   // direccion inicial			
				300,350,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				100,150,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(7.f,7.f),         // tam minimo				
				irr::core::dimension2df(10.f,10.f));        // tam maximo

			//HUMO
			em2 = ps2->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor							
				irr::core::vector3df(0.0f,0.01f,0.0f),   // direccion inicial			
				150,200,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(255,255,255,255),       // color mas oscuro			
				400,600,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(2.f,2.f),         // tam minimo				
				irr::core::dimension2df(5.f,5.f));        // tam maximo

			ps->setEmitter(em); // asociamos el emisor al sistema de particulas
			em->drop(); 

			ps2->setEmitter(em2);
			em2->drop();

			//para que las particulas no desaparezcan de golpe creamos un affector con un fadeout:
			paf = ps->createFadeOutParticleAffector(); 


			ps->addAffector(paf); 
			ps2->addAffector(paf);
			paf->drop();

			break;

		case FXEngine::ParticleEffect::FIGHTER_ENGINE:
			pX = -3;
			pY = -1;
			pZ = 0;



			ps->setScale(irr::core::vector3df(0.01,0.01,0.01));


			ps->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_FIREBALL));					



			ps2 = sceneManager->addParticleSystemSceneNode(false,irrlichtNode);
			ps2->setPosition(irr::core::vector3df(pX,pY,pZ)); 
			ps->setPosition(irr::core::vector3df(pX,pY,pZ)); 

			ps2->setScale(irr::core::vector3df(0.01,0.01,0.01));
			ps2->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			ps2->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
			ps2->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_SMOKE));				
			ps2->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR); 

			//FUEGO
			em = ps->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor							
				//irr::core::vector3df(0.0f,0.03f,0.0f),   // direccion inicial			
				irr::core::vector3df(0.0f,0.0f,0.0f),   // direccion inicial			
				200,350,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				100,150,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(5.f,5.f),         // tam minimo				
				irr::core::dimension2df(5.f,5.f));        // tam maximo

			//HUMO
			em2 = ps2->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor							
				//irr::core::vector3df(0.0f,0.01f,0.0f),   // direccion inicial			
				irr::core::vector3df(0.0f,0.0f,0.0f),   // direccion inicial			
				100,150,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				400,600,0,                         // duracion minima, maxima y angulo			
				//4000000000,6000000000,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(1.f,1.f),         // tam minimo				
				irr::core::dimension2df(3.f,3.f));        // tam maximo

			ps->setEmitter(em); // asociamos el emisor al sistema de particulas
			em->drop(); 

			ps2->setEmitter(em2);
			em2->drop();

			//para que las particulas no desaparezcan de golpe creamos un affector con un fadeout:
			paf = ps->createFadeOutParticleAffector(); 
			ps->addAffector(paf); 
			ps2->addAffector(paf);
			paf->drop();
			break;

		case FXEngine::ParticleEffect::ENGINER_LEFT_ENGINE:
			pX = -3;
			pY = 0;
			pZ = 1;

			ps->setScale(irr::core::vector3df(0.01,0.01,0.01));
			ps->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_FIREBALL));					

			ps2 = sceneManager->addParticleSystemSceneNode(false,irrlichtNode);
			ps2->setPosition(irr::core::vector3df(pX,pY,pZ)); 
			ps->setPosition(irr::core::vector3df(pX,pY,pZ)); 

			ps2->setScale(irr::core::vector3df(0.01,0.01,0.01));
			ps2->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			ps2->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
			ps2->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_SMOKE));				
			ps2->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR); 

			//FUEGO
			em = ps->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor							
				//irr::core::vector3df(0.0f,0.03f,0.0f),   // direccion inicial			
				irr::core::vector3df(0.0f,0.0f,0.0f),   // direccion inicial			
				200,350,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				100,150,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(2.f,2.f),         // tam minimo				
				irr::core::dimension2df(3.f,3.f));        // tam maximo

			//HUMO
			em2 = ps2->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor							
				//irr::core::vector3df(0.0f,0.01f,0.0f),   // direccion inicial			
				irr::core::vector3df(0.0f,0.0f,0.0f),   // direccion inicial			
				100,150,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				400,600,0,                         // duracion minima, maxima y angulo			
				//4000000000,6000000000,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(1.f,1.f),         // tam minimo				
				irr::core::dimension2df(2.f,2.f));        // tam maximo

			ps->setEmitter(em); // asociamos el emisor al sistema de particulas
			em->drop(); 

			ps2->setEmitter(em2);
			em2->drop();

			//para que las particulas no desaparezcan de golpe creamos un affector con un fadeout:
			paf = ps->createFadeOutParticleAffector(); 

			ps->addAffector(paf); 
			ps2->addAffector(paf);
			paf->drop();
			break;

		case FXEngine::ParticleEffect::ENGINER_RIGHT_ENGINE:
			pX = -3;
			pY = 0;
			//pZ = -1;
			pZ = -1.5;



			ps->setScale(irr::core::vector3df(0.01,0.01,0.01));


			ps->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_FIREBALL));					



			ps2 = sceneManager->addParticleSystemSceneNode(false,irrlichtNode);
			ps2->setPosition(irr::core::vector3df(pX,pY,pZ)); 
			ps->setPosition(irr::core::vector3df(pX,pY,pZ)); 

			ps2->setScale(irr::core::vector3df(0.01,0.01,0.01));
			ps2->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			ps2->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
			ps2->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_SMOKE));				
			ps2->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR); 

			//FUEGO
			em = ps->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor							
				//irr::core::vector3df(0.0f,0.03f,0.0f),   // direccion inicial			
				irr::core::vector3df(0.0f,0.0f,0.0f),   // direccion inicial			
				200,350,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				100,150,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(2.f,2.f),         // tam minimo				
				irr::core::dimension2df(3.f,3.f));        // tam maximo

			//HUMO
			em2 = ps2->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor							
				//irr::core::vector3df(0.0f,0.01f,0.0f),   // direccion inicial			
				irr::core::vector3df(0.0f,0.0f,0.0f),   // direccion inicial			
				100,150,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				400,600,0,                         // duracion minima, maxima y angulo			
				//4000000000,6000000000,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(1.f,1.f),         // tam minimo				
				irr::core::dimension2df(2.f,2.f));        // tam maximo

			ps->setEmitter(em); // asociamos el emisor al sistema de particulas
			em->drop(); 

			ps2->setEmitter(em2);
			em2->drop();

			//para que las particulas no desaparezcan de golpe creamos un affector con un fadeout:
			paf = ps->createFadeOutParticleAffector(); 

			ps->addAffector(paf); 
			ps2->addAffector(paf);
			paf->drop();
			break;

		case FXEngine::ParticleEffect::BOMBER_ENGINE:
			pX = -6;//pX = -19.5;
			pY = 1;//pY = 2.8;
			pZ = 0;



			ps->setScale(irr::core::vector3df(0.01,0.01,0.01));


			ps->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_FIREBALL_BLUE));					


			ps2 = sceneManager->addParticleSystemSceneNode(false,irrlichtNode);
			ps2->setPosition(irr::core::vector3df(pX,pY,pZ)); 
			ps->setPosition(irr::core::vector3df(pX,pY,pZ)); 

			ps2->setScale(irr::core::vector3df(0.01,0.01,0.01));
			ps2->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			ps2->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
			ps2->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_SMOKE));				
			ps2->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR); 

			//FUEGO
			em = ps->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor									
				irr::core::vector3df(0.0f,0.0f,0.0f),   // direccion inicial			
				200,350,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				100,150,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(3,3),         // tam minimo								
				irr::core::dimension2df(5,5));        // tam maximo

			//HUMO
			em2 = ps2->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor									
				irr::core::vector3df(0.0f,0.0f,0.0f),   // direccion inicial			
				80,120,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				350,500,0,                         // duracion minima, maxima y angulo							
				irr::core::dimension2df(1.f,1.f),         // tam minimo				
				irr::core::dimension2df(3.f,3.f));        // tam maximo

			ps->setEmitter(em); // asociamos el emisor al sistema de particulas
			em->drop(); 

			ps2->setEmitter(em2);
			em2->drop();

			//para que las particulas no desaparezcan de golpe creamos un affector con un fadeout:
			paf = ps->createFadeOutParticleAffector(); 

			ps->addAffector(paf); 
			ps2->addAffector(paf);
			paf->drop();
			break;

		case FXEngine::ParticleEffect::BATTLESHIP_ENGINE:			
			pX = -6;//pX = -17;
			pY = -1;
			pZ = 0;



			ps->setScale(irr::core::vector3df(0.01,0.01,0.01));			


			ps->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_FIREBALL_BLUE));					



			ps2 = sceneManager->addParticleSystemSceneNode(false,irrlichtNode);
			ps2->setPosition(irr::core::vector3df(pX,pY,pZ)); 
			ps->setPosition(irr::core::vector3df(pX,pY,pZ)); 

			ps2->setScale(irr::core::vector3df(0.01,0.01,0.01));			
			ps2->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			ps2->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
			ps2->setMaterialTexture(0, driver->getTexture(Resources::SPRITE_SMOKE));				
			ps2->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR); 

			//FUEGO
			em = ps->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor																				
				irr::core::vector3df(0.0f,0.0f,0.0f),   // direccion inicial			
				200,350,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				100,150,0,                         // duracion minima, maxima y angulo								
				irr::core::dimension2df(3,3),         // tam minimo								
				irr::core::dimension2df(5,5));        // tam maximo

			//HUMO
			em2 = ps2->createBoxEmitter(
				irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // tamanyo del emisor									
				irr::core::vector3df(0.0f,0.0f,0.0f),   // direccion inicial			
				80,120,                             // frecuencia min, max de emision
				irr::video::SColor(0,255,255,255),       // color mas brillante
				irr::video::SColor(0,255,255,255),       // color mas oscuro			
				350,500,0,                         // duracion minima, maxima y angulo			
				irr::core::dimension2df(1.f,1.f),         // tam minimo				
				irr::core::dimension2df(3.f,3.f));        // tam maximo

			ps->setEmitter(em); // asociamos el emisor al sistema de particulas
			em->drop(); 

			ps2->setEmitter(em2);
			em2->drop();

			//para que las particulas no desaparezcan de golpe creamos un affector con un fadeout:
			paf = ps->createFadeOutParticleAffector(); 

			ps->addAffector(paf); 
			ps2->addAffector(paf);
			paf->drop();
			break;

		default:
			std::cout<<"\n### No se ha aplicado ninguna particula al nodo ###";
			pX = -2;
			pY = 0;
			pZ = 0;
			break;
		}
	}
}

void FXEngineIrrlicht::damageEffect( bool keep )
{
	GraphicsEngineIrrlicht* graphicsEngineIrrlicht = (GraphicsEngineIrrlicht*)GameManager::getInstance()->getGraphicsEngine();
	graphicsEngineIrrlicht->damageEffectScreenQuad(keep);
}
