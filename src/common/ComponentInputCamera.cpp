#include "GameObject.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GraphicsEngine.h"
#include "ComponentInputCamera.h"

ComponentInputCamera::ComponentInputCamera()
{
	eventManager = GameManager::getInstance()->getEventManager();
}

ComponentInputCamera::~ComponentInputCamera()
{
	eventManager = NULL;
}

void ComponentInputCamera::update()
{
	if(eventManager->isKeyPressed(KEY::KEY_KEY_A))
	{
		parent->position.x -= 1;
	}
	if(eventManager->isKeyPressed(KEY::KEY_KEY_D))
	{
		parent->position.x += 1;
	}
	if(eventManager->isKeyPressed(KEY::KEY_KEY_S))
	{
		parent->position.y -= 1;
	}

	if(eventManager->isKeyPressed(KEY::KEY_KEY_W))
	{
		parent->position.y += 1;
	}

}

void ComponentInputCamera::onMessage(const Message& message)
{

}
