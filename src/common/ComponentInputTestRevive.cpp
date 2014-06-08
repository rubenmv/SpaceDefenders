#include "ComponentInput.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GameObject.h"
#include "GraphicsEngine.h"
#include "CollisionManager.h"

ComponentInput::ComponentInput(float speed)
{
	eventManager = GameManager::getInstance()->getEventManager();
}


ComponentInput::~ComponentInput(void)
{
	eventManager = NULL;
}

void ComponentInput::update()
{

}