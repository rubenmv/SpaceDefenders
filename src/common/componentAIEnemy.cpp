#include "ComponentAIEnemy.h"
#include "Vector2d.h"
#include "GameObject.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "CollisionManager.h"
#include "MapManager.h"
#include "AIEngine.h"
#include <algorithm>
#include <cctype>
#include "GameObjectManager.h"
ComponentAIEnemy::ComponentAIEnemy(float attackRange)
{

	watching = false;
	shooting = false;
	state = EnemyState::MOVE;
	this->attackRange = attackRange;
	target=NULL;
	selectedTarget = false;
	path = GameManager::getInstance()->getAIEngine()->getFirstPath();

	
}

void ComponentAIEnemy::awake()
{
	//Comprueba si esta lejos de la posicion inicial y si esta lejos le asigna un camino cercano.
	if(parent->position.getDistanceFrom(Vector2d(832,-59)) > 10)
	{
		path = GameManager::getInstance()->getAIEngine()->newPath(parent);
	}

}

ComponentAIEnemy::~ComponentAIEnemy(void)
{
	target = NULL;
	if(path->getNumberOfEnemies() > 0)
	{
		path->deleteEnemy();
	}
}

void ComponentAIEnemy::update()
{
	GameObject::removeDeadOnesFromList(onVisionList);
	if( state == EnemyState::ATTACK && distanceFromThePath() > 60 )
	{
		
		watching = false;
		shooting = false;		
		target=NULL;
		selectedTarget = false;
		messageStopAIM();
		state = EnemyState::RETURN;
	}

	
	//Comprueba que pases cerca del punto final del camino y si cambia el camino
	if(parent->position.getDistanceFrom(path->getEnd()) < 30 && state != EnemyState::ATTACK)
	{
		messageStopAIM();
		changeTarget(true);
	}


	switch (state)
	{
	case ATTACK:
		updateAttack();
		break;
	case MOVE:
		updateMove();
		break;
	case RETURN:
		updateReturn();
		break;
	default:
		break;
	}
}

void ComponentAIEnemy::updateAttack()
{

	if(onVisionList.empty())
	{
		if(!watching)
		{
			watching = false;
			shooting = false;
			state = EnemyState::MOVE;
			selectedTarget = false;
			//detienes la nave

			Message message;
			message.type = Message::NEW_AIM;
			message.gameObject = NULL;
			parent->broadcastMessage(message);
			//en vez de pararse como antes, se ira a la posicion que deba
			//messageNewTargetPosition(parent->position);
			messageNewTargetPosition(path->getEnd());
		}	
	}
	else
	{
		GameObject::removeDeadOnesFromList(onVisionList);

		selectAim();
	}

}


void ComponentAIEnemy::updateMove()
{
	
	if(!selectedTarget)
	{
		selectedTarget = true;
		messageNewTargetPosition(path->getEnd());
	}
}

void ComponentAIEnemy::updateReturn() 
{
	messageNewTargetPosition(path->getEnd());

	if(distanceFromThePath() < 15)
	{
		
		state = EnemyState::MOVE;
		selectAim();
	}
}

void ComponentAIEnemy::deleteObject(GameObject* targetObject)
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
			target = NULL;
			watching = false;
			selectTarget();
			
			break;
		}
	}
}



void ComponentAIEnemy::addObject(GameObject* gameObject)
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

// Selecciona un nuevo objetivo
void ComponentAIEnemy::selectAim()
{
	if(target != NULL)
	{
		if(attackRange > parent->position.getDistanceFrom(target->position)) //Comprobaremos si esta dentro del rango
		{

			if(!shooting)
			{					
				messageNewAim(target);
			}
			/* Version con can see target pero el can see target no esta implementado
			GameManager *gameManager = GameManager::getInstance();

			//if(mapManager->canSeeTarget(parent->position,target->position,5)) && GameManager::getInstance()->getCollisionManager()->canSeeTarget(parent->position,target->position, parent->getId(), target->getId()))
			if(GameManager::getInstance()->getCollisionManager()->canSeeTarget(parent->position,target->position, parent->getId(), target->getId()))
			{			
				if(!shooting)
				{					
					messageNewAim(target);
				}

			}
			else
			{		
				if(shooting)
				{
					messageStopAIM();

					messageNewTarget(target);

					shooting = false;
				}
			}*/

		}
		else
		{	
			if(shooting)
			{

				messageStopAIM();
				messageNewTarget(target);

				shooting = false;
			}
		}
	}
	else
	{

		selectTarget();
	}
}
	


void ComponentAIEnemy::selectTarget()
{
	GameObject::removeDeadOnesFromList(onVisionList);

	if(!onVisionList.empty())
	{
		target=NULL;

		target = onVisionList.front();
		watching = true;	
		messageNewTarget(target);
		selectAim();
		state = EnemyState::ATTACK;
	}
	else
	{

		watching = false;
		shooting = false;
		state = EnemyState::MOVE;
		selectedTarget = false;
		//detienes la nave
		messageStopAIM();

	}
}

void ComponentAIEnemy::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::NEW_VISION:
		addObject(message.gameObject);
		break;
	case Message::LOST_VISION:
		deleteObject(message.gameObject);
		break;
	case Message::TARGET_REACHED:
		changeTarget(false);
		break;
	}
}

void ComponentAIEnemy::messageNewTarget(GameObject* gameObject)
{

	Message messageToSend;
	messageToSend.type = Message::NEW_TARGET;
	messageToSend.gameObject = gameObject;
	parent->broadcastMessage(messageToSend);
	shooting = false;
}

//Esta funcion  envia el gameObject al que se va atacar y la posicion actual de la nave, asi se para.
//se puede mandar un gameObject null y la nave se detiene y no ataca a nadie.

void ComponentAIEnemy::messageNewAim(GameObject* gameObject)
{

	Message messageToSend;
	messageToSend.type = Message::NEW_AIM;
	messageToSend.gameObject = gameObject;
	parent->broadcastMessage(messageToSend);
	shooting = true;

	Message messageNewTarget;
	messageNewTarget.type = Message::NEW_TARGET;
	messageNewTarget.target = parent->position;
	parent->broadcastMessage(messageNewTarget);
}


void ComponentAIEnemy::messageNewTargetPosition(Vector2d target)
{

	Message messageNewTarget;
	messageNewTarget.type = Message::NEW_TARGET;
	messageNewTarget.target = target;
	parent->broadcastMessage(messageNewTarget);
	
}


void ComponentAIEnemy::messageStopAIM()
{
	Message message;
	message.type = Message::STOP_AIM;
	parent->broadcastMessage(message);
}

void ComponentAIEnemy::changeTarget( bool allStates )
{
	if(state == EnemyState::MOVE || allStates)
	{
		int pos = GameManager::getInstance()->getAIEngine()->getNextPath(parent->getName(),path->getPathList());
		if(pos != -1)			
		{
			if(path->getNumberOfEnemies() > 0)
			{
				path->deleteEnemy();
			}
			path = path->getPathList()[pos];
			path->addEnemy();
		} 
		messageNewTargetPosition(path->getEnd());
	}
}

float ComponentAIEnemy::distanceFromThePath()
{
	Vector2d start  = path->getStart();
	Vector2d end = path->getEnd();
	Vector2d normalPoint = Vector2d::getNormalPoint(parent->position, start, end);
	
	//COmprobamos que el punto esta entra el principio y el fin, si no es cogemos el final
	if (normalPoint.x < Math::min_(start.x,end.x) || normalPoint.x > Math::max_(start.x,end.x) ) 
	{
			if(start.getDistanceFrom(normalPoint) < end.getDistanceFrom(normalPoint))
			{
				normalPoint = start;
			}else
			{
				normalPoint = end;
			}
	}
	
	//Dibujamos linea
	GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	if(graphicsEngine != NULL)
	{
		if(GameManager::getInstance()->getDebugTools()->isShowingDebug())
		{				
			//	graphicsEngine->drawDebugLine(start.asVector3d(), end.asVector3d());
			graphicsEngine->drawDebugLine(normalPoint.asVector3d(), parent->position.asVector3d());
		}
	}

	float distance = parent->position.getDistanceFrom(normalPoint);

	return distance;

}

