#include "ComponentGraphicsStatics.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "NodeMesh.h"

ComponentGraphicsStatics::ComponentGraphicsStatics(const char* meshFilename, unsigned int color)
{
	GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	staticNode = graphicsEngine->addNodeMesh(meshFilename);
}


ComponentGraphicsStatics::~ComponentGraphicsStatics(void)
{
	delete staticNode;
	staticNode = 0;
}

void ComponentGraphicsStatics::update()
{
	if (parent->isDead())
	{
		staticNode->setColor(DebugColor::WHITE);
	}
	if(staticNode != NULL)
	{
		staticNode->setPosition(parent->position);
	}
}

void ComponentGraphicsStatics::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::ON_CAMERA:
		staticNode->setVisible(true);
		break;
	case Message::OUT_OF_CAMERA:
		staticNode->setVisible(false);
		break;
	}
}