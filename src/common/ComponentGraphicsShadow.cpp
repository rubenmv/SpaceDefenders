#include "ComponentGraphicsShadow.h"
#include "NodeMesh.h"
#include "GraphicsEngine.h"
#include "GameManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"

ComponentGraphicsShadow::ComponentGraphicsShadow(const char* meshFilename, bool transparent, unsigned int color)
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

ComponentGraphicsShadow::~ComponentGraphicsShadow()
{
	delete node;
	node = 0;
}

void ComponentGraphicsShadow::update()
{
	node->setPosition(parent->position+Vector2d(3,-2));
	//Sin inclinacion
	node->setRotation(parent->rotation);	
}

void ComponentGraphicsShadow::onMessage(const Message& message)
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