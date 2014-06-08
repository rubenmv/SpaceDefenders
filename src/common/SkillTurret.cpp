#include "SkillTurret.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "NodeMesh.h"
SkillTurret::SkillTurret(int name)
{
	selected = false;
	turret = NULL;
	delay = 50;
	nodeMesh = NULL;
	switch (name)
	{
	case GameObject::N_TURRET_2_CANON:
		levelRequired = 1;
		price = 300;
		break;
	case GameObject::N_TURRET_BULLET:
		levelRequired = 1;
		price = 400;
		break;
	case GameObject::N_TURRET_PROJECTILE:
		levelRequired = 3;
		price = 500;
		break;	
	case GameObject::N_TURRET_BIG_CANON:
		levelRequired = 5;
		price = 600;
		break;
	case GameObject::N_TURRET_1_CANON:
		levelRequired = 6;
		price = 700;
		break;
	case GameObject::N_TURRET_GUN:
		levelRequired = 6;
		price = 300;
		break;
	}
	active = false;
	this->name = (GameObject::Name)name;
}


SkillTurret::~SkillTurret(void)
{
	turret = NULL;
}

void SkillTurret::addDescription()
{
	std::wstring description = L"Turret";
	description.append(L" Delay: ");
	description.append(std::to_wstring(delay/1000));
	description.append(L" \nLevel: ");
	description.append(std::to_wstring((int)levelRequired));
	description.append(L" Cost: ");
	description.append(std::to_wstring((int)price));

	GameManager::getInstance()->getGraphicsEngine()->addDescriptionText(skillNumber,description);
}

void SkillTurret::addLogo()
{
	switch (name)
	{
	case GameObject::N_TURRET_2_CANON:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_TURRET_2);
		break;
	case GameObject::N_TURRET_BIG_CANON:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_TURRET_BIG_CANON);
		break;
	case GameObject::N_TURRET_BULLET:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_TURRET_2);
		break;
	case GameObject::N_TURRET_PROJECTILE:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_TURRET);
		break;
	case GameObject::N_TURRET_1_CANON:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_TURRET_1_CANON);
		break;
	case GameObject::N_TURRET_GUN:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_MINE);
		break;
	}
	
}

void SkillTurret::addBlackLogo()
{
	switch (name)
	{
	case GameObject::N_TURRET_2_CANON:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_TURRET_2_BLACK);
		break;
	case GameObject::N_TURRET_BIG_CANON:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_TURRET_BIG_CANON_BLACK);
		break;
	case GameObject::N_TURRET_BULLET:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_TURRET_2_BLACK);
		break;
	case GameObject::N_TURRET_PROJECTILE:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_TURRET_BLACK);
		break;
	case GameObject::N_TURRET_1_CANON:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_TURRET_1_CANON_BLACK);
		break;
	case GameObject::N_TURRET_GUN:
		GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_MINE_BLACK);
		break;
	}
	
}

void SkillTurret::update()
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

void SkillTurret::activate()
{
	active = true;
	addBlackLogo();
	deselect();
	timeSinceActivated.start();
}

void SkillTurret::select()
{
	if(!isOnCooldown())
	{
		switch (name)
		{
		case GameObject::N_TURRET_2_CANON:
			turret = GameManager::getInstance()->getGameObjectManager()->createInventoryTurret2Canon(price);
			break;
		case GameObject::N_TURRET_BIG_CANON:
			turret = GameManager::getInstance()->getGameObjectManager()->createInventoryTurretBigCanon(price);
			break;
		case GameObject::N_TURRET_BULLET:
			turret = GameManager::getInstance()->getGameObjectManager()->createInventoryTurret2(price);
			break;
		case GameObject::N_TURRET_PROJECTILE:
			turret = GameManager::getInstance()->getGameObjectManager()->createInventoryTurret(price);
			break;
		case GameObject::N_TURRET_1_CANON:
			turret = GameManager::getInstance()->getGameObjectManager()->createInventoryTurret1Canon(price);
			break;
		case GameObject::N_TURRET_GUN:
			turret = GameManager::getInstance()->getGameObjectManager()->createInventoryTurretGun(price);
			break;
		}
		nodeMesh = GameManager::getInstance()->getGraphicsEngine()->addNodeMesh("../assets/models/others/target/zone_construction.obj",true);
		nodeMesh->setPosition(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
		nodeMesh->setScale(Vector3d(60,1,60));
		selected = true;
	}
}

void SkillTurret::deselect()
{
	delete nodeMesh;
	nodeMesh = NULL;
	if(turret != NULL)
	{
		turret = NULL;
	}

	turret = NULL;
	selected = false;
}

