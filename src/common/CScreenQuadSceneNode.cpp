#include "CScreenQuadSceneNode.h"

#include "irrlicht/irrlicht.h"
#include "Shader.h"
#include "FilterShaderCallBack.h"

// Para el efecto de damage
const float CScreenQuadSceneNode::FADEIN_STEP = 0.05f;
const float CScreenQuadSceneNode::FADEOUT_STEP = 0.005f;

CScreenQuadSceneNode::CScreenQuadSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* sceneManager, irr::s32 id) : ISceneNode(parent,sceneManager,id)
{
	irr::f32 shiftX,shiftY;
	irr::core::dimension2d<irr::u32> currentResolution;

	/**Here we initialize the vertices of the screen Aligned quad*/

	currentResolution = sceneManager->getVideoDriver()->getScreenSize();

	aabb.reset(0,0,0);

	// Originalmente 0.5 pero se veia borroso, no entiendo para que es esto en realidad
	shiftX = 1.0/currentResolution.Width;	//This small shift is necesary to compensate the texture sampling bias
	shiftY = 1.0/currentResolution.Height;	//It avoids that our effect becomes too blurry.

	vertices[0] = irr::video::S3DVertex2TCoords(
		-1.0f,-1.0f,0.0f,
		0.0f,0.0f,-1.0f,
		irr::video::SColor(255,255,255,255),
		shiftX,1+shiftY,
		shiftX,1+shiftY);

	vertices[1] = irr::video::S3DVertex2TCoords(
		1.0f,-1.0,0.0f,
		0.0f,0.0f,-1.0f,
		irr::video::SColor(255,255,255,255),
		1.0f+shiftX,1+shiftY,
		1.0f+shiftX,1+shiftY);

	vertices[2] = irr::video::S3DVertex2TCoords(
		-1.0f,1.0,0.0f,
		0.0f,0.0f,-1.0f,
		irr::video::SColor(255,255,255,255),
		shiftX,shiftY,
		shiftX,shiftY);

	vertices[3] = irr::video::S3DVertex2TCoords(
		1.0f,1.0f,0.0f,
		0.0f,0.0f,-1.0f,
		irr::video::SColor(255,255,255,255),
		1.0f+shiftX,shiftY,
		1.0f+shiftX,shiftY);



	material.Lighting = false;							//No need for lighting.
	//material.MaterialType = irr::video::EMT_LIGHTMAP_ADD;	//This will add both first and second textures :)
	material.BackfaceCulling=false;						//not needed, but simplifies things
	setAutomaticCulling(irr::scene::EAC_OFF);				//We don't need this scene
	//node to be culled because we render it in screen space.

	//Inicializamos shaders y variables de efectos
	radius = 0.0f;
	intensity = 0.0f;

	// create materials
	irr::s32 newMaterial = 0;

	shaderCallback = new FilterShaderCallBack();
	shaderCallback->setIntensity(intensity);
	shaderCallback->setRadius(radius);
	std::cout << "res: " << currentResolution.Width << std::endl;
	shaderCallback->setResolution(currentResolution);

	const irr::video::E_GPU_SHADING_LANGUAGE shadingLanguage = irr::video::EGSL_DEFAULT;

	Shader* shader = new Shader("../assets/shaders/pass_through/pass_through.vert", "../assets/shaders/postprocessing/vignette.frag");

	newMaterial = sceneManager->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(
		shader->getVertexFileName().c_str(),"main",irr::video::EVST_VS_1_1,
		shader->getFragmentFileName().c_str(), "main", irr::video::EPST_PS_1_1,
		shaderCallback,irr::video::EMT_SOLID,0,shadingLanguage);	


	shaders.push_back(newMaterial);

	// 2. GLOW

	shader = new Shader("../assets/shaders/pass_through/pass_through.vert", "../assets/shaders/postprocessing/glow.frag");

	newMaterial = sceneManager->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(
		shader->getVertexFileName().c_str(),"main",irr::video::EVST_VS_1_1,
		shader->getFragmentFileName().c_str(), "main", irr::video::EPST_PS_1_1,
		shaderCallback,irr::video::EMT_SOLID,0,shadingLanguage);	

	shaders.push_back(newMaterial);

	// 3. MINIME
	shader = new Shader("../assets/shaders/pass_through/pass_through.vert", "../assets/shaders/postprocessing/minime.frag");

	newMaterial = sceneManager->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(
		shader->getVertexFileName().c_str(),"main",irr::video::EVST_VS_1_1,
		shader->getFragmentFileName().c_str(), "main", irr::video::EPST_PS_1_1,
		shaderCallback,irr::video::EMT_SOLID,0,shadingLanguage);	

	shaders.push_back(newMaterial);

	// 4. BW
	shader = new Shader("../assets/shaders/pass_through/pass_through.vert", "../assets/shaders/postprocessing/bw.frag");

	newMaterial = sceneManager->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(
		shader->getVertexFileName().c_str(),"main",irr::video::EVST_VS_1_1,
		shader->getFragmentFileName().c_str(), "main", irr::video::EPST_PS_1_1,
		shaderCallback,irr::video::EMT_SOLID,0,shadingLanguage);	

	shaders.push_back(newMaterial);

	// 5. BLUR
	shader = new Shader("../assets/shaders/pass_through/pass_through.vert", "../assets/shaders/postprocessing/blur.frag");

	newMaterial = sceneManager->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(
		shader->getVertexFileName().c_str(),"main",irr::video::EVST_VS_1_1,
		shader->getFragmentFileName().c_str(), "main", irr::video::EPST_PS_1_1,
		shaderCallback,irr::video::EMT_SOLID,0,shadingLanguage);	

	shaders.push_back(newMaterial);

	// 6. BILLBOARD
	shader = new Shader("../assets/shaders/pass_through/pass_through.vert", "../assets/shaders/postprocessing/billboard.frag");

	newMaterial = sceneManager->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(
		shader->getVertexFileName().c_str(),"main",irr::video::EVST_VS_1_1,
		shader->getFragmentFileName().c_str(), "main", irr::video::EPST_PS_1_1,
		shaderCallback,irr::video::EMT_SOLID,0,shadingLanguage);	

	shaders.push_back(newMaterial);

	newMaterial = 0;
	shaders.push_back(newMaterial);


	shaderCallback->drop();

	delete shader;

}

CScreenQuadSceneNode::~CScreenQuadSceneNode()
{

	delete shaderCallback;
}

void CScreenQuadSceneNode::render()
{
	irr::video::IVideoDriver* drv = getSceneManager()->getVideoDriver();
	//	irr::core::matrix4 proj;
	irr::u16 indices[] = {0,1,2,3,1,2};
	//A triangle list

	bindPostProcessing();
	drv->setMaterial(material);

	drv->setTransform(irr::video::ETS_PROJECTION, irr::core::IdentityMatrix);
	drv->setTransform(irr::video::ETS_VIEW, irr::core::IdentityMatrix);

	drv->drawIndexedTriangleList(&vertices[0],4,&indices[0],2);

}

void CScreenQuadSceneNode::damageEffect( bool keep )
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

//Carga el material, pero el nombre lo he cogido por si lo pasamos a FXIrrlicht cuando funcione
void CScreenQuadSceneNode::bindPostProcessing()
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

			shaderCallback->setRadius(radius);
			shaderCallback->setIntensity(intensity);

			getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)shaders[PostEffect::DAMAGE];		
		}
		break;
	case PostEffect::GLOW:
		getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)shaders[PostEffect::GLOW];
		break;
	case PostEffect::MINIME:
		getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)shaders[PostEffect::MINIME];
		break;
	case PostEffect::BW:
		getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)shaders[PostEffect::BW];
		break;
	case PostEffect::BLUR:
		getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)shaders[PostEffect::BLUR];
		break;
	case PostEffect::BILLBOARD:
		getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)shaders[PostEffect::BILLBOARD];
		break;
	case PostEffect::NONE:
		getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)shaders[PostEffect::NONE];
		break;
	}
}

void CScreenQuadSceneNode::reset()
{
	activeEffect = PostEffect::NONE;
}
