#include "SkillTargetProjectile.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "CollisionManager.h"
SkillTargetProjectile::SkillTargetProjectile(void)
{
	selected = false;
	nodeMesh = NULL;
	delay = 2000;
	levelRequired = 3;
	price = 0;
	projectile = NULL;
	active = true;
}

void SkillTargetProjectile::addLogo()
{
	GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_MISSILE);
}

void SkillTargetProjectile::addBlackLogo()
{
	GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,GUI::HUD_IMAGE_ICON_MISSILE_BLACK);
}

void SkillTargetProjectile::addDescription()
{
	std::wstring description = L"Target Projectile";
	description.append(L" Delay: ");
	description.append(std::to_wstring(delay/1000));
	description.append(L" \nLevel: ");
	description.append(std::to_wstring((int)levelRequired));
	description.append(L" Cost: ");
	description.append(std::to_wstring((int)price));

	GameManager::getInstance()->getGraphicsEngine()->addDescriptionText(skillNumber,description);
}

SkillTargetProjectile::~SkillTargetProjectile(void)
{
	if(nodeMesh != NULL)
	{
		delete nodeMesh;
		nodeMesh = NULL;
	}

	projectile = NULL;
}

void SkillTargetProjectile::update()
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
		GameObject* objective = GameManager::getInstance()->getCollisionManager()->getGameObjectIn(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
		if(objective != NULL)
		{
			Vector3d position = objective->position.asVector3d();
			position.y = -1;
			nodeMesh->setPosition(position);
		}
		else
		{
			nodeMesh->setPosition(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
		}
	}
}

void SkillTargetProjectile::activate()
{
	active = true;
	addBlackLogo();
	GameManager::getInstance()->getEventManager()->mouseState.leftButtonDown = false;
	if(GameManager::getInstance()->getNetworkEngine()->isStarted())
	{
		GameObject* gameObject = GameManager::getInstance()->getCollisionManager()->getGameObjectIn(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
		if(gameObject != NULL)
		{
			Vector2d target(gameObject->GetNetworkID(),gameObject->GetNetworkID());
			GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_SKILL_TARGET_PROJECTILE,target,parent->position,gameObject->GetNetworkID());
		}
		else
		{
			Vector2d target = GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround();
			GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_SKILL_TARGET_PROJECTILE,target,parent->position,-1);
		}
	}
	else
	{
		Message message;
		message.type = Message::NEW_TARGET;
		message.gameObject = GameManager::getInstance()->getCollisionManager()->getGameObjectIn(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());

		projectile = GameManager::getInstance()->getGameObjectManager()->createTargetProjectile(damage);
	
		if(message.gameObject == NULL)
		{
			message.target = parent->position + Vector2d::getVector2dByAngle(-parent->rotation)*5000;
		}

		projectile->rotation = parent->rotation;
		projectile->position = parent->position + Vector2d::getVector2dByAngle(-parent->rotation)*7;
		projectile->velocity = parent->velocity;

		projectile->broadcastMessage(message);
	}

	deselect();
	timeSinceActivated.start();
}

void SkillTargetProjectile::select()
{
	if(!isOnCooldown())
	{
		selected = true;
		nodeMesh = GameManager::getInstance()->getGraphicsEngine()->addNodeMesh("../assets/models/others/target/target_red.obj",true);
		nodeMesh->setPosition(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
		nodeMesh->setScale(Vector3d(15,1,15));
	}
}

void SkillTargetProjectile::deselect()
{
	delete nodeMesh;
	nodeMesh = NULL;
	selected = false;
}
