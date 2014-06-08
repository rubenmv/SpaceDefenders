#include "ComponentInputTarget.h"
#include "GameObject.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GraphicsEngine.h"

ComponentInputTarget::ComponentInputTarget()
{
	eventManager = GameManager::getInstance()->getEventManager();
	graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	mousePressed = false;
}

ComponentInputTarget::~ComponentInputTarget()
{
	eventManager = NULL;
	graphicsEngine = NULL;
	
}


void ComponentInputTarget::update()
{
	if(eventManager->mouseState.leftButtonPressed )
	{
		if(!mousePressed)
		{
			onTargetChange(graphicsEngine->getMousePositionOnGround());
			mousePressed = true;
		}
	} else 
	{
		mousePressed = false;
	}
}

void ComponentInputTarget::onMessage(const Message& message)
{

}

void ComponentInputTarget::onTargetChange(Vector2d target)
{
	Message message;

	message.type = Message::NEW_TARGET;
	message.target = target;

	// Se llama al objeto para que lance el mensaje
	parent->broadcastMessage(message);
}
