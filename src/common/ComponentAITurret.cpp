#include "ComponentAITurret.h"
#include "GameObject.h"
#include <algorithm>
#include <cctype>

ComponentAITurret::ComponentAITurret(void)
{
	watching = false;
}


ComponentAITurret::~ComponentAITurret(void)
{
}



void ComponentAITurret::update()
{
	if(onVisionList.empty())
	{

		if(!watching)
		{
			messageStopAIM();
			watching = true;
		}
	} else
	{
		GameObject::removeDeadOnesFromList(onVisionList);
	}
}

void ComponentAITurret::deleteObject(GameObject* targetObject)
{
	std::list<GameObject*>::iterator it;
	for (it=onVisionList.begin(); it!=onVisionList.end(); ++it)
	{	
		if( *it == targetObject)
		{
			if(*it == onVisionList.front())
			{
				onVisionList.erase(it);
				selectAim();
			}
			else
			{
				onVisionList.erase(it);
				
				if(onVisionList.size() < 1)
				{
					messageStopAIM();
				}
				else
				{
					Message message;
					message.type = Message::NEW_AIM;
					message.gameObject = onVisionList.front();
					parent->broadcastMessage(message);
				}
				
			}
			watching = false;
			break;
		}
	}

}

void ComponentAITurret::addObject(GameObject* gameObject)
{
	onVisionList.push_back(gameObject);
	

	if(onVisionList.size() > 1)
	{
		//A veces salta excepcion porque puede haber basura dentro de la lista, pasa en las oleadas
		onVisionList.sort(GameObject::selectPrefTurret);
	}

	if(onVisionList.front() == gameObject)
	{
		selectAim();
	}
	
}

// Selecciona un nuevo objetivo
void ComponentAITurret::selectAim()
{
	if(!onVisionList.empty())
	{
		GameObject* newTarget=NULL;

		GameObject::removeDeadOnesFromList(onVisionList);

		if(!onVisionList.empty())
		{
			newTarget = onVisionList.front();
		}


		Message messageToSend;
		messageToSend.type = Message::NEW_AIM;
		messageToSend.gameObject = newTarget;
		parent->broadcastMessage(messageToSend);
	}
}

void ComponentAITurret::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::NEW_VISION:
		addObject(message.gameObject);
		break;
	case Message::LOST_VISION:
		deleteObject(message.gameObject);
		break;
	}
}

void ComponentAITurret::messageStopAIM()
{
	Message message;
	message.type = Message::STOP_AIM;
	parent->broadcastMessage(message);
}