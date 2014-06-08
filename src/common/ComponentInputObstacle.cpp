#include "ComponentInputObstacle.h"
#include "GameObject.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GraphicsEngine.h"
#include "DebugTools.h"
#include "MapManager.h"

ComponentInputObstacle::ComponentInputObstacle(void)
{
	eventManager = GameManager::getInstance()->getEventManager();
	graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	mousePressed = false;
}


ComponentInputObstacle::~ComponentInputObstacle(void)
{
	eventManager = NULL;
	graphicsEngine = NULL;
}

void ComponentInputObstacle::update()
{
	if(eventManager->mouseState.rightButtonPressed)
	{
		//Llamamos a la funcion createCubeObstacle
		GameManager::getInstance()->getDebugTools()->createCubeObstacle(graphicsEngine->getMousePositionOnGround());
	}
}


void ComponentInputObstacle::onMessage(const Message& message)
{

}