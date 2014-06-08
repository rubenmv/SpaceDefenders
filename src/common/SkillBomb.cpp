#include "SkillBomb.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
SkillBomb::SkillBomb(void)
{
	selected = false;
	nodeMesh = NULL;
	delay = 15000;
	levelRequired = 8;
	price = 0;
	radius = 20;
	active = true;
}

void SkillBomb::addDescription()
{
	std::wstring description = L"Zone Bomb";
	description.append(L" Delay: ");
	description.append(std::to_wstring(delay/1000));
	description.append(L" \nLevel: ");
	description.append(std::to_wstring((int)levelRequired));
	description.append(L" Cost: ");
	description.append(std::to_wstring((int)price));

	GameManager::getInstance()->getGraphicsEngine()->addDescriptionText(skillNumber,description);
}

SkillBomb::~SkillBomb(void)
{
	if(nodeMesh != NULL)
	{
		delete nodeMesh;
		nodeMesh = NULL;
	}
}

void SkillBomb::addLogo()
{
	GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,"../assets/images/logos/Bomb.png");
}

void SkillBomb::addBlackLogo()
{
	GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,"../assets/images/logos/Bomb_black.png");
}

void SkillBomb::update()
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

void SkillBomb::activate()
{
	active = true;
	addBlackLogo();
	deselect();
	
	if(GameManager::getInstance()->getNetworkEngine()->isStarted())
	{
		GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_SKILL_BOMB,GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
	}
	else
	{
		GameManager::getInstance()->getGameObjectManager()->createSkillBomb(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
		GameManager::getInstance()->getGameObjectManager()->animationFire(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
	}
	
	timeSinceActivated.start();
}

void SkillBomb::select()
{
	if(!isOnCooldown())
	{
		int color = DebugColor::RED;
		selected = true;
		nodeMesh = GameManager::getInstance()->getGraphicsEngine()->addNodeMesh("../assets/models/others/target/target_zone_red.obj",true);
		nodeMesh->setPosition(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
		nodeMesh->setScale(Vector3d(radius*2,1,radius*2));
	}
}

void SkillBomb::deselect()
{
	delete nodeMesh;
	nodeMesh = NULL;
	selected = false;
}
