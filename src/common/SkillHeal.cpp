#include "SkillHeal.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
SkillHeal::SkillHeal(void)
{
	selected = false;
	nodeMesh = NULL;
	delay = 5000;
	levelRequired = 1;
	price = 0;
	radius = 20;
	active = true;
}

void SkillHeal::addLogo()
{
	GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,"../assets/images/logos/heal_zone.png");
}

void SkillHeal::addBlackLogo()
{
	GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,"../assets/images/logos/heal_zone_black.png");
}

void SkillHeal::addDescription()
{
	std::wstring description = L"Zone Heal";
	description.append(L" Delay: ");
	description.append(std::to_wstring(delay/1000));
	description.append(L" \nLevel: ");
	description.append(std::to_wstring((int)levelRequired));
	description.append(L" Cost: ");
	description.append(std::to_wstring((int)price));

	GameManager::getInstance()->getGraphicsEngine()->addDescriptionText(skillNumber,description);
}

SkillHeal::~SkillHeal(void)
{
	if(nodeMesh != NULL)
	{
		delete nodeMesh;
		nodeMesh = NULL;
	}
}

void SkillHeal::update()
{
	if(!isOnCooldown() && active)
	{
		active = false;
		addLogo();
	}

	if(selected && GameManager::getInstance()->getEventManager()->mouseState.leftButtonDown)
	{
		GameManager::getInstance()->getEventManager()->mouseState.leftButtonDown = false;
		activate();
	}

	if(nodeMesh != NULL && selected)
	{
		nodeMesh->setPosition(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
	}
}

void SkillHeal::activate()
{
	active = true;
	addBlackLogo();
	deselect();
	if(GameManager::getInstance()->getNetworkEngine()->isStarted())
	{
		GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_SKILL_HEAL,GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
	}
	else
	{
		GameManager::getInstance()->getGameObjectManager()->createSkillHeal(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
		GameManager::getInstance()->getGameObjectManager()->animationHeal(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
	}
	timeSinceActivated.start();
}

void SkillHeal::select()
{
	if(!isOnCooldown())
	{
		selected = true;
		nodeMesh = GameManager::getInstance()->getGraphicsEngine()->addNodeMesh("../assets/models/others/target/target_zone_green.obj",true);
		nodeMesh->setPosition(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
		nodeMesh->setScale(Vector3d(radius*2,1,radius*2));
	}
}

void SkillHeal::deselect()
{
	delete nodeMesh;
	nodeMesh = NULL;
	selected = false;
}
