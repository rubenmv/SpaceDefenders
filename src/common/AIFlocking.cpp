#include "AIFlocking.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "EventManager.h"

AIFlocking::AIFlocking(void)
{
	gameManager = GameManager::getInstance();
	surround = false;
	defend = false;

	eventManager = gameManager->getEventManager();
	mapManager = gameManager->getMapManager();
	updateTime=-1;
}


AIFlocking::~AIFlocking(void)
{
	gameManager = NULL;
	eventManager = NULL;
	mapManager = NULL;

	// Limpia las listas, no elimina objetos, eso ya lo hara el gameObjectManager
	enemyList.clear();
	playersList.clear();

	updateTime=0;
}

void AIFlocking::cleanup()
{
	// Limpia las listas, no elimina objetos, eso ya lo hara el gameObjectManager
	enemyList.clear();
	playersList.clear();
}


void AIFlocking::init()
{

}

void AIFlocking::update()
{
	updateClockAIF.start();

	checkInput();

	for(unsigned int i=0; i < playersList.size() ;i++)
	{
		Vector2d leaderMovement;

		leaderMovement += obstacleAvoiding(enemyList[0]);
		leaderMovement += evade(playersList[i]->position,enemyList[0]);

		if(leaderMovement.x != 0 || leaderMovement.y != 0)
		{
			Message message;
			message.target = leaderMovement;
			message.type = Message::NEW_FORCE;
			enemyList[0]->broadcastMessage(message);
		}

		for(unsigned int j=1;j<enemyList.size();j++)
		{
			Vector2d movement;

			movement += separation(enemyList[j])*1.2;
			movement += alignament(enemyList[j])*0.05;
			movement += cohesion(enemyList[j])*0.2;

			movement += obstacleAvoiding(enemyList[j])*1.5;

			if(!defend)
			{
				if(!surround)
				{
					movement += followLeader(enemyList[0],enemyList[j]);
				}
				else
				{
					movement += surroundLeader(enemyList[0],enemyList[j],j);
				}
			}
			else
			{
				movement += defendLeader(enemyList[0],enemyList[j],j,playersList[i]);
			}

			movement += evade(playersList[i]->position,enemyList[j]);
			
			enemyList[j]->velocity += movement * 0.1;
			//parent->velocity.truncate(maxSpeed);
			//Debug Lines
			//gameManager->getGraphicsEngine()->drawDebugLine(parent->position.asVector3d(),(parent->position + parent->velocity*20).asVector3d(),2);

			//gameManager->getGraphicsEngine()->drawDebugLine(parent->position.asVector3d(),(parent->position + steering*150).asVector3d(),3);
	
			//Modificaciones en la nave
			enemyList[j]->rotation = enemyList[j]->velocity.getAngle(); 
			enemyList[j]->position += enemyList[j]->velocity * 2;
			/*enemyList[j]->velocity -= movement;
	
			//Modificaciones en la nave
			enemyList[j]->rotation = movement.getAngle(); 
			enemyList[j]->position += movement;
			enemyList[j]->velocity.normalize();*/
		}
	}
	updateTime=updateClockAIF.getElapsedTime();
}

void AIFlocking::addEnemy(GameObject* enemy)
{
	enemyList.push_back(enemy);
}

void AIFlocking::addPlayer(GameObject* player)
{
	playersList.push_back(player);
}

Vector2d AIFlocking::flee(Vector2d posFlee, GameObject* enemy)
{
	//Calculos necesarios del Seek
	Vector2d desired_velocity = enemy->position - posFlee;
	desired_velocity.normalize();
	desired_velocity *= 0.6;
	Vector2d steering = desired_velocity - enemy->velocity;
	return steering;
}

Vector2d AIFlocking::evade(Vector2d target, GameObject* enemy) //no funciona
{
	Vector2d distance = target - enemy->position;
	if(distance.getLength() < 15)
	{
		return flee(target,enemy);
	}

	return Vector2d();
}

Vector2d AIFlocking::seek(Vector2d target, GameObject* enemy)
{
	//Calculos necesarios del Seek
	Vector2d desired_velocity = target - enemy->position;
	desired_velocity.normalize();
	desired_velocity *= 0.6;
	Vector2d steering = desired_velocity - enemy->velocity;
	gameManager->getGraphicsEngine()->drawDebugLine(enemy->position.asVector3d(),(enemy->position + desired_velocity*10).asVector3d(),1);
	return steering;
}

Vector2d AIFlocking::obstacleAvoiding(GameObject* enemy)
{
	Vector2d posWall;
	Vector2d steering;
	Vector2d frame = mapManager->getFrame(enemy->position);
	int xbegin = frame.x-2;
	int ybegin = frame.y-2;
	int xend = frame.x+2;
	int yend = frame.y+2;

	if(xbegin < 0)
	{
		xbegin=0;
	}
	if(ybegin < 0)
	{
		ybegin=0;
	}
	if(xend > 299)
	{	
		xend=299;
	}
	if(yend > 299)
	{
		yend=299;
	}

	MapManager::CollisionMap map = mapManager->getCollisionMap();

	for(int i=xbegin;i<=xend;i++)
	{
		for(int j=ybegin;j<=yend;j++)
		{
			if(!mapManager->isEmpty(j,i))
			{
				posWall = mapManager->getFrameCenter(j,i);
				steering += evade(posWall,enemy);
			}
		}
	}

	return steering;
}

Vector2d AIFlocking::arrive(Vector2d target, GameObject* enemy)
{
	Vector2d desired_velocity = target - enemy->position;
	float distance = desired_velocity.getLength();

	// Check the distance to detect whether the character
	// is inside the slowing area
	if (distance < 10.0) 
	{
		// Inside the slowing area
		desired_velocity.normalize();
		desired_velocity = desired_velocity * 1 * (distance / 10.0);
	} 
	else 
	{
		// Outside the slowing area.
		desired_velocity.normalize();
		desired_velocity *= 1;
	}
 
	// Set the steering based on this
	Vector2d steering = desired_velocity - enemy->velocity;

	return steering;
}

void AIFlocking::checkInput()
{
	if(eventManager->isKeyDown(KEY::KEY_KEY_1))
	{
		surround = !surround;
	}
	if(eventManager->isKeyDown(KEY::KEY_KEY_2))
	{
		defend = !defend;
	}
}

Vector2d AIFlocking::followLeader(GameObject* leader, GameObject* enemy)
{
	Vector2d tv = leader->velocity;
 
	// Calculate the behind point
	tv *= -1;
	tv.normalize();
	tv *= 10;

	Vector2d behindLeader = leader->position + tv;
 
	gameManager->getGraphicsEngine()->drawDebugLine(behindLeader.asVector3d(),(behindLeader+tv*2).asVector3d(),1);
	return arrive(behindLeader, enemy);
}

Vector2d AIFlocking::defendLeader(GameObject* leader, GameObject* enemy, int pos, GameObject* player)
{
	Vector2d direction = player->position - leader->position;
 
	// Calculate the behind point
	direction.rotateBy(pos*20-40,Vector2d(0,0));
	direction.normalize();
	direction *= 20;

	Vector2d behindLeader = leader->position + direction;

	gameManager->getGraphicsEngine()->drawDebugLine(behindLeader.asVector3d(),(behindLeader+direction*2).asVector3d(),2);
	return arrive(behindLeader, enemy);
	//return seek(behindLeader, enemy);
}

Vector2d AIFlocking::surroundLeader(GameObject* leader, GameObject* enemy, int pos)
{
	Vector2d tv = leader->velocity;
 
	// Calculate the behind point
	tv.rotateBy(pos*45,Vector2d(0,0));
	tv.normalize();
	tv *= 10;

	Vector2d behindLeader = leader->position + tv;

	gameManager->getGraphicsEngine()->drawDebugLine(behindLeader.asVector3d(),(behindLeader+tv*2).asVector3d(),2);
	return arrive(behindLeader, enemy);
}

Vector2d AIFlocking::separation(GameObject* enemy)
{
	Vector2d steeringForce;

	for(unsigned int i=0;i<enemyList.size();i++)
	{
		if(enemy != enemyList[i])
		{
			if(enemy->position.getDistanceFrom(enemyList[i]->position) < 20)
			{
				Vector2d toTarget = enemy->position - enemyList[i]->position;
				float length = toTarget.getLength();
				toTarget.normalize();

				steeringForce += toTarget/length;
			}
		}
	}

	return steeringForce;
}

Vector2d AIFlocking::alignament(GameObject* enemy)
{
	Vector2d averageHeading;

	int neighborCount = 0;

	for(unsigned int i=0;i<enemyList.size();i++)
	{
		if(enemy != enemyList[i])
		{
			if(enemy->position.getDistanceFrom(enemyList[i]->position) < 30)
			{
				averageHeading += enemyList[i]->velocity;

				neighborCount++;
			}
		}
	}

	if(neighborCount > 0)
	{
		averageHeading /= (float)neighborCount;

		averageHeading = enemy->velocity - averageHeading;  
	}
	averageHeading.normalize();
	return averageHeading;
}

Vector2d AIFlocking::cohesion(GameObject* enemy)
{
	Vector2d centerOfMass, steeringForce;

	int neighburCount = 0;

	for(unsigned int i=0;i<enemyList.size();i++)
	{
		if(enemy != enemyList[i])
		{
			if(enemy->position.getDistanceFrom(enemyList[i]->position) > 20)
			{
				centerOfMass += enemyList[i]->position;

				neighburCount++;
			}
		}
	}

	if(neighburCount > 0)
	{
		centerOfMass /= (float)neighburCount;
		steeringForce = centerOfMass - enemy->position;
		steeringForce.normalize();
	}

	return steeringForce;
}


unsigned int AIFlocking::getUpdateTime()
{
	return updateTime;
}
