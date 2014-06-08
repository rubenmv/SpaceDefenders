#pragma once

#include "FXEngineTAG.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "ShaderProgramTAG.h"
#include "GameSettings.h"

#include "GameObjectManager.h" // Resources

// Para el efecto de damage
const float FXEngineTAG::FADEIN_STEP = 0.05f;
const float FXEngineTAG::FADEOUT_STEP = 0.001f;

FXEngineTAG::FXEngineTAG()
{
	activeEffect = PostEffect::NONE;

	ShaderProgramTAG* shader = NULL;
	GameSettings* gameSettings = GameManager::getInstance()->getGameSettings();
	ResourceManager* resourceManager = GameManager::getInstance()->getResourceManager();

	// INICIALIZACION DE SHADERS DE POSTPROCESADO
	// 1. VIGNETTE-SEPIA
	radius = 0.7f;
	intensity = 0.0f;
	// Shader de vignette cuando se recibe daño
	shader = resourceManager->getShader(Resources::SHADER_POST_PASS_VERT, Resources::SHADER_POST_VIGNETTE);

	shader->bind();
	// La resolucion de la pantalla y el radio del circulo para el efecto vignette
	shader->setUniformf("u_resolution", (float)gameSettings->getWidth(), (float)gameSettings->getHeight());
	shader->setUniformf("u_radius", radius);
	// Intensidad del color del centro
	shader->setUniformf("u_intensity", intensity);

	shader->unbind();

	shaders.push_back(shader);

	// 2. GLOW
	shader = resourceManager->getShader(Resources::SHADER_POST_PASS_VERT, Resources::SHADER_POST_GLOW);
	shaders.push_back(shader);

	// 3. MINIME
	shader = resourceManager->getShader(Resources::SHADER_POST_PASS_VERT, Resources::SHADER_POST_MINIME );
	shaders.push_back(shader);

	// 4. BW
	shader = resourceManager->getShader(Resources::SHADER_POST_PASS_VERT, Resources::SHADER_POST_BW);
	shaders.push_back(shader);

	// 5. BLUR
	shader = resourceManager->getShader(Resources::SHADER_POST_PASS_VERT, Resources::SHADER_POST_BLUR);
	shaders.push_back(shader);

	// 6. BILLBOARD
	shader = resourceManager->getShader(Resources::SHADER_POST_PASS_VERT, Resources::SHADER_POST_BILLBOARD);
	shaders.push_back(shader);

	shader = NULL;
}

FXEngineTAG::~FXEngineTAG()
{
	activeEffect = PostEffect::NONE;

	// Los shaders los elimina el gestor de recursos
	shaders.clear();
}
	
void FXEngineTAG::damageEffect( bool keep )
{
	keepActive = keep;

	activeEffect = PostEffect::DAMAGE;

	if ( keep )
	{
		radius = 0.72f;
		intensity = 0.6f; // Para el color, efecto sepia
	}
	else
	{
		radius = 0.75f;
		intensity = 0.0f;
	}
}

void FXEngineTAG::bindPostProcessing()
{
	switch (activeEffect)
	{
		case PostEffect::DAMAGE:
			// El efecto ha pasado
			if (radius > 1.2)
			{
				activeEffect = PostEffect::NONE;
				keepActive = false;
			}
			else
			{
				if ( !keepActive )
				{
					radius += FADEOUT_STEP;
				}
				
				shaders[PostEffect::DAMAGE]->bind();
				shaders[PostEffect::DAMAGE]->setUniformf("u_radius", radius);
				shaders[PostEffect::DAMAGE]->setUniformf("u_intensity", intensity);
			}
			break;
		case PostEffect::GLOW:
			shaders[PostEffect::GLOW]->bind();
			break;
		case PostEffect::MINIME:
			shaders[PostEffect::MINIME]->bind();
			break;
		case PostEffect::BW:
			shaders[PostEffect::BW]->bind();
			break;
		case PostEffect::BLUR:
			shaders[PostEffect::BLUR]->bind();
			break;
		case PostEffect::BILLBOARD:
			shaders[PostEffect::BILLBOARD]->bind();
			break;
	}
}

void FXEngineTAG::unbindPostProcessing()
{
	// Cualquiera sirve para hacer el unbind
	shaders[PostEffect::DAMAGE]->unbind();
}

void FXEngineTAG::reset()
{
	activeEffect = PostEffect::NONE;
}
