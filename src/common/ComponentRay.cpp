#include "ComponentRay.h"
#include "GraphicsEngine.h"
#include "GameManager.h"
#include "CollisionManager.h"
#include "GameObjectManager.h"
ComponentRay::ComponentRay(GameObject* shooter)
{
	this->shooter = shooter;
	Vector2d start = shooter->position + Vector2d::getVector2dByAngle(-shooter->rotation)*5;
	Vector2d direction = Vector2d::getVector2dByAngle(-shooter->rotation)*3;
	Vector2d end = start + direction*14;
	downright = (start+direction).rotateBy(90,start);
	downleft = (start+direction).rotateBy(-90,start);
	upright = (start+direction*9).rotateBy(-90,end);
	upleft = (start+direction*9).rotateBy(90,end);
	canHurt.start();
	//std::cout<<"Ancho: "<<downleft.getDistanceFrom(downright)<<std::endl;
	//std::cout<<"Largo: "<<downleft.getDistanceFrom(upleft)<<std::endl;
}
/*
			end
  upleft___________upright  
		|          |
		|          |
		|          |
		|          |
downleft___________downright
		   start
*/
ComponentRay::~ComponentRay(void)
{
}

void ComponentRay::update()
{
	if(shooter == NULL || shooter->isDead() || parent->isDead())
	{
		parent->kill();
	}
	else
	{
		std::vector<GameObject*> list = GameManager::getInstance()->getCollisionManager()->getGameObjectBetween(upright, upleft, downright, downleft);
		if(canHurt.getElapsedTime() > 200) //Hace falta cambiarlo por un while
		{
			for(size_t i=0;i<list.size();i++)
			{
				if(list[i]->getName() != GameObject::N_ENEMY_DEFENDER || shooter->getTag() == GameObject::MAIN_PLAYER)
				{
					
					if(list.at(i) != NULL && !list.at(i)->isDead())
					{
						if(list.at(i) != shooter)
						{
							/*GameObject* explosion = GameManager::getInstance()->getGameObjectManager()->createFire();
							explosion->position = list.at(i)->position + Vector2d(Math::random(-3,3),Math::random(-3,3));*/
							Message message;
							message.type = Message::INCOMING_DAMAGE;
							message.value = 40;
							list.at(i)->broadcastMessage(message);
						}						
					}
				}
			}
			canHurt.restart();
		}
		if(shooter != NULL && !shooter->isDead() && !parent->isDead())
		{
			Vector2d start = shooter->position + Vector2d::getVector2dByAngle(-shooter->rotation)*5;
			Vector2d direction = Vector2d::getVector2dByAngle(-shooter->rotation)*3;
			Vector2d end = start + direction*33;
			downright = (start+direction).rotateBy(90,start);
			downleft = (start+direction).rotateBy(-90,start);
			upright = (start+direction*32).rotateBy(-90,end);
			upleft = (start+direction*32).rotateBy(90,end);
			parent->position = start;
			parent->rotation = shooter->rotation;
			shooter->velocity *= 0.01;
		}
		else
		{
			parent->kill();
		}
		/*if(!GameManager::getInstance()->isServer())
		{
			GameManager::getInstance()->getGraphicsEngine()->drawDebugLine(upright.asVector3d(),downright.asVector3d());
			GameManager::getInstance()->getGraphicsEngine()->drawDebugLine(downright.asVector3d(),downleft.asVector3d());
			GameManager::getInstance()->getGraphicsEngine()->drawDebugLine(downleft.asVector3d(),upleft.asVector3d());
			GameManager::getInstance()->getGraphicsEngine()->drawDebugLine(upleft.asVector3d(),upright.asVector3d());
		}*/
		
	}
}

void ComponentRay::onMessage(const Message& message)
{

}
