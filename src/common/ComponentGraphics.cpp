#include "ComponentGraphics.h"
#include "NodeMesh.h"
#include "GraphicsEngine.h"
#include "GameManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"

ComponentGraphics::ComponentGraphics(const char* meshFilename, bool transparent, unsigned int color)
{	
	GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	node = graphicsEngine->addNodeMesh(meshFilename, transparent);

	if(!node)
	{
		node = graphicsEngine->addNodeMesh(Resources::MESH_ERROR);
	}
	//node->setDebugColor(color);

	
	lastRotation = 0;
}

ComponentGraphics::~ComponentGraphics()
{
	delete node;
	node = 0;
}

void ComponentGraphics::update()
{
	node->setPosition(parent->position);

	if(parent->isPlayer())
	{
		//Con inclinacion
		float rate = 0.12;
		float inclination = (Math::warpAngle(lastRotation-parent->rotation))/GameManager::getInstance()->getDeltaTime()*rate;
		inclination = Math::constraint(inclination, -30, 30);
		node->setRotation(Vector3d(inclination, parent->rotation, 0));
		lastRotation = parent->rotation;

	} else
	{
		//Sin inclinacion
		node->setRotation(parent->rotation);
	}
	/*
	else if(parent->getTag() == GameObject::ENEMY)
	{
		float rate = 0.06;
		float inclination = (Math::warpAngle(lastRotation-parent->rotation))/GameManager::getInstance()->getDeltaTime()*rate;
		inclination = Math::constraint(inclination, -30, 30);
		node->setInclination(Vector3d(inclination, 0, 0));
	}
	*/
	
}

void ComponentGraphics::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::ON_CAMERA:
		node->setVisible(true);
		break;
	case Message::OUT_OF_CAMERA:
		node->setVisible(false);
		break;
	}
}