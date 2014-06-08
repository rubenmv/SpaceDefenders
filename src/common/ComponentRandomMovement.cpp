#include "ComponentRandomMovement.h"
#include "GameObject.h"
#include "GameManager.h"
#include "Math.h"

ComponentRandomMovement::ComponentRandomMovement(void)
{
		this->positionVector = 0;
		Vector2d target1(100,100);
		Vector2d target2(-100,100);
		Vector2d target3(-100,-100);
		Vector2d target4(100,-100);

		targetList.push_back(target1);
		targetList.push_back(target2);
		targetList.push_back(target3);
		targetList.push_back(target4);
		/*
		int dentro = 0;

		bool t1 = false;
		bool t2 = false;
		bool t3 = false;
		bool t4 = false;

		while(dentro != 4)
		{
			int random = Math::random(1,5);

			if(random == 1 && !t1)
			{
				this->targetList.push_back(target1);
				dentro++;
				t1 = true;
			}
			else if(random == 2 && !t2)
			{
				this->targetList.push_back(target2);
				dentro++;
				t2 = true;
			}
			else if(random == 3 && !t3)
			{
				this->targetList.push_back(target3);
				dentro++;
				t3 = true;
			}
			else if(random == 4 && !t4)
			{
				this->targetList.push_back(target4);
				dentro++;
				t4 = true;
			}
		}*/
}

void ComponentRandomMovement::awake()
{
	changeTarget();
}


ComponentRandomMovement::~ComponentRandomMovement(void)
{
}

void ComponentRandomMovement::update()
{
	
	/*//parent->rotation;

	Vector2d direction = Vector2d(Math::sin((parent->rotation+90) * Math::DEGTORAD), Math::cos((parent->rotation+90) * Math::DEGTORAD));

	parent->position += direction * GameManager::getInstance()->getDeltaTime()*0.02;

	if(parent->position.x > 100 || parent->position.x < -100 || parent->position.y >100 || parent->position.y < -100)
		parent->rotation += 90;

	if(parent->rotation > 360)
		parent->rotation -= 360;*/
	
}

void ComponentRandomMovement::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::TARGET_REACHED: 
		changeTarget();
		break;
	case Message::COLLISION: 
		changeTarget(message.target, message.gameObject);
		break;
	}
}

void ComponentRandomMovement::changeTarget()
{
	positionVector++;
	if(positionVector ==  targetList.size())
	{
		positionVector = 0;			
	}

	Message message;
	message.type = Message::NEW_TARGET;
	message.target = targetList[positionVector];
	parent->broadcastMessage(message);
}

void ComponentRandomMovement::changeTarget(Vector2d direction, GameObject* targetObject)
{
	int rand = Math::random(0,2);
	if(targetObject->getTag() != GameObject::PROJECTILE)
	{
		newDirection(direction);
	}
	else if(rand) // 0 o 1 si choca con proyectil
	{
		newDirection(direction);
	}
}

void ComponentRandomMovement::newDirection( Vector2d &direction )
{
	Vector2d target = direction.normalize()*(-80) + parent->position;
	Message message;
	message.type = Message::NEW_TARGET;
	message.target = target;
	parent->broadcastMessage(message);
}
