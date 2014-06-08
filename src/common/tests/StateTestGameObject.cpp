#include <iostream>
#include <vector>
#include "StateTestGameObject.h"
#include "../GraphicsEngine.h"
#include "../GameManager.h"
#include "../GameObject.h"
#include "../GameObjectManager.h"
#include "../Component.h"
#include "../ComponentGraphics.h"
#include "../ComponentCamera.h"
#include "../DebugTools.h"

// Singleton
StateTestGameObject* StateTestGameObject::pInstance = NULL;

StateTestGameObject* StateTestGameObject::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateTestGameObject();

	return pInstance;
}

StateTestGameObject::StateTestGameObject()
{
	name = State::StateName::TEST_GAMEOBJECT;
}

StateTestGameObject::~StateTestGameObject()
{
}

void StateTestGameObject::init()
{
	gameManager = GameManager::getInstance();

	gameObjectManager = gameManager->getGameObjectManager();
	collisionManager = gameManager->getCollisionManager();
	gameManager->getGraphicsEngine()->initWorld();
	gameObjectManager->createPlayer();

}

void StateTestGameObject::cleanup()
{
}

void StateTestGameObject::update(GameManager* gameManager)
{
	gameObjectManager->update();
	collisionManager->update();
	gameManager->getDebugTools()->update();
}
