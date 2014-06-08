#include "ComponentAIBomber.h"
#include "GameObject.h"
#include "GameManager.h"
#include "GameObjectManager.h"
#include "AIEngine.h"
#include "CollisionManager.h"
#include "GraphicsEngine.h"

ComponentAIBomber::ComponentAIBomber(float attackRange)
{
	for(int i=0;i<8;i++)
	{
		GameObject* defender = GameManager::getInstance()->getGameObjectManager()->voidGameObject();
		defenders.push_back(defender);
	}

	watching = false;
	shooting = false;
	state = EnemyState::MOVE;
	this->attackRange = attackRange;
	target=NULL;
	selectedTarget = false;
	path = GameManager::getInstance()->getAIEngine()->getFirstPath();
	charging = false;
}


ComponentAIBomber::~ComponentAIBomber(void)
{
	target = NULL;
}

void ComponentAIBomber::update()
{
	GameObject::removeDeadOnesFromList(onVisionList);
	GameObject::removeDeadOnesFromList(defenders);

	if(!parent->isDead())
	{

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
	else
	{
		std::list<GameObject*>::iterator it;
		for (it=defenders.begin(); it!=defenders.end(); ++it)
		{
			(*it)->kill();
		}
	}
}

void ComponentAIBomber::onMessage(const Message& message)
{
	std::list<GameObject*>::iterator it;
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
	case Message::DEATH:
		for (it=defenders.begin(); it!=defenders.end(); ++it)
		{
			(*it)->kill();
		}
		break;
	case Message::START_SHOOTING:
		charging = true;
		break;
	case Message::STOP_SHOOTING:
		charging = false;
		break;
	}
}

void ComponentAIBomber::awake()
{
	//Comprueba si esta lejos de la posicion inicial y si esta lejos le asigna un camino cercano.
	if(parent->position.getDistanceFrom(Vector2d(832,-59)) > 10)
	{
		path = GameManager::getInstance()->getAIEngine()->newPath(parent);
	}

	GameObjectManager* gameObjectManager = GameManager::getInstance()->getGameObjectManager();
	int i=0;
	std::list<GameObject*>::iterator it;
	for (it=defenders.begin(); it!=defenders.end(); ++it)
	{
		Vector2d tv(1,0);
 
		tv.rotateBy(i*(360/defenders.size()),Vector2d(0,0));
		tv.normalize();
		tv *= 15;

		(*it)->position = Vector2d(105,-340) + tv;
		GameObject* defender = gameObjectManager->createEnemyDefender((*it));
		//defender->position = Vector2d(105,-340) + tv;
		defender->position = parent->position + tv;
		Message message;
		message.type = Message::NEW_TARGET;
		message.gameObject = (*it);
		defender->broadcastMessage(message);
		i++;
	}
}

void ComponentAIBomber::defenderSquad()
{

	//GameManager::getInstance()->getGraphicsEngine()->drawDebugLine(parent->position.asVector3d(), (parent->position + (Vector2d::getVector2dByAngle(-parent->rotation) * -30)).asVector3d(),2);
	int i=0;
	std::list<GameObject*>::iterator it;
	int cantRow = defenders.size();
	int separationBack = 260/defenders.size();
	for (it=defenders.begin(); it!=defenders.end(); ++it)
	{
		if(!(*it)->isDead())
		{
			Vector2d tv = Vector2d::getVector2dByAngle(-parent->rotation) * -1;
			//tv = Vector2d::getVector2dByAngle(parent->rotation);
			int angle = 260/cantRow;
			tv.normalize();

			int total = separationBack * (defenders.size()-1);
			tv.rotateBy(i*separationBack - total/2,Vector2d(0,0));
			tv.normalize();
			tv *= 15;
			//angulo igual a nueva pos mirando hacia player
			(*it)->position = parent->position + tv;
			if(players.empty())
			{
				float desiredRotation = parent->rotation;

				float newRotation = (*it)->rotation - desiredRotation;

				newRotation = Math::warpAngle(newRotation);

				if(Math::abs(newRotation) < 0.001)
				{
					(*it)->rotation = desiredRotation;
				} 
				else
				{
					(*it)->rotation -= newRotation * 0.2;
				}
			}
			else
			{
				float desiredRotation = (players.front()->position - (*it)->position).getAngle();

				float newRotation = (*it)->rotation - desiredRotation;

				newRotation = Math::warpAngle(newRotation);

				if(Math::abs(newRotation) < 0.001)
				{
					(*it)->rotation = desiredRotation;
				} 
				else
				{
					(*it)->rotation -= newRotation * 0.2;
				}
			}
			/*if(!GameManager::getInstance()->isServer())
			{
				GameManager::getInstance()->getGraphicsEngine()->drawDebugLine(parent->position.asVector3d(),(*it)->position.asVector3d());
			}*/
		}
		i++;
	}
}

void ComponentAIBomber::attackSquad(GameObject* targetPlayer)
{
	int i=0;
	std::list<GameObject*>::iterator it;

	int lengthFrontRow = -1;
	int lengthBackRow = -1;
	if(defenders.size()/4 == 0 && defenders.size() > 0)
	{
		lengthBackRow = defenders.size()%4;
		if(lengthBackRow == 0)
		{
			lengthBackRow = 4;
		}
	}
	else
	{
		lengthBackRow = 4;
		lengthFrontRow = defenders.size()%4;
		if(lengthFrontRow == 0)
		{
			lengthFrontRow = 4;
		}
	}
	int separationBack = 20;
	int separationFront = 15;
	for (it=defenders.begin(); it!=defenders.end(); ++it)
	{
		if(!(*it)->isDead())
		{
			Vector2d tv = targetPlayer->position - parent->position;
			//tv = Vector2d::getVector2dByAngle(parent->rotation);
			tv.normalize();
			if(i < 4)
			{
				int total = separationBack * (lengthBackRow-1);
				tv.rotateBy(i%lengthBackRow*separationBack - total/2,Vector2d(0,0));
				tv.normalize();
				tv *= 15+10*i/lengthBackRow;
			}
			else
			{
				int total = separationFront * (lengthFrontRow-1);
				tv.rotateBy(i%lengthFrontRow*separationFront - total/2,Vector2d(0,0));
				tv.normalize();
				tv *= 15+10*i/lengthBackRow;
			}
			//angulo igual a nueva pos mirando hacia player
			(*it)->position = parent->position + tv;
			float desiredRotation = (players.front()->position - (*it)->position).getAngle();

			float newRotation = (*it)->rotation - desiredRotation;

			newRotation = Math::warpAngle(newRotation);

			if(Math::abs(newRotation) < 0.001)
			{
				(*it)->rotation = desiredRotation;
			} else
			{
				(*it)->rotation -= newRotation * 0.2;
			}
			/*if(!GameManager::getInstance()->isServer())
			{
				GameManager::getInstance()->getGraphicsEngine()->drawDebugLine(parent->position.asVector3d(),(*it)->position.asVector3d());
			}*/
		}
		i++;
	}
}

void ComponentAIBomber::updateAttack()
{
	GameObject::removeDeadOnesFromList(defenders);
	bool defend = true;
	if(!defenders.empty())
	{
		if(!players.empty())
		{
			std::list<GameObject*>::iterator it;
			for (it=players.begin(); it!=players.end() && defend; ++it)
			{
				if(!(*it)->position.getSqrDistanceFrom(parent->position) < 80*80)
				{
					attackSquad(*it);
					defend = false;
				}
			}
			if(defend)
			{
				defenderSquad();
			}
		}
		else
		{
			defenderSquad();
		}
	}
	
	if(onVisionList.empty())
	{
		if(!watching)
		{
			watching = false;
			shooting = false;
			state = EnemyState::MOVE;
			selectedTarget = false;
			//detienes la nave

			messageStopAIM();
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


void ComponentAIBomber::updateMove()
{
	int i=0;
	if(!defenders.empty())
	{
	std::list<GameObject*>::iterator it;
	for (it=defenders.begin(); it!=defenders.end(); ++it)
	{
		if(!(*it)->isDead())
		{
			Vector2d tv(1,0);
 
			tv.rotateBy(i*(360/defenders.size()),Vector2d(0,0));
			tv.normalize();
			tv *= 15;
			
			(*it)->position = parent->position + tv;
			
			if(players.empty())
			{
					float desiredRotation = parent->rotation;

					float newRotation = (*it)->rotation - desiredRotation;

					newRotation = Math::warpAngle(newRotation);

					if(Math::abs(newRotation) < 0.001)
					{
						(*it)->rotation = desiredRotation;
					} else
					{
						(*it)->rotation -= newRotation * 0.2;
					}
			}
			else
			{
					float desiredRotation = (players.front()->position - (*it)->position).getAngle();

					float newRotation = (*it)->rotation - desiredRotation;

					newRotation = Math::warpAngle(newRotation);

					if(Math::abs(newRotation) < 0.001)
					{
						(*it)->rotation = desiredRotation;
					} else
					{
						(*it)->rotation -= newRotation * 0.5;
					}
			}

			/*if(!GameManager::getInstance()->isServer())
			{
				GameManager::getInstance()->getGraphicsEngine()->drawDebugLine(parent->position.asVector3d(),(*it)->position.asVector3d());
			}*/
		}
		i++;
	}
	}
	if(!selectedTarget)
	{
		selectedTarget = true;
		messageNewTargetPosition(path->getEnd());
	}
}



void ComponentAIBomber::updateReturn() 
{
	messageNewTargetPosition(path->getEnd());

	if(distanceFromThePath() < 15)
	{

		state = EnemyState::MOVE;
		selectAim();
	}
}


void ComponentAIBomber::deleteObject(GameObject* targetObject)
{

	if(targetObject->getTag() == GameObject::MAIN_PLAYER)
	{
		std::list<GameObject*>::iterator it;
		if(!players.empty())
		{
			for (it=players.begin(); it!=players.end(); ++it)
			{	
				if( *it == targetObject)
				{
					players.erase(it);

					break;
				}
			}
		}
	}
	else
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
}


void ComponentAIBomber::addObject(GameObject* gameObject)
{


	if(gameObject->getTag() != GameObject::MAIN_PLAYER)
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
	else
	{
		players.push_back(gameObject);
	}
}

// Selecciona un nuevo objetivo
void ComponentAIBomber::selectAim()
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
			MapManager* mapManager = gameManager->getMapManager();
			GraphicsEngine *graphicsEngine = gameManager->getGraphicsEngine();


			if(mapManager->canSeeTarget(parent->position,target->position,5)) && GameManager::getInstance()->getCollisionManager()->canSeeTarget(parent->position,target->position, parent->getId(), target->getId()))
			if(GameManager::getInstance()->getCollisionManager()->canSeeTarget(parent->position,target->position, parent->getId(), target->getId())) // <- no hace nada
			{	

				if(!shooting)
				{					
					messageNewAim(target);
				}

			}
			else
			{		
				if(target->isDead() || target == NULL)
				{
					
					messageStopAIM();
					messageNewTarget(target);

					shooting = false;
				}
			}*/

		}
		else
		{	
			if(target->isDead() || target == NULL)
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
	


void ComponentAIBomber::selectTarget()
{
	GameObject::removeDeadOnesFromList(onVisionList);

	if(!onVisionList.empty())
	{
		target=NULL;

		if(!onVisionList.empty())
		{
			target = onVisionList.front();
			watching = true;	
			messageNewTarget(target);
			selectAim();
			state = EnemyState::ATTACK;
		}
		
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

void ComponentAIBomber::messageNewTarget(GameObject* gameObject)
{
	Message message;
	message.type = Message::STOP_SHOOTING;
	parent->broadcastMessage(message);

	Message messageToSend;
	messageToSend.type = Message::NEW_TARGET;
	messageToSend.gameObject = gameObject;
	parent->broadcastMessage(messageToSend);
	shooting = false;
}

//Esta funcion  envia el gameObject al que se va atacar y la posicion actual de la nave, asi se para.
//se puede mandar un gameObject null y la nave se detiene y no ataca a nadie.

void ComponentAIBomber::messageNewAim(GameObject* gameObject)
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


void ComponentAIBomber::messageNewTargetPosition(Vector2d target)
{

	Message messageNewTarget;
	messageNewTarget.type = Message::NEW_TARGET;
	messageNewTarget.target = target;
	parent->broadcastMessage(messageNewTarget);
	
}

void ComponentAIBomber::changeTarget( bool allStates)
{
	if(state == EnemyState::MOVE || allStates)
	{
		int pos = GameManager::getInstance()->getAIEngine()->getNextPath(parent->getName(), path->getPathList());
		if(pos != -1)			
		{
			path = path->getPathList()[pos];
		} 
		messageNewTargetPosition(path->getEnd());
	}
}

float ComponentAIBomber::distanceFromThePath()
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
	/*GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	if(graphicsEngine != NULL)
	{
		if(GameManager::getInstance()->getDebugTools()->isShowingDebug())
		{				
			//	graphicsEngine->drawDebugLine(start.asVector3d(), end.asVector3d());
			graphicsEngine->drawDebugLine(normalPoint.asVector3d(), parent->position.asVector3d());
		}
	}*/

	float distance = parent->position.getDistanceFrom(normalPoint);

	return distance;

}

void ComponentAIBomber::messageStopAIM()
{
	Message message;
	message.type = Message::STOP_AIM;
	parent->broadcastMessage(message);

	message.type = Message::STOP_SHOOTING;
	parent->broadcastMessage(message);
}

