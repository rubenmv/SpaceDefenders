#include "ComponentAIDefender.h"
#include "GameObject.h"
#include "GameManager.h"
#include "CollisionManager.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
ComponentAIDefender::ComponentAIDefender(GameObject* target,float attackRange)
{
	this->target = target;
	this->targetPlayer = NULL;
	this->attackRange = attackRange;
	this->shooting = false;
}


ComponentAIDefender::~ComponentAIDefender(void)
{
	target = NULL;
	targetPlayer = NULL;
}

void ComponentAIDefender::update()
{
	GameObject::removeDeadOnesFromList(onVisionList);

	if(targetPlayer == NULL && shooting)
	{
		shooting = false;
	}
	if(target != NULL && !target->isDead() && !parent->isDead())
	{
		if(!onVisionList.empty())
		{
			GameObject::removeDeadOnesFromList(onVisionList);
			selectAim();
		}
		parent->rotation = target->rotation;
		/*if(!GameManager::getInstance()->isServer())
		{
			GameManager::getInstance()->getGraphicsEngine()->drawDebugLine(parent->position.asVector3d(),target->position.asVector3d());
		}*/
	}
	else
	{
		Message message;
		message.type = Message::DEATH;
		parent->broadcastMessage(message);
		parent->kill();
	}
}

void ComponentAIDefender::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::TARGET_REACHED:
		messageNewTarget(target);
		break;
	case Message::NEW_VISION:
		addObject(message.gameObject);
		break;
	case Message::LOST_VISION:
		deleteObject(message.gameObject);
		break;
	case Message::DEATH:
		target->kill();
		target = NULL;
		break;
	}
}

void ComponentAIDefender::messageNewTarget(GameObject* gameObject)
{
	Message messageNewTarget;
	messageNewTarget.type = Message::NEW_TARGET;
	messageNewTarget.gameObject = target;
	parent->broadcastMessage(messageNewTarget);
}

void ComponentAIDefender::deleteObject(GameObject* targetObject)
{
	std::list<GameObject*>::iterator it;
	for (it=onVisionList.begin(); it!=onVisionList.end(); ++it)
	{	
		if( *it == targetObject)
		{
			//Es el primero
			if(*it == onVisionList.front() && shooting)
			{
				messageStopAIM();
				shooting = false;
			}
			onVisionList.erase(it);
			targetPlayer = NULL;
			selectTarget();
			
			break;
		}
	}
}



void ComponentAIDefender::addObject(GameObject* gameObject)
{
	if(gameObject->isPlayer() || gameObject->getTag() == GameObject::TURRET)
	{
		onVisionList.push_back(gameObject);
		if(onVisionList.size() > 1)
		{
			onVisionList.sort(GameObject::selectPrefEnemy);
		}

		if(onVisionList.front() == gameObject)
		{
			selectTarget();
		}
	}
}

// Selecciona un nuevo objetivo
void ComponentAIDefender::selectAim()
{
	if(targetPlayer != NULL)
	{
		if(attackRange > parent->position.getDistanceFrom(targetPlayer->position)) //Comprobaremos si esta dentro del rango
		{

			if(!shooting)
			{					
				messageNewAim(targetPlayer);
			}
			/* Version con can see target pero el can see target no esta implementado
			
			GameManager *gameManager = GameManager::getInstance();
			MapManager* mapManager = gameManager->getMapManager();
			GraphicsEngine *graphicsEngine = gameManager->getGraphicsEngine();


			//if(mapManager->canSeeTarget(parent->position,target->position,5)) && GameManager::getInstance()->getCollisionManager()->canSeeTarget(parent->position,target->position, parent->getId(), target->getId()))
			if(GameManager::getInstance()->getCollisionManager()->canSeeTarget(parent->position,targetPlayer->position, parent->getId(), targetPlayer->getId()))
			{			
				if(!shooting)
				{					
					messageNewAim(targetPlayer);
				}

			}
			else
			{		
				if(shooting)
				{
					messageStopAIM();
					shooting = false;
				}
			}*/

		}
		else
		{	
			if(shooting)
			{
				messageStopAIM();
				shooting = false;
			}
		}
	}
	else
	{
		selectTarget();
	}
}
	


void ComponentAIDefender::selectTarget()
{
	GameObject::removeDeadOnesFromList(onVisionList);

	if(!onVisionList.empty())
	{
		targetPlayer=NULL;

		targetPlayer = onVisionList.front();
		selectAim();
	}
	else
	{
		shooting = false;
		messageStopAIM();
	}
}

//Esta funcion  envia el gameObject al que se va atacar y la posicion actual de la nave, asi se para.
//se puede mandar un gameObject null y la nave se detiene y no ataca a nadie.

void ComponentAIDefender::messageNewAim(GameObject* gameObject)
{
	Message messageToSend;
	messageToSend.type = Message::NEW_AIM;
	messageToSend.gameObject = gameObject;
	parent->broadcastMessage(messageToSend);
	shooting = true;
}


void ComponentAIDefender::messageStopAIM()
{
	Message message;
	message.type = Message::STOP_AIM;
	parent->broadcastMessage(message);
}