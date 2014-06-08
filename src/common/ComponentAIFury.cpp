#include "ComponentAIFury.h"
#include "GameObject.h"
#include "Vector2d.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "CollisionManager.h"
#include "MapManager.h"
#include "AIEngine.h"
#include "GameObjectManager.h"
#include <algorithm>
#include <cctype>

ComponentAIFury::ComponentAIFury(GameObject* targetObject)
{
	reached = false;
	
	if(targetObject != NULL)
	{
		target = targetObject;
	}
	else
	{
		std::vector<GameObject*> gameObjects = GameManager::getInstance()->getGameObjectManager()->getGameObjectList();
		std::vector<GameObject*> players;
		for(size_t i=0;i<gameObjects.size();i++)
		{
			if(gameObjects[i]->getTag() == GameObject::MAIN_PLAYER)
			{
				players.push_back(gameObjects[i]);
			}
		}

		if(players.size() > 0)
		{
			int pos = Math::random(players.size());
			target = players[pos];			
		}
		else
		{
			target = NULL;
		}

	}
}

void ComponentAIFury::awake()
{
	firstPosition(target);
}
void ComponentAIFury::update()
{
	
	if(brother != NULL && brother->isDead())
	{
		brother = NULL;
	}
}

void ComponentAIFury::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::DEATH:
		cheatBrother();
		break;
	case Message::ASSIGN_GAME_OBJECT:
		this->brother = message.gameObject;
		break;
	case Message::NEW_VISION:
		if(reached)
		{
			if(message.gameObject->getTag() == GameObject::MAIN_PLAYER)
			{
				target = message.gameObject;
				messageNewAim(message.gameObject);
			}
		}
		break;
	case Message::LOST_VISION:
		if(reached)
		{
			if(message.gameObject == target)
			{
				messageStopAim();
			}
		}
		break;
	case Message::TARGET_REACHED:
		if(!reached)
		{
			messageStopAim();
			reached = true;
		}
		break;
	}
}

void ComponentAIFury::cheatBrother()
{
	Message messageToSend;
	messageToSend.type = Message::LEVEL_CHANGED;
	messageToSend.target = Vector2d(2,2);
	messageToSend.value = 2;
	if(brother != NULL && !brother->isDead())
	{
		brother->broadcastMessage(messageToSend);
	}
}

ComponentAIFury::~ComponentAIFury(void)
{
	target = NULL;

}

//Esta funcion  envia el gameObject al que se va atacar y la posicion actual de la nave, asi se para.
//se puede mandar un gameObject null y la nave se detiene y no ataca a nadie.

void ComponentAIFury::messageNewAim(GameObject* gameObject)
{
	Message messageToSend;
	messageToSend.type = Message::NEW_AIM;
	messageToSend.gameObject = gameObject;
	parent->broadcastMessage(messageToSend);
	
	Message messageNewTarget;
	messageNewTarget.type = Message::NEW_TARGET;
	if(gameObject != NULL)
	{
		messageNewTarget.target = parent->position;
	}
	else
	{
		messageNewTarget.gameObject = target;
	}
	parent->broadcastMessage(messageNewTarget);
}

void ComponentAIFury::messageStopAim()
{
	Message message;
	message.type = Message::STOP_AIM;
	parent->broadcastMessage(message);

	Message messageNewTarget;
	messageNewTarget.type = Message::NEW_TARGET;
	if(target==NULL || target->isDead())
	{
		std::vector<GameObject*> gameObjects = GameManager::getInstance()->getGameObjectManager()->getGameObjectList();
		std::vector<GameObject*> players;
		for(size_t i=0;i<gameObjects.size();i++)
		{
			if(gameObjects[i]->getTag() == GameObject::MAIN_PLAYER && !gameObjects[i]->isDead())
			{
				players.push_back(gameObjects[i]);
			}
		}

		if(players.size() > 0)
		{
			int pos = Math::random(players.size());
			target = players[pos];
		}
		else
		{
			target = NULL;
		}
	}
	messageNewTarget.gameObject = target;
	parent->broadcastMessage(messageNewTarget);
}

void ComponentAIFury::firstPosition( GameObject* target )
{
	targetPosition = Vector2d(613,-290);

	if(target != NULL)
	{
		std::vector<Vector2d> firstPositionList;
		//	firstPositionList.push_back(Vector2d(487,-88));
		firstPositionList.push_back(Vector2d(487,-88));
		firstPositionList.push_back(Vector2d(806, -460));

		for(size_t i = 0; i < firstPositionList.size();i++)
		{
			if(targetPosition.getDistanceFrom(target->position) > firstPositionList[i].getDistanceFrom(target->position)  )
			{

				targetPosition = firstPositionList[i];
			}

		}
	}


	Message messageNewTarget;
	messageNewTarget.type = Message::NEW_TARGET;
	messageNewTarget.target = targetPosition;
	parent->broadcastMessage(messageNewTarget);
}
