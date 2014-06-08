#include "ComponentCreatingWall.h"
#include "GameManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"

ComponentCreatingWall::ComponentCreatingWall(GameObject* creator1, GameObject* creator2)
{
	this->creator1 = creator1;
	this->creator2 = creator2;
	start = creator1->position;
	end = creator2->position;
}


ComponentCreatingWall::~ComponentCreatingWall(void)
{
	creator1 = NULL;
	creator2 = NULL;
}

void ComponentCreatingWall::update()
{
	if(timeBuild.getElapsedTime() > 5900)
	{
		GameObject* element = GameManager::getInstance()->getGameObjectManager()->createEnergyWall(start,end);

		if(element != NULL)
		{
			element->position = parent->position;
			element->rotation = parent->rotation;
		}
	}

	if(creator1->isDead() || creator2->isDead())
	{
		parent->kill();
	}
}

void ComponentCreatingWall::onMessage(const Message& message)
{

}