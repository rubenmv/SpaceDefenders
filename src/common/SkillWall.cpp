#include "SkillWall.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "GraphicsEngine.h"
#include "NodeMesh.h"
SkillWall::SkillWall(void)
{
	selected = false;
	nodeMesh = NULL;
	wall = NULL;
	delay = 50;
	levelRequired = 1;
	price = 50;
	active = false;
}


SkillWall::~SkillWall(void)
{
	wall = NULL;
}

void SkillWall::addLogo()
{
	GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_WALL);
}

void SkillWall::update()
{
	if(!isOnCooldown() && active)
	{
		active = false;
		addLogo();
	}

	if(selected && GameManager::getInstance()->getEventManager()->mouseState.leftButtonDown)
	{
		activate();
	}
}

void SkillWall::activate()
{
	active = true;
	GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_ENGINEER);
	deselect();
	timeSinceActivated.restart();
}

void SkillWall::select()
{
	if(!isOnCooldown())
	{
		wall = GameManager::getInstance()->getGameObjectManager()->createInventoryWall(price);
		selected = true;
	}
}

void SkillWall::deselect()
{
	if(wall != NULL)
	{
		wall = NULL;
	}
	selected = false;
}
