#include "ComponentInputObstacle.h"
#include "GameObject.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GraphicsEngine.h"
#include "DebugTools.h"
#include "AIEngine.h"

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
	if(eventManager->isKeyDown(KEY::KEY_F1))
	{
		if(!showKeyPressed)
		{
			showKeyPressed = true;
			if(!showDebug)
			{
				debugMessageText->setText(L"");
			}
		}
	} else 
	{
		showKeyPressed = false;
	}
}


void ComponentInputObstacle::onMessage(Message message)
{

}