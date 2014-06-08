#include "ComponentDebugGameObject.h"
#include <iostream>
#include <sstream>

#include "NodeText.h"
#include "GraphicsEngine.h"
#include "GameManager.h"
#include "DebugTools.h"
#include "GameObject.h"
#include "Math.h"



ComponentDebugGameObject::ComponentDebugGameObject(void)
{
	showPosition = false;

	relativeTextPosition = Vector2d(20, 20);	

	graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	node = graphicsEngine->addNodeText();

	//Hasta que la vida no cambie saldra -1
	shield = -1;
	health = -1;
	jade = -1;
}


ComponentDebugGameObject::~ComponentDebugGameObject(void)
{
	delete node;
}

void ComponentDebugGameObject::update()
{
	Vector2d playerScreenPosition = graphicsEngine->getScreenCoordinatesFrom3DPosition(parent->position.asVector3d());

	node->setPosition(playerScreenPosition + relativeTextPosition);

	std::wstringstream string;

	if(GameManager::getInstance()->getDebugTools()->isShowingDebug())
	{
		string << parent->getName() << std::endl ;
		string << "Pos: " << parent->position << std::endl;
		string << "Vel: " << parent->velocity << std::endl;
		string << Math::round(parent->rotation, 1) << "º" << std::endl;
		if(shield != -1)
		{
			string << "SP: ";
			for(int i = 0; i < shield/10; i++)
			{
				string << "#";
			}
			string << std::endl;
		}
		if(health != -1)
		{
			string << "HP: ";
			for(int i = 0; i < health/10; i++)
			{
				string << "#";
			}
			string << std::endl;
		}
		if(jade != -1)
		{
			string << "Jade: ";
			string << jade;
			string << std::endl;
		}
		/*if(parent->getTag() == GameObject::ENEMY)
		{
			graphicsEngine->setDebugText(string.str().c_str());
		}*/
	}

	//graphicsEngine->drawDebugLine(parent->position.asVector3d(),parent->velocity.asVector3d(),4);

	//string << parent->getTag() << ": ";
	

	node->setText(string.str().c_str());
}

void ComponentDebugGameObject::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::HEALTH_CHANGED:
		onHealthChanged(message.target.x);
		break;
	case Message::SHIELD_CHANGED:
		onShieldChanged(message.target.x);
		break;
	case Message::JADE_CHANGED:
		onJadeChanged(message.value);
		break;
	}
}


void ComponentDebugGameObject::onHealthChanged(float newHealth)
{
	health = newHealth;
}

void ComponentDebugGameObject::onShieldChanged(float newShield)
{
	shield = newShield;
}

void ComponentDebugGameObject::onJadeChanged(int newJade)
{
	jade = newJade;
}