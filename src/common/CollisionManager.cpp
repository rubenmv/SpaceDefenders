#include "CollisionManager.h"
#include "ComponentCollider.h"
#include "ComponentVision.h"
#include <iostream>
#include <vector>

CollisionManager::CollisionManager(void)
{
	updateTime=-1;
}

CollisionManager::~CollisionManager(void)
{
	updateTime=0;
	colliderEnemyList.clear();
	colliderAlliesList.clear();
	visionList.clear();
}

void CollisionManager::addComponentCollider(ComponentCollider* collider)
{
	if(collider->getGameObject()->isEnemyTeam())
	{
		colliderEnemyList.push_back(collider);
	} else
	{
		colliderAlliesList.push_back(collider);
	}
}

void CollisionManager::addComponentVision(ComponentVision* vision)
{
	visionList.push_back(vision);
}

void CollisionManager::update()
{
	updateClockCM.start();
	ComponentVision* currentVision = NULL;
	ComponentCollider* targetCollider = NULL;
	GameObject *currentGameObject = NULL;
	

	// VISION. Colisiones con campos de vision
	int listSize = visionList.size();
	for(int i = 0; i < listSize; i++)
	{
		currentVision = visionList[i];
		currentGameObject = currentVision->getGameObject();
		if(currentGameObject->isDead())
		{
			continue;
		}
		bool isInventory =  currentGameObject->getTag() == GameObject::INVENTORY;
		bool isEnemyTeam = currentGameObject->isEnemyTeam();
		// Comprueba si la vision actual colisiona con algun objeto
		if(!isEnemyTeam || isInventory)
		{
			int colliderSize = colliderEnemyList.size();
			for(int j = 0; j < colliderSize; j++)
			{
				targetCollider = colliderEnemyList[j];
				checkVisionBetween(currentVision, targetCollider);
			}
		}

		if(isEnemyTeam || isInventory)
		{
			int colliderSize = colliderAlliesList.size();
			for(int j = 0; j < colliderSize; j++)
			{
				targetCollider = colliderAlliesList[j];         
				checkVisionBetween(currentVision, targetCollider);              
			}
		}
		
	}
	
	
	// COLLIDER. Colisiones directas entre objetos

	//Colisiones entre aliados vs aliados y aliados vs enemigos

	listSize = colliderAlliesList.size();
	for(int i=0;i<listSize;i++)
	{
		ComponentCollider* currentCollider = colliderAlliesList[i];
		if(currentCollider->getGameObject()->isDead())
		{
			continue;
		}
		bool currentIsStatic = currentCollider->getGameObject()->isStatic();
		bool currentIsRemote = currentCollider->getGameObject()->getTag() == GameObject::REMOTE;
		// Recorre lista y comprueba colision entre dos elementos de la lista 
		// y si colisionan llama a la funcion onCollision() de los dos
		for(int j = i + 1; j < listSize; j++)
		{
			targetCollider = colliderAlliesList[j];

			if(currentIsStatic && targetCollider->getGameObject()->isStatic())
			{
				//Los dos son estaticos y no pueden chocarse
				continue;
			}

			if(currentIsRemote && targetCollider->getGameObject()->getTag() == GameObject::REMOTE)
			{
				//Los dos son remotes y el que decida si se chocan es el servidor no los remotes
				continue;
			}
			checkCollisionBetween(currentCollider, targetCollider);     
		}

		int enemyListSize = colliderEnemyList.size();
		for(int j = 0; j < enemyListSize; j++)
		{
			targetCollider = colliderEnemyList[j];

			//Por ahora no hay enemigos estaticos por lo que no hace falta comprobar si los dos son estaticos

			if(currentIsRemote && targetCollider->getGameObject()->getTag() == GameObject::REMOTE)
			{
				//Los dos son remotes y el que decida si se chocan es el servidor no los remotes
				continue;
			}
			checkCollisionBetween(currentCollider, targetCollider);     
		}
	}

	//Colisiones entre enemigos vs enemigos
	listSize = colliderEnemyList.size();
	for(int i=0;i<listSize;i++)
	{

		ComponentCollider* currentCollider = colliderEnemyList[i];
		if(currentCollider->getGameObject()->isDead())
		{
			continue;
		}
		bool currentIsRemote = currentCollider->getGameObject()->getTag() == GameObject::REMOTE;
		// Recorre lista y comprueba colision entre dos elementos de la lista 
		// y si colisionan llama a la funcion onCollision() de los dos

		Vector2d filter = currentCollider->getGameObject()->position;

		for(int j = i + 1; j < listSize; j++)
		{
			targetCollider = colliderEnemyList[j];
			//Por ahora no hay enemigos estaticos por lo que no hace falta comprobar si los dos son estaticos

			if(currentIsRemote && targetCollider->getGameObject()->getTag() == GameObject::REMOTE)
			{
				//Los dos son remotes y el que decida si se chocan es el servidor no los remotes
				continue;
			}

			//Filtro un poco sucio pero mejora un 20% el rendimiento del collision manager
			//Funcionara mientras no haya enemigos con un collider de mas de 10
			if(Math::abs(filter.y - targetCollider->getGameObject()->position.y) < 20)
			{
				if(Math::abs(filter.x - targetCollider->getGameObject()->position.x) < 20)
				{
					checkCollisionBetween(currentCollider, targetCollider);     

				}
			}
		}
	}

	updateTime =updateClockCM.getElapsedTime();
}

// Comprueba colision entre Vision y Collider. Un objeto entra en la vision de otro
void CollisionManager::checkVisionBetween (ComponentVision* visionCollider, ComponentCollider* targetCollider)
{
	//Optimizaciones de memoria
	GameObject *targetGameObject = targetCollider->getGameObject();

	if(targetGameObject->isDead())
	{
		return;
	}

	// Distancia entre los objetos
	Vector2d direction = targetGameObject->position - visionCollider->getGameObject()->position;
	float distance = direction.getSqrLength();

	float sqrVisionRadius = visionCollider->getSqrVisionRadius();
	float sqrTargetRadius = targetCollider->getSqrCollisionRadius();

	// Dentro de vision
	if(distance <= sqrVisionRadius + sqrTargetRadius)
	{
		Collision collision;
		collision.collider = targetGameObject;
		collision.direction = direction;
		visionCollider->onVision(collision);
	}
}

// Comprueba colision entre Colliders. Dos objetos se tocan/colisionan
void CollisionManager::checkCollisionBetween (ComponentCollider* currentCollider, ComponentCollider* targetCollider)
{
	GameObject *targetGameObject = targetCollider->getGameObject();

	//if(targetGameObject->isDead())
	//{
//		return;
//	}

	// Direccion entre objetos
	//Al hacerlo en una linea nos ahorramos crear un vector2d muchas veces
	float distance = (targetGameObject->position - currentCollider->getGameObject()->position).getSqrLength();              

	float sqrCollisionRadius = currentCollider->getSqrCollisionRadius();
	float sqrTargetRadius = targetCollider->getSqrCollisionRadius();

	// Colisionan. Se activa una colision en los dos objetos involucrados

	if(distance <= sqrCollisionRadius + sqrTargetRadius)
	{
		if(distance == 0)
		{
			targetGameObject->position += Vector2d(0,0.01);
			distance = (targetGameObject->position - currentCollider->getGameObject()->position).getSqrLength();              

		}
		Vector2d direction = targetGameObject->position - currentCollider->getGameObject()->position;
		direction.normalize();
		direction *= Math::sqrt(Math::abs(distance - (sqrCollisionRadius + sqrTargetRadius)));
		// La colision se envia con un objeto que es con el cual choca
		Collision collision;
		collision.collider = targetGameObject;
		collision.direction = direction;
		currentCollider->onCollision(collision);

		//La colision deberia llegar a los dos, si no, deberia ser checkVision...
		collision.collider = currentCollider->getGameObject();
		collision.direction = direction * -1;
		targetCollider->onCollision(collision);
	}
}

// Elimina un Collider de la lista
void CollisionManager::removeFromList(ComponentCollider* colliderToRemove)
{
	std::vector<ComponentCollider*>::iterator iterator;
	std::vector<ComponentCollider*>::iterator lastIterator;
	std::vector<ComponentCollider*>* colliderList;

	if(colliderToRemove->getGameObject()->isEnemyTeam())
	{
		iterator = colliderEnemyList.begin();
		lastIterator = colliderEnemyList.end();
		colliderList = &colliderEnemyList;
	} else
	{
		iterator = colliderAlliesList.begin();
		lastIterator = colliderAlliesList.end();
		colliderList = &colliderAlliesList;
	}

	
	while(iterator != lastIterator)
	{
		ComponentCollider* collider = *iterator;
		if(collider == colliderToRemove)
		{
			colliderList->erase(iterator);
			return;
		}
		else
		{
			++iterator;
		}
	}
}

// Elimina un Vision de la lista
void CollisionManager::removeFromList(ComponentVision* visionToRemove)
{

	std::vector<ComponentVision*>::iterator iterator = visionList.begin();
	while(iterator != visionList.end())
	{
		ComponentVision* collider = *iterator;
		if(collider == visionToRemove)
		{
			visionList.erase(iterator);
			return;
		}
		else
		{
			++iterator;
		}
	}

}

// Obtiene el objeto en una posicion indicada. Solo objetos registrados con collider
GameObject* CollisionManager::getGameObjectIn( Vector2d position )
{
	for(std::size_t i = 0; i < colliderAlliesList.size(); i++)
	{
		Vector2d direction = position - colliderAlliesList[i]->getGameObject()->position;
		float distance = direction.getSqrLength();

		if(distance <= colliderAlliesList[i]->getSqrCollisionRadius())
		{
			return colliderAlliesList[i]->getGameObject();
		}
	}

	for(std::size_t i = 0; i < colliderEnemyList.size(); i++)
	{
		Vector2d direction = position - colliderEnemyList[i]->getGameObject()->position;
		float distance = direction.getSqrLength();

		if(distance <= colliderEnemyList[i]->getSqrCollisionRadius())
		{
			return colliderEnemyList[i]->getGameObject();
		}
	}
	return NULL;
}

unsigned int CollisionManager::getUpdateTime()
{
	return updateTime;
}

float sign(Vector2d p1, Vector2d p2, Vector2d p3)
{
  return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle(Vector2d center, Vector2d v1, Vector2d v2, Vector2d v3)
{
  bool b1, b2, b3;

  b1 = sign(center, v1, v2) < 0.0f;
  b2 = sign(center, v2, v3) < 0.0f;
  b3 = sign(center, v3, v1) < 0.0f;

  return ((b1 == b2) && (b2 == b3));
}

bool VertexRectangleInCircle(Vector2d center, float radius, Vector2d v1, Vector2d v2, Vector2d v3, Vector2d v4)
{
	if(v1.getSqrDistanceFrom(center) <= radius*radius || v2.getSqrDistanceFrom(center) <= radius*radius || 
		v3.getSqrDistanceFrom(center) <= radius*radius || v4.getSqrDistanceFrom(center) <= radius*radius)
	{
		return true;
	}

	return false;
}

bool LineCircleIntersect(Vector2d center, float radius, Vector2d v1, Vector2d v2)
{
	Vector2d distc_v1 = center- v1;
	Vector2d distc_v2 = v2 - v1;
	
	float k = distc_v1.dot(distc_v2);

	if(k > 0)
	{
		float len = distc_v2.getLength();
		k = k/len;

		if(k < len)
		{
			if((distc_v1.getSqrLength() - k*k) <= radius*radius)
			{
				return true;
			}
		}
	}

	return false;
}

std::vector<GameObject*> CollisionManager::getGameObjectBetween(Vector2d upright, Vector2d upleft, Vector2d downright, Vector2d downleft)
{
	std::vector<GameObject*> list;
	GameObject* gameObject = NULL;
	Vector2d position;
	float radius;
	for(std::size_t i = 0; i < colliderEnemyList.size(); i++)
	{
		position = colliderEnemyList[i]->getGameObject()->position;
		gameObject = colliderEnemyList[i]->getGameObject();
		radius = Math::sqrt(colliderEnemyList[i]->getSqrCollisionRadius());
		if (PointInTriangle(position,upleft,downleft,downright))
		{
			list.push_back(gameObject);
		}
		else if(PointInTriangle(position,upleft,upright,downright))
		{
			list.push_back(gameObject);
		}
		else if(VertexRectangleInCircle(position,radius,upleft,downleft,downright,upright))
		{
			list.push_back(gameObject);
		}
		else if(LineCircleIntersect(position,radius,downright,upright))
		{
			list.push_back(gameObject);
		}
		else if(LineCircleIntersect(position,radius,downleft,downright))
		{
			list.push_back(gameObject);
		}
		else if(LineCircleIntersect(position,radius,upleft,downleft))
		{
			list.push_back(gameObject);
		}
		else if(LineCircleIntersect(position,radius,upleft,upright))
		{
			list.push_back(gameObject);
		}
	}
	
	for(std::size_t i = 0; i < colliderAlliesList.size(); i++)
	{
		position = colliderAlliesList[i]->getGameObject()->position;
		gameObject = colliderAlliesList[i]->getGameObject();
		radius = Math::sqrt(colliderAlliesList[i]->getSqrCollisionRadius());
		if (PointInTriangle(position,upleft,downleft,downright))
		{
			list.push_back(gameObject);
		}
		else if(PointInTriangle(position,upleft,upright,downright))
		{
			list.push_back(gameObject);
		}
		else if(VertexRectangleInCircle(position,radius,upleft,downleft,downright,upright))
		{
			list.push_back(gameObject);
		}
		else if(LineCircleIntersect(position,radius,downright,upright))
		{
			list.push_back(gameObject);
		}
		else if(LineCircleIntersect(position,radius,downleft,downright))
		{
			list.push_back(gameObject);
		}
		else if(LineCircleIntersect(position,radius,upleft,downleft))
		{
			list.push_back(gameObject);
		}
		else if(LineCircleIntersect(position,radius,upleft,upright))
		{
			list.push_back(gameObject);
		}
	}
	return list;
}


bool CollisionManager::canSeeTarget( Vector2d start, Vector2d end, int id1, int id2 )
{
	return true;

	/*Vector2d AB = Vector2d(end.x - start.x, end.y - start.y);

	int precision = 10;

	std::vector<Vector2d> pointList;

	pointList.push_back(start);
	pointList.push_back(end);

	
	for (int i = 1; i < precision - 1; i++)
	{
		pointList.push_back(start + AB*i/precision); //Esta seria mas exacta y no hace falta utilizar temp, el for tendria que llegar hasta precision -1
	}
	

	for(std::size_t i = 0; i < colliderList.size(); i++)
	{
		Vector2d point = colliderList[i]->getGameObject()->position;
		if(colliderList[i]->getGameObject()->getId() != id1 && 
		   colliderList[i]->getGameObject()->getId() != id2 &&
		   colliderList[i]->getGameObject()->getTag() != GameObject::PROJECTILE &&
		   colliderList[i]->getGameObject()->getTag() != GameObject::BULLET)
		{
			for(std::size_t  j = 0; j < pointList.size();j++)
			{
				float sqrRadius = colliderList[i]->getCollisionRadius() * colliderList[i]->getCollisionRadius();
				if(point.getSqrDistanceFrom(pointList[j]) < sqrRadius - 1) //menus uno para asegurarnos que queda dentro
				{                   
					return false;
				}
			}
		}       
		
	}
	*/
	/*
	Vector2d center;
	for(std::size_t i = 0; i < colliderList.size(); i++)
	{
		if(colliderList[i]->getGameObject()->getId() == id2)
		{
			center = colliderList[i]->getGameObject()->position;
		}
	}
	
	if(center.getDistanceFrom(start)<start.getDistanceFrom(end)+2 &&center.getDistanceFrom(end)<start.getDistanceFrom(end)+2)
	{
		float dx = end.x - start.x;
		float dy = end.y - start.y;
		float a = dx * dx + dy * dy;
		float b = 2 * (dx * (start.x - center.x) + dy * (start.y - center.y));
		float cc = center.x * center.x + center.y * center.y;
		cc += start.x * start.x + start.y * start.y;
		cc -= 2 * (center.x * start.x + center.y * start.y);
		cc -= 4 * 4;
		float bb4ac = b * b - 4 * a * cc;

		if(bb4ac<0)
		{
			return false;    // No collision
		}
		else
		{
			return true;
		}
	}

	return false;*/
}
