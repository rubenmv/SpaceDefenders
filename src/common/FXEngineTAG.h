#pragma once

#include "FXEngine.h"
#include <vector>

class GraphicsEngineTAG;
class ShaderProgramTAG;

class FXEngineTAG : public FXEngine
{
public:
	//const float OPACITY_STEP;

	FXEngineTAG();
	virtual ~FXEngineTAG();

	// <Metodos virtuales>
	void damageEffect( bool keep );
	// </Metodos virtuales>

	void bindPostProcessing();
	void unbindPostProcessing();
	void reset();
private:
	static const float FADEIN_STEP;
	static const float FADEOUT_STEP;

	// Shader activo
	int activeEffect;
	// Los shaders en este vector tienen que crearse ordenados segun
	// el enum de FXEngine.h (PostEffect)
	std::vector<ShaderProgramTAG*> shaders;
	// Para el efecto de damage
	float radius;
	float intensity;
	bool keepActive; // Para saber si el efecto es permanente
	bool finished; // Indica si el efecto ha terminado
};


