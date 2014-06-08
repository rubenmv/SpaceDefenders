#include "ComponentCreateDebris.h"
#include "GameManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "PhysicsManager.h"

ComponentCreateDebris::ComponentCreateDebris(int nDebris)
{
	this->nDebris = nDebris;
	
}


ComponentCreateDebris::~ComponentCreateDebris(void)
{

}

void ComponentCreateDebris::update()
{

}

void ComponentCreateDebris::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::DEATH:
		if(GameManager::getInstance()->getPhysicsManager() != NULL)
		{
			createDebris();
		}
		break;
	}
}

void ComponentCreateDebris::createDebris()
{
	GameObjectManager* gameObjectManager = GameManager::getInstance()->getGameObjectManager();
	Vector2d velocity(1,1);
	int angle = 360/nDebris;
	for(int i=0;i<nDebris;i++)
	{
		Vector2d velocityAux = velocity.rotateBy(Math::random(20) + i*angle + angle,Vector2d(0,0)).normalize();
		switch (Math::random(0,5))
		{
		case 0:
			gameObjectManager->createGenericDebris(Resources::MESH_DEBRIS_1,velocityAux * 2,parent->position);
			break;
		case 1:
			gameObjectManager->createGenericDebris(Resources::MESH_DEBRIS_2,velocityAux * 2,parent->position);
			break;
		case 2:
			gameObjectManager->createGenericDebris(Resources::MESH_DEBRIS_3,velocityAux * 2,parent->position);
			break;
		case 3:
			gameObjectManager->createGenericDebris(Resources::MESH_DEBRIS_4,velocityAux * 2,parent->position);
			break;
		case 4:
			gameObjectManager->createGenericDebris(Resources::MESH_DEBRIS_5,velocityAux * 2,parent->position);
			break;
		}

	}
	gameObjectManager->createMuzzleFlash()->position =parent->position-velocity;
	gameObjectManager->createMuzzleFlash()->position =parent->position+velocity*2;
	GameManager::getInstance()->getPhysicsManager()->createExplosion(parent->position,nDebris/5);
}