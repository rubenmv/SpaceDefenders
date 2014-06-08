#include "SkillTargetHeal.h"
#include "GameManager.h"
#include "EventManager.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "CollisionManager.h"
SkillTargetHeal::SkillTargetHeal(void)
{
	selected = false;
	nodeMesh = NULL;
	delay = 5000;
	levelRequired = 1;
	price = 0;
	active = true;
}

void SkillTargetHeal::addLogo()
{
	GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,"../assets/images/logos/heal.png");
}

void SkillTargetHeal::addBlackLogo()
{
	GameManager::getInstance()->getGraphicsEngine()->addInventoryLogo(skillNumber,"../assets/images/logos/heal_black.png");
}

void SkillTargetHeal::addDescription()
{
	std::wstring description = L"Target Heal";
	description.append(L" Delay: ");
	description.append(std::to_wstring(delay/1000));
	description.append(L" \nLevel: ");
	description.append(std::to_wstring((int)levelRequired));
	description.append(L" Cost: ");
	description.append(std::to_wstring((int)price));

	GameManager::getInstance()->getGraphicsEngine()->addDescriptionText(skillNumber,description);
}

SkillTargetHeal::~SkillTargetHeal(void)
{
	if(nodeMesh != NULL)
	{
		delete nodeMesh;
		nodeMesh = NULL;
	}
}

void SkillTargetHeal::update()
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

void SkillTargetHeal::activate()
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
			GameManager::getInstance()->getNetworkEngine()->messageCreateSkill(GameObject::N_SKILL_TARGET_HEAL,target,target,gameObject->GetNetworkID());
		}
	}
	else
	{
		GameObject* gameObject = GameManager::getInstance()->getCollisionManager()->getGameObjectIn(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
		
		if(gameObject != NULL)
		{
			Message message;
			message.type = Message::INCOMING_DAMAGE;
			message.value = -100;

			gameObject->broadcastMessage(message);
		}
	}

	deselect();
	timeSinceActivated.start();
}

void SkillTargetHeal::select()
{
	if(!isOnCooldown())
	{
		selected = true;
		nodeMesh = GameManager::getInstance()->getGraphicsEngine()->addNodeMesh("../assets/models/others/target/target_green.obj",true);
		nodeMesh->setPosition(GameManager::getInstance()->getGraphicsEngine()->getMousePositionOnGround());
		nodeMesh->setScale(Vector3d(15,1,15));
	}
}

void SkillTargetHeal::deselect()
{
	delete nodeMesh;
	nodeMesh = NULL;
	selected = false;
}
