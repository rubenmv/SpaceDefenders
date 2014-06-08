#include "SkillMultipleShot.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
SkillMultipleShot::SkillMultipleShot(void)
{
	selected = false;
	delay = 2000;
	levelRequired = 4;
	price = 0;
	active = true;
}

void SkillMultipleShot::addLogo()
{
	GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_TRIPLE);
}

void SkillMultipleShot::addBlackLogo()
{
	GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_TRIPLE_BLACK);
}

void SkillMultipleShot::addDescription()
{
	std::wstring description = L"Multiple Shot";
	description.append(L" Delay: ");
	description.append(std::to_wstring(delay/1000));
	description.append(L" \nLevel: ");
	description.append(std::to_wstring((int)levelRequired));
	description.append(L" Cost: ");
	description.append(std::to_wstring((int)price));

	GameManager::getInstance()->getGraphicsEngine()->addDescriptionText(skillNumber,description);
}

SkillMultipleShot::~SkillMultipleShot(void)
{

}

void SkillMultipleShot::update()
{
	if(!isOnCooldown() && active)
	{
		active = false;
		addLogo();
	}
}

void SkillMultipleShot::activate()
{
	active = true;
	addBlackLogo();
	Message message;
	message.type = Message::NEW_TARGET;
	GameObject* projectileOrBullet = NULL;
	message.gameObject = NULL; 
	int nProjectiles = 15;

	for(int i=0;i<nProjectiles*2;i++)
	{
		projectileOrBullet = GameManager::getInstance()->getGameObjectManager()->createBullet(0);

		Vector2d position(1,0);
		position.rotateBy(0+i*360/nProjectiles,Vector2d(0,0));

		projectileOrBullet->rotation = position.getAngle();
		projectileOrBullet->position = parent->position + position;
		projectileOrBullet->velocity = position/2;
		message.target = parent->position + position*5000;
		projectileOrBullet->broadcastMessage(message);
	}
}

void SkillMultipleShot::select()
{
	if(!isOnCooldown())
	{
		if(GameManager::getInstance()->getNetworkEngine()->isStarted())
		{
			GameManager::getInstance()->getEventManager()->mouseState.leftButtonDown = false;
			GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_SKILL_MULTIPLE_SHOT,GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround(),parent->position);
		}
		else
		{
			GameManager::getInstance()->getEventManager()->mouseState.leftButtonDown = false;
			activate();
		}
		timeSinceActivated.start();
	}
}

void SkillMultipleShot::deselect()
{

}
