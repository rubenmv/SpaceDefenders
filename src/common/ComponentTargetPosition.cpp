#include "ComponentTargetPosition.h"
#include "GameObject.h"

ComponentTargetPosition::ComponentTargetPosition(void)
{

}


ComponentTargetPosition::~ComponentTargetPosition(void)
{
}

void ComponentTargetPosition::update()
{

}

void ComponentTargetPosition::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::NEW_TARGET:
		changePosition(message.target);
		break;
	}
}

void ComponentTargetPosition::changePosition(Vector2d newTarget )
{
	parent->position = newTarget;
}