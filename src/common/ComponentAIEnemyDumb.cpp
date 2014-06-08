#include "ComponentAIEnemyDumb.h"
#include "GameObject.h"
#include "GameManager.h"
#include "GameObjectManager.h"
#include "Message.h"

ComponentAIEnemyDumb::ComponentAIEnemyDumb(std::vector<Vector2d> targetList, int startPositionIndex)
{
	this->positionVector = startPositionIndex;
	this->targetList = targetList;

	if(this->positionVector > (int) targetList.size() || this->positionVector < 0)
	{
		this->positionVector = 0;
	}
}

void ComponentAIEnemyDumb::awake()
{
	changedTarget();
}

ComponentAIEnemyDumb::~ComponentAIEnemyDumb()
{
}

void ComponentAIEnemyDumb::update()
{


}

void ComponentAIEnemyDumb::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::TARGET_REACHED: 
		changedTarget();
		break;
	}
}

void ComponentAIEnemyDumb::changedTarget()
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

