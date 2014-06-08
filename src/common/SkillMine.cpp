#include "SkillMine.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "NodeMesh.h"
SkillMine::SkillMine(int name)
{
	selected = false;
	nodeMesh = NULL;
	mine = NULL;
	delay = 50;
	levelRequired = 1;
	price = 100;
	active = false;
	this->name = (GameObject::Name)name;
}

void SkillMine::addLogo()
{
	switch (name)
	{
	case GameObject::N_MINE:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_MINE);
		break;
	case GameObject::N_MINE2:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_MINE_2);
		break;
	case GameObject::N_MINE3:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_MINE_3);
		break;
	}
}

void SkillMine::addBlackLogo()
{
	switch (name)
	{
	case GameObject::N_MINE:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_MINE_BLACK);
		break;
	case GameObject::N_MINE2:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_MINE_2_BLACK);
		break;
	case GameObject::N_MINE3:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_MINE_3_BLACK);
		break;
	}
}

void SkillMine::addDescription()
{
	std::wstring description = L"Mine";
	description.append(L" Delay: ");
	description.append(std::to_wstring(delay/1000));
	description.append(L" \nLevel: ");
	description.append(std::to_wstring((int)levelRequired));
	description.append(L" Cost: ");
	description.append(std::to_wstring((int)price));

	GameManager::getInstance()->getGraphicsEngine()->addDescriptionText(skillNumber,description);
}

SkillMine::~SkillMine(void)
{
	mine = NULL;
}


void SkillMine::update()
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

	if(nodeMesh != NULL && selected)
	{
		nodeMesh->setPosition(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
	}
}

void SkillMine::activate()
{
	active = true;
	addBlackLogo();
	deselect();
	timeSinceActivated.start();
}

void SkillMine::select()
{
	if(!isOnCooldown())
	{
		switch (name)
		{
		case GameObject::N_MINE:
			mine = GameManager::getInstance()->getGameObjectManager()->createInventoryMine(price);
			break;
		case GameObject::N_MINE2:
			mine = GameManager::getInstance()->getGameObjectManager()->createInventoryMine2(price);
			break;
		case GameObject::N_MINE3:
			mine = GameManager::getInstance()->getGameObjectManager()->createInventoryMine3(price);
			break;
		}
		nodeMesh = GameManager::getInstance()->getGraphicsEngine()->addNodeMesh("../assets/models/others/target/zone_construction.obj",true);
		nodeMesh->setPosition(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
		nodeMesh->setScale(Vector3d(60,1,60));
		selected = true;
	}
}

void SkillMine::deselect()
{
	delete nodeMesh;
	nodeMesh = NULL;
	if(mine != NULL && mine->isDead())
	{
		mine = NULL;
	}

	mine = NULL;
	selected = false;
}
