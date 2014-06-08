#include "ComponentVision.h"

#include "ComponentCollider.h"
#include <iostream>
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "GameObject.h"
#include "Message.h"

ComponentVision::ComponentVision(float visionRadius)
{
	this->sqrVisionRadius = visionRadius*visionRadius;

	GameManager::getInstance()->getCollisionManager()->addComponentVision(this);

}

ComponentVision::~ComponentVision(void)
{
	GameManager::getInstance()->getCollisionManager()->removeFromList(this);
}

// Envia un mensaje con el objeto que entra en el radio de vision
void ComponentVision::onVision(Collision collision)
{
	// Recoge el objeto
	bool found = false;
	int size = onVisionList.size();
	for(int i = 0 ; i < size ; i++)
	{
		if(onVisionList[i] == collision.collider)
		{
			found = true;
			break;
		}
	}

	
	// Guarda el objeto para no volver a enviar
	onVisionListTemp.push_back(collision.collider);
	if(!found)
	{
		Message message;
		message.type = Message::NEW_VISION;
		message.gameObject = collision.collider;
		parent->broadcastMessage(message);
	}

	// Debug
	/*
	GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	if(graphicsEngine != NULL)
	{
		if(GameManager::getInstance()->getDebugTools()->isShowingDebug())
		{				
			graphicsEngine->drawDebugLine(parent->position.asVector3d(), collision.collider->getGameObject()->position.asVector3d());
		}
	}
	*/
}

// Un objeto sale de la vision
void ComponentVision::noVision(GameObject* gameObject)
{
	Message message;
	message.type = Message::LOST_VISION;
	message.gameObject = gameObject;
	parent->broadcastMessage(message);
}

void ComponentVision::update()
{
	if(!onVisionListTemp.empty())
	{
		GameObject::removeDeadOnesFromVector(onVisionListTemp);
	}

	int sizeTemp = onVisionListTemp.size();
	int size = onVisionList.size();
	bool found=false;


	// Comprueba si ha salido algun objeto
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < sizeTemp; j++)
		{
			if(onVisionList[i] == onVisionListTemp[j])
			{
				found = true;
				j = sizeTemp;
			}
		}

		// El objeto ha salido del campo de vision
		if(!found)
		{
			noVision(onVisionList[i]); // Lo envia por mensaje
		}
		found = false;
	}

	onVisionList = onVisionListTemp;
	onVisionListTemp.clear();
}

void ComponentVision::onMessage(const Message& message)
{

}