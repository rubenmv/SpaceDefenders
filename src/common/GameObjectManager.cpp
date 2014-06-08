#include "GameObjectManager.h"

#include <algorithm>

#include "GameObject.h"
#include "raknet/NetworkIDManager.h"
#include "ComponentCollider.h"
#include "ComponentHealth.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "MapManager.h"
#include "ComponentCamera.h"
#include "ComponentGraphicsShadow.h"
#include "ComponentDebris.h"
#include "ComponentInventory.h"
#include "ComponentAIBomber.h"
#include "ComponentGraphics.h"
#include "ComponentGraphicsShield.h"
#include "ComponentGraphicsStatics.h"
#include "ComponentRandomMovement.h"
#include "ComponentTargetPath.h"
#include "ComponentCreateDebris.h"
#include "ComponentJade.h"
#include "ComponentPathfinder.h"
#include "ComponentAIProjectile.h"
#include "ComponentTargetPosition.h"
#include "ComponentLevelExp.h"
#include "ComponentTargetAim.h"
#include "ComponentProjectileLauncher.h"
#include "ComponentBulletLauncher.h"
#include "ComponentAIMine.h"
#include "ComponentMinesGenerator.h"
#include "ComponentExplosionDamage.h"
#include "ComponentAIEnemy.h"
#include "ComponentHUD.h"
#include "ComponentSkill.h"
#include "ComponentAIFury.h"
#include "ComponentAIEnemyDumb.h"
#include "ComponentAITurret.h"
#include "ComponentVision.h"
#include "ComponentInputObstacle.h"
#include "ComponentWallConstruction.h"
#include "ComponentAutoDestroy.h"
#include "ComponentSteering.h"
#include "ComponentMovement.h"
#include "ComponentBehaviour.h"
#include "ComponentMiniMap.h"
#include "ComponentGiveExp.h"
#include "ComponentRay.h"
#include "ComponentEndGame.h"
#include "ComponentAIDefender.h"
#include "ComponentConstruction.h"
#include "ComponentEnergyWall.h"
#include "ComponentRemoteMessage.h"
#include "ComponentCreatingWall.h"
#include "ComponentSound.h"
// Inputs
#include "ComponentInput.h"
#include "ComponentInputCamera.h"
#include "ComponentInputTarget.h"

// Debug
#include "ComponentDebugGameObject.h"
#include "ComponentMovementInterpolation.h"
#include "ComponentDebugPathfinding.h"
#include "ComponentRepairSystem.h"
#include "Vector2d.h"

//Skills
#include "SkillTurret.h"
#include "SkillMine.h"
#include "SkillWall.h"
#include "SkillBomb.h"
#include "SkillTargetProjectile.h"
#include "SkillHeal.h"
#include "SkillMultipleShot.h"
#include "SkillTargetHeal.h"

//Animations
#include "ComponentAnimationArea.h"

GameObjectManager::GameObjectManager()
{
	gameManager = GameManager::getInstance();
	nextID = 1;
	//updateClockGOM = Clock();  //Inicializamos el reloj del update
	updateTime=-1;
}

GameObjectManager::~GameObjectManager()
{
	clear();
}

void GameObjectManager::update()
{
	updateClockGOM.start(); //Iniciamos el reloj del update
	if(!gameObjectList.empty())
	{
		gameObjectList.erase(std::remove_if(gameObjectList.begin(), gameObjectList.end(), GameObject::deleteIfReady), gameObjectList.end());
	}
	if(!tempGameObjectList.empty())
	{
		for (std::size_t i = 0; i < tempGameObjectList.size(); i++)
		{
			tempGameObjectList[i]->awake();
		}

		gameObjectList.insert(gameObjectList.end(),tempGameObjectList.begin(),tempGameObjectList.end());
		tempGameObjectList.clear();
	}

	for (std::size_t i = 0; i < gameObjectList.size(); i++)
	{
		gameObjectList[i]->update();
	}

	updateTime=updateClockGOM.getElapsedTime();//Guardamos el tiempo que ha tardado el update
}

void GameObjectManager::clear()
{
	updateTime=0;
	// El mismo objeto no puede estar en las dos listas al mismo tiempo
	while(!tempGameObjectList.empty())
	{
		delete tempGameObjectList.back();
		tempGameObjectList.pop_back();
	}

	while(!gameObjectList.empty())
	{
		delete gameObjectList.back();
		gameObjectList.pop_back();
	}

	mainPlayer = NULL;
}

GameObject* GameObjectManager::createPlayer(int character, std::vector<Skill*> skills)
{

	struct attributesPlayer	{
		float speed;
		float fireRate;
		float shield;
		float health; 
		float rechargeTime; 
		int character; 
		float maxSpeed; 
		int jade; 
		int collisionRadius;
		std::vector<Skill*> skills;

		const char* meshFilename;
		const char* shadowFilename;
		float shieldRadius;
		GameObject::Name name;

	};

	attributesPlayer attributes;
	std::vector<Component*> componentList;

	switch (character)
	{		
	case 1: //Bomber return createPlayer(5,25,200,300,300,3000,1,75,500,15,skills);
		
		attributes.speed = 5;
		attributes.fireRate = 150;
		attributes.shield = 300;
		attributes.health = 300; 
		attributes.rechargeTime = 3000; 
		attributes.character = 1; 
		attributes.maxSpeed = 75; 
		attributes.jade = 500; 
		attributes.collisionRadius = 15;
		attributes.skills = skills;
		attributes.meshFilename = Resources::MESH_PLAYER_BOMBER;
		attributes.shadowFilename = Resources::SPRITE_SHADOW_PLAYER_BOMBER;
		attributes.shieldRadius = 1.45;
		attributes.name = GameObject::N_BOMBER_PLAYER;

		componentList.push_back(new ComponentProjectileLauncher(attributes.fireRate,&ComponentProjectileLauncher::bomberBulletShoot)); 

		break;
	case 2: //Enginner  createPlayer(10,10,200,150,150,1000,2,125,500,10,skills);

		attributes.speed = 10;
		attributes.fireRate = 100;
		attributes.shield = 150;
		attributes.health = 150; 
		attributes.rechargeTime = 1000; 
		attributes.character = 2; 
		attributes.maxSpeed = 125; 
		attributes.jade = 500; 
		attributes.collisionRadius = 10;
		attributes.skills  = skills;
		attributes.meshFilename = Resources::MESH_PLAYER_ENGINEER;
		attributes.shadowFilename = Resources::SPRITE_SHADOW_PLAYER_ENGINEER;
		attributes.shieldRadius = 1;
		attributes.name = GameObject::N_ENGINEER_PLAYER;

		componentList.push_back(new ComponentProjectileLauncher(attributes.fireRate,&ComponentProjectileLauncher::engineerBulletShoot)); // El player dispara con 15 de daño
		break;
	case 3://Battleship createPlayer(5,15,200,300,300,3000,3,75,500,10,skills);
		
		attributes.speed = 5;
		attributes.fireRate = 125;
		attributes.shield = 300;
		attributes.health = 300; 
		attributes.rechargeTime = 3000; 
		attributes.character = 3; 
		attributes.maxSpeed = 75; 
		attributes.jade = 500; 
		attributes.collisionRadius = 10;
		attributes.skills = skills;
		attributes.meshFilename = Resources::MESH_PLAYER_BATTLESHIP;
		attributes.shadowFilename = Resources::SPRITE_SHADOW_PLAYER_BATTLESHIP;
		attributes.shieldRadius = 1.45;
		attributes.name = GameObject::N_BATTLESHIP_PLAYER;

		componentList.push_back(new ComponentProjectileLauncher(attributes.fireRate,&ComponentProjectileLauncher::battleshipBulletShoot)); // El player dispara con 15 de daño
		break;

	case 4: //Fighter	return createPlayer(10,20,200,150,150,3000,4,125,500,15,skills);

		attributes.speed = 10;
		attributes.fireRate = 75;
		attributes.shield = 150;
		attributes.health = 150; 
		attributes.rechargeTime = 3000; 
		attributes.character = 4; 
		attributes.maxSpeed = 125; 
		attributes.jade = 500; 
		attributes.collisionRadius = 15;
		attributes.skills  = skills;
		attributes.meshFilename = Resources::MESH_PLAYER_FIGHTER;
		attributes.shadowFilename = Resources::SPRITE_SHADOW_PLAYER_FIGHTER;
		attributes.shieldRadius = 1;
		attributes.name = GameObject::N_FIGHTER_PLAYER;

		componentList.push_back(new ComponentProjectileLauncher(attributes.fireRate,&ComponentProjectileLauncher::fighterBulletShoot)); // El player dispara con 15 de daño
		break;
	}


	componentList.push_back(new ComponentSkill(attributes.skills, attributes.jade));
	componentList.push_back(new ComponentDebugGameObject());
	componentList.push_back(new ComponentInput(attributes.speed));
	componentList.push_back(new ComponentCollider(4));	
	componentList.push_back(new ComponentVision(GameConstants::PLAYER_VISION_RADIUS));
	componentList.push_back(new ComponentRemoteMessage(Message::DEATH));
	componentList.push_back(new ComponentRemoteMessage(Message::REPAIRED));
	componentList.push_back(new ComponentLevelExp(	GameConstants::LEVEL_DEFAULT,
		GameConstants::EXP_DEFAULT,
		GameConstants::MOD_HP,
		GameConstants::MOD_HP));
	//componentList.push_back(new ComponentProjectileLauncher(damage,fireRate,&ComponentProjectileLauncher::normalBulletShoot)); // El player dispara con 15 de daño
	componentList.push_back(new ComponentHealth(attributes.health,attributes.shield, attributes.rechargeTime));
	componentList.push_back(new ComponentMiniMap(Resources::SPRITE_TRIANGLE, DebugColor::GREEN));
	componentList.push_back(new ComponentCamera(Vector3d(0,75,-42)));
	componentList.push_back(new ComponentHUD(attributes.jade,Vector2d(attributes.health,attributes.health),
	   Vector2d(attributes.shield,attributes.shield), 0, GameConstants::LEVEL_DEFAULT, GameConstants::EXP_DEFAULT, attributes.health));
	//componentList.push_back(new ComponentSound(attributes.fireRate, Resources::SOUND_PLAYER_PROJECTILE));

	componentList.push_back(new ComponentGraphics(attributes.meshFilename));
	componentList.push_back(new ComponentGraphicsShadow(attributes.shadowFilename,true));
	componentList.push_back(new ComponentGraphicsShield(false,attributes.shieldRadius));
	//componentList.push_back(new ComponentProjectileLauncher(damage,fireRate,&ComponentProjectileLauncher::bomberBulletShoot)); // El player dispara con 15 de daño

	componentList.push_back(new ComponentMovement(attributes.maxSpeed));

	GameObject* gameObject = NULL;
	gameObject = new GameObject(componentList, GameObject::MAIN_PLAYER, attributes.name, getNextID(), false);
	
	tempGameObjectList.push_back(gameObject);
	mainPlayer = gameObject;
	return gameObject;
}

GameObject* GameObjectManager::createMiniHangar( Vector2d position )
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_BUILDING_MINI_HANGAR));
		//componentList.push_back(new ComponentGraphicsShield(true,4));
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentCreateDebris(20));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));

	}
	componentList.push_back(new ComponentEndGame());
	componentList.push_back(new ComponentCollider(GameConstants::BASE_COLLISION_RADIUS));
	componentList.push_back(new ComponentHealth(GameConstants::BASE_HEALTH, GameConstants::BASE_SHIELD, GameConstants::BASE_RECHARGE_TIME));

	GameObject* gameObject = NULL;
	gameObject = new GameObject(componentList, GameObject::Tag::BASE, GameObject::N_MINI_HANGAR, getNextID(), false, true);
	gameObject->position = position + Vector2d(13,-23);
	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createMiniBase( Vector2d position )
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_BUILDING_MINI_BASE));
		//componentList.push_back(new ComponentGraphicsShield(true,3));
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentCreateDebris(20));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));

	}
	componentList.push_back(new ComponentEndGame());
	componentList.push_back(new ComponentCollider(GameConstants::BASE_COLLISION_RADIUS));
	componentList.push_back(new ComponentHealth(GameConstants::BASE_HEALTH, GameConstants::BASE_SHIELD, GameConstants::BASE_RECHARGE_TIME));

	GameObject* gameObject = NULL;
	gameObject = new GameObject(componentList, GameObject::Tag::BASE, GameObject::N_MINI_BASE, getNextID(), false, true);
	//gameObject->position = Vector2d(150,-780);
	gameObject->position = position + Vector2d(6,-56);
	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createBase( Vector2d position )
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_BUILDING_BASE));
		//componentList.push_back(new ComponentGraphicsShield(true,5));
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentCreateDebris(30));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));

	}
	componentList.push_back(new ComponentEndGame());
	componentList.push_back(new ComponentCollider(GameConstants::BASE_COLLISION_RADIUS));
	componentList.push_back(new ComponentHealth(GameConstants::BASE_HEALTH, GameConstants::BASE_SHIELD, GameConstants::BASE_RECHARGE_TIME));

	GameObject* gameObject = NULL;
	gameObject = new GameObject(componentList, GameObject::Tag::BASE, GameObject::N_BASE, getNextID(), false, true);
	//gameObject->position = Vector2d(120,-765);
	gameObject->position = position+Vector2d(-2,43);
	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createHangar( Vector2d position )
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_BUILDING_HANGAR));
		//	componentList.push_back(new ComponentGraphicsShield(true,3));
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentCreateDebris(30));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));

	}
	componentList.push_back(new ComponentEndGame());
	componentList.push_back(new ComponentCollider(GameConstants::BASE_COLLISION_RADIUS));
	componentList.push_back(new ComponentHealth(GameConstants::BASE_HEALTH, GameConstants::BASE_SHIELD, GameConstants::BASE_RECHARGE_TIME));

	GameObject* gameObject = NULL;
	gameObject = new GameObject(componentList, GameObject::Tag::BASE, GameObject::N_HANGAR, getNextID(), false, true);
	gameObject->position = position + Vector2d(36,9);
	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createExtractors( Vector2d position )
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_BUILDING_EXTRACTOR));
		//componentList.push_back(new ComponentGraphicsShield(true,6));
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentCreateDebris(30));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));

	}
	componentList.push_back(new ComponentEndGame());
	componentList.push_back(new ComponentCollider(GameConstants::BASE_COLLISION_RADIUS));
	componentList.push_back(new ComponentHealth(GameConstants::BASE_HEALTH, GameConstants::BASE_SHIELD, GameConstants::BASE_RECHARGE_TIME));

	GameObject* gameObject = NULL;
	gameObject = new GameObject(componentList, GameObject::Tag::BASE, GameObject::N_EXTRACTORS, getNextID(), false, true);
	gameObject->position = position + Vector2d(-41,-1);
	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createRemoteMiniHangar()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_BUILDING_MINI_HANGAR));
		componentList.push_back(new ComponentCreateDebris(20));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentMovementInterpolation());
	}
	componentList.push_back(new ComponentCollider(GameConstants::BASE_COLLISION_RADIUS));

	GameObject* gameObject = NULL;
	gameObject = new GameObject(componentList, GameObject::Tag::REMOTE, GameObject::N_MINI_HANGAR, getNextID(), false,true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createRemoteMiniBase()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_BUILDING_MINI_BASE));
		componentList.push_back(new ComponentCreateDebris(20));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentMovementInterpolation());
	}

	componentList.push_back(new ComponentCollider(GameConstants::BASE_COLLISION_RADIUS));


	GameObject* gameObject = NULL;
	gameObject = new GameObject(componentList, GameObject::Tag::REMOTE, GameObject::N_MINI_BASE, getNextID(), false,true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createRemoteBase()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_BUILDING_BASE));
		componentList.push_back(new ComponentCreateDebris(30));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentMovementInterpolation());
	}

	componentList.push_back(new ComponentCollider(GameConstants::BASE_COLLISION_RADIUS));

	GameObject* gameObject = NULL;
	gameObject = new GameObject(componentList, GameObject::Tag::REMOTE, GameObject::N_BASE, getNextID(), false,true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createRemoteHangar()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_BUILDING_HANGAR));
		componentList.push_back(new ComponentCreateDebris(30));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentMovementInterpolation());
	}

	componentList.push_back(new ComponentCollider(GameConstants::BASE_COLLISION_RADIUS));

	GameObject* gameObject = NULL;
	gameObject = new GameObject(componentList, GameObject::Tag::REMOTE, GameObject::N_HANGAR, getNextID(), false,true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createRemoteExtractors()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_BUILDING_EXTRACTOR));
		componentList.push_back(new ComponentCreateDebris(30));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentMovementInterpolation());
	}

	componentList.push_back(new ComponentCollider(GameConstants::BASE_COLLISION_RADIUS));


	GameObject* gameObject = NULL;
	gameObject = new GameObject(componentList, GameObject::Tag::REMOTE, GameObject::N_EXTRACTORS, getNextID(), false,true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

void GameObjectManager::createWholeBase( Vector2d position )
{
	createBase(position);
	createMiniBase(position);
	createMiniHangar(position);
	createExtractors(position);
	createHangar(position);
}

GameObject* GameObjectManager::createWall()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_ENERGY_WALL));
		componentList.push_back(new ComponentMiniMap("../assets/images/minimap/point.png", DebugColor::GREEN));
	}
	componentList.push_back(new ComponentCollider(10));
	//componentList.push_back(new ComponentLevelExp(GameConstants::LEVEL_DEFAULT, GameConstants::EXP_DEFAULT));
	componentList.push_back(new ComponentHealth(200,0,0));

	GameObject* gameObject = new GameObject(componentList, GameObject::Tag::WALL, GameObject::N_WALL, getNextID(), false, true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createEnergyWall(Vector2d start, Vector2d end)
{
	std::vector<Component*> componentList;
	componentList.push_back(new ComponentEnergyWall(start,end));
	GameObject* gameObject = new GameObject(componentList, GameObject::Tag::WALL, GameObject::N_ENERGY_WALL, getNextID(), false, true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createPlayer()
{
	std::vector<Component*> componentList;
	componentList.push_back(new ComponentDebugGameObject());
	componentList.push_back(new ComponentInput(10));
	componentList.push_back(new ComponentCollider(6));
	componentList.push_back(new ComponentVision(40));
	componentList.push_back(new ComponentProjectileLauncher(100,&ComponentProjectileLauncher::multipleBulletShoot)); // El player dispara con 15 de daño
	componentList.push_back(new ComponentHealth(100,100, 1));
	componentList.push_back(new ComponentCamera(Vector3d(30,75,-30)));
	componentList.push_back(new ComponentGraphics(Resources::MESH_PLAYER_DEFAULT));
	componentList.push_back(new ComponentMovement(100));

	GameObject* gameObject = new GameObject(componentList, GameObject::Tag::MAIN_PLAYER, GameObject::N_MAIN_PLAYER, getNextID(), false);

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createBomber()
{
	std::vector<Skill*> skills;

	
	skills.push_back(new SkillTargetHeal());
	skills.push_back(new SkillBomb());
	skills.push_back(new SkillTargetProjectile());
	skills.push_back(new SkillMultipleShot());
	skills.push_back(new SkillTurret(GameObject::N_TURRET_BULLET));	
	skills.push_back(new SkillTurret(GameObject::N_TURRET_PROJECTILE));
	skills.push_back(new SkillTurret(GameObject::N_TURRET_BIG_CANON));
	skills.push_back(new SkillTurret(GameObject::N_TURRET_1_CANON));



	for(size_t i=0;i<skills.size();i++)
	{
		skills[i]->setSkillNumber(i);
	}
	return createPlayer(1,skills);
}

GameObject* GameObjectManager::createEngineer()
{
	std::vector<Skill*> skills;
	skills.push_back(new SkillTargetHeal());
	skills.push_back(new SkillHeal());
	skills.push_back(new SkillMultipleShot());
	skills.push_back(new SkillTargetProjectile());
	skills.push_back(new SkillTurret(GameObject::N_TURRET_BULLET));
	skills.push_back(new SkillTurret(GameObject::N_TURRET_1_CANON));
	skills.push_back(new SkillMine(GameObject::N_MINE));
	skills.push_back(new SkillMine(GameObject::N_MINE2));

	
	for(size_t i=0;i<skills.size();i++)
	{
		skills[i]->setSkillNumber(i);
	}
	return createPlayer(2,skills);
}

GameObject* GameObjectManager::createBattleship()
{
	std::vector<Skill*> skills;
	skills.push_back(new SkillTargetHeal());
	skills.push_back(new SkillTargetProjectile());
	skills.push_back(new SkillTurret(GameObject::N_TURRET_BULLET));
	skills.push_back(new SkillTurret(GameObject::N_TURRET_PROJECTILE));
	skills.push_back(new SkillTurret(GameObject::N_TURRET_BIG_CANON));
	skills.push_back(new SkillMine(GameObject::N_MINE));
	skills.push_back(new SkillMine(GameObject::N_MINE2));
	skills.push_back(new SkillMine(GameObject::N_MINE3));

	for(size_t i=0;i<skills.size();i++)
	{
		skills[i]->setSkillNumber(i);
	}
	return createPlayer(3,skills);
}

GameObject* GameObjectManager::createFighter()
{
	std::vector<Skill*> skills;

	skills.push_back(new SkillTargetHeal());
	skills.push_back(new SkillMultipleShot);
	skills.push_back(new SkillTargetProjectile);
	skills.push_back(new SkillTurret(GameObject::N_TURRET_BULLET));
	skills.push_back(new SkillMine(GameObject::N_MINE));
	skills.push_back(new SkillMine(GameObject::N_MINE2));
	skills.push_back(new SkillMine(GameObject::N_MINE3));
	skills.push_back(new SkillTurret(GameObject::N_TURRET_1_CANON));

	
	for(size_t i=0;i<skills.size();i++)
	{
		skills[i]->setSkillNumber(i);
	}
	//return createPlayer(10,20,100,150,100,3000,4,125,150,15);
	return createPlayer(4,skills);
}

GameObject* GameObjectManager::createRemotePlayer()
{
	std::vector<Component*> componentList;

	// Jugador fantasma para mostrar en los clientes
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_PLAYER_DEFAULT));

		componentList.push_back(new ComponentMovementInterpolation());
		componentList.push_back(new ComponentCollider(4));
	}

	GameObject* gameObject = new GameObject(componentList, GameObject::Tag::REMOTE, GameObject::N_MAIN_PLAYER, getNextID(), false);
	std::cout<<gameObject->getName()<<std::endl;
	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createRemotePlayer(int character)
{
	std::vector<Component*> componentList;
	if(gameManager->isServer())
	{
		componentList.push_back(new ComponentRemoteMessage(Message::INCOMING_DAMAGE));
	}
	// Jugador fantasma para mostrar en los clientes
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		switch (character)
		{
		case 1:
			componentList.push_back(new ComponentGraphics(Resources::MESH_PLAYER_BOMBER));
			componentList.push_back(new ComponentGraphicsShadow(Resources::SPRITE_SHADOW_PLAYER_BOMBER,true));
			componentList.push_back(new ComponentGraphicsShield(false,1.2));
			break;
		case 2:
			componentList.push_back(new ComponentGraphics(Resources::MESH_PLAYER_BATTLESHIP));
			componentList.push_back(new ComponentGraphicsShadow(Resources::SPRITE_SHADOW_PLAYER_BATTLESHIP,true));
			componentList.push_back(new ComponentGraphicsShield(false,1.2));
			break;
		case 3:
			componentList.push_back(new ComponentGraphics(Resources::MESH_PLAYER_ENGINEER));
			componentList.push_back(new ComponentGraphicsShadow(Resources::SPRITE_SHADOW_PLAYER_ENGINEER,true));
			componentList.push_back(new ComponentGraphicsShield(false,1));
			break;
		case 4:
			componentList.push_back(new ComponentGraphics(Resources::MESH_PLAYER_FIGHTER));
			componentList.push_back(new ComponentGraphicsShadow(Resources::SPRITE_SHADOW_PLAYER_FIGHTER,true));
			componentList.push_back(new ComponentGraphicsShield(false,1));
			break;
		}
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_TRIANGLE, DebugColor::BLUE));

		componentList.push_back(new ComponentMovementInterpolation());

	}
	componentList.push_back(new ComponentCollider(4));

	GameObject* gameObject = NULL; 

	switch (character)
	{
	case 1:
		gameObject = new GameObject(componentList, GameObject::Tag::MAIN_PLAYER, GameObject::N_BOMBER_PLAYER, getNextID(), false);
		break;
	case 2:
		gameObject = new GameObject(componentList, GameObject::Tag::MAIN_PLAYER, GameObject::N_BATTLESHIP_PLAYER, getNextID(), false);
		break;
	case 3:
		gameObject = new GameObject(componentList, GameObject::Tag::MAIN_PLAYER, GameObject::N_ENGINEER_PLAYER, getNextID(), false);
		break;
	case 4:
		gameObject = new GameObject(componentList, GameObject::Tag::MAIN_PLAYER, GameObject::N_FIGHTER_PLAYER, getNextID(), false);
		break;
	}

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createRemoteBomber()
{
	return createRemotePlayer(1);
}

GameObject* GameObjectManager::createRemoteBattleship()
{
	return createRemotePlayer(2);
}

GameObject* GameObjectManager::createRemoteEngineer()
{
	return createRemotePlayer(3);
}

GameObject* GameObjectManager::createRemoteFighter()
{
	return createRemotePlayer(4);
}

GameObject* GameObjectManager::createTurret()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_TURRET_2_TOP));
		componentList.push_back(new ComponentGraphicsStatics(Resources::MESH_INVENTORY_TURRET_2_BOTTOM));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentGraphicsShield(true,1.55));

	}
	componentList.push_back(new ComponentCollider(8));
	componentList.push_back(new ComponentVision(100));
	componentList.push_back(new ComponentAITurret());
	componentList.push_back(new ComponentTargetAim());
	componentList.push_back(new ComponentProjectileLauncher(400,&ComponentProjectileLauncher::turretShoot1));
	componentList.push_back(new ComponentHealth(GameConstants::TURRET_HEALTH, GameConstants::TURRET_SHIELD, GameConstants::TURRET_RECHARGE_TIME));

	GameObject* gameObject = new GameObject(componentList, GameObject::TURRET, GameObject::N_TURRET_PROJECTILE, getNextID(),false, true);

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createTurret2()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_TURRET_TOP));
		componentList.push_back(new ComponentGraphicsStatics("../assets/models/inventory/turrets/turret_basic/turret_basic_bot.obj"));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentGraphicsShield(true,1.25));

	}
	componentList.push_back(new ComponentCollider(6));
	componentList.push_back(new ComponentVision(100));
	componentList.push_back(new ComponentAITurret());
	componentList.push_back(new ComponentTargetAim());
	componentList.push_back(new ComponentProjectileLauncher(300,&ComponentProjectileLauncher::turretBulletShoot));
	componentList.push_back(new ComponentHealth(GameConstants::TURRET_HEALTH, GameConstants::TURRET_SHIELD, GameConstants::TURRET_RECHARGE_TIME));

	GameObject* gameObject = new GameObject(componentList, GameObject::TURRET, GameObject::N_TURRET_BULLET, getNextID(),false, true);

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createTurret2Canon()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_TURRET_2_CANON_TOP));
		componentList.push_back(new ComponentGraphicsStatics(Resources::MESH_INVENTORY_TURRET_2_CANON_BOTTOM));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentGraphicsShield(true,1.25));
	}
	componentList.push_back(new ComponentCollider(4));
	componentList.push_back(new ComponentVision(100));
	componentList.push_back(new ComponentAITurret());
	componentList.push_back(new ComponentTargetAim());
	componentList.push_back(new ComponentProjectileLauncher(200,&ComponentProjectileLauncher::normalBulletShoot));
	componentList.push_back(new ComponentHealth(GameConstants::TURRET_HEALTH, GameConstants::TURRET_SHIELD, GameConstants::TURRET_RECHARGE_TIME));

	GameObject* gameObject = new GameObject(componentList, GameObject::TURRET, GameObject::N_TURRET_2_CANON, getNextID(),false, true);

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createTurretBigCanon()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_TURRET_BIG_CANON_TOP));
		componentList.push_back(new ComponentGraphicsStatics(Resources::MESH_INVENTORY_TURRET_BIG_CANON_BOTTOM));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentGraphicsShield(true,1.3));

	}
	componentList.push_back(new ComponentCollider(4));
	componentList.push_back(new ComponentVision(100));
	componentList.push_back(new ComponentAITurret());
	componentList.push_back(new ComponentTargetAim());
	componentList.push_back(new ComponentProjectileLauncher(800,&ComponentProjectileLauncher::multipleShoot));
	componentList.push_back(new ComponentHealth(GameConstants::TURRET_HEALTH, GameConstants::TURRET_SHIELD, GameConstants::TURRET_RECHARGE_TIME));

	GameObject* gameObject = new GameObject(componentList, GameObject::TURRET, GameObject::N_TURRET_BIG_CANON, getNextID(),false, true);

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}


GameObject* GameObjectManager::createTurret1Canon()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_TURRET_1_CANON_TOP));
		componentList.push_back(new ComponentGraphicsStatics(Resources::MESH_INVENTORY_TURRET_1_CANON_BOTTOM));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentGraphicsShield(true,1.2));

	}
	componentList.push_back(new ComponentCollider(4));
	componentList.push_back(new ComponentVision(100));
	componentList.push_back(new ComponentAITurret());
	componentList.push_back(new ComponentTargetAim());
	componentList.push_back(new ComponentProjectileLauncher(500,&ComponentProjectileLauncher::multipleBulletShoot));
	componentList.push_back(new ComponentHealth(GameConstants::TURRET_HEALTH, GameConstants::TURRET_SHIELD, GameConstants::TURRET_RECHARGE_TIME));

	GameObject* gameObject = new GameObject(componentList, GameObject::TURRET, GameObject::N_TURRET_1_CANON, getNextID(),false, true);

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}


GameObject* GameObjectManager::createTurretGun()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_TURRET_GUN_TOP));
		componentList.push_back(new ComponentGraphicsStatics(Resources::MESH_INVENTORY_TURRET_GUN_BOTTOM));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentGraphicsShield(true,1.1));

	}
	componentList.push_back(new ComponentCollider(4));
	componentList.push_back(new ComponentVision(100));
	componentList.push_back(new ComponentAITurret());
	componentList.push_back(new ComponentTargetAim());
	componentList.push_back(new ComponentProjectileLauncher(200,&ComponentProjectileLauncher::normalBulletShoot));
	componentList.push_back(new ComponentHealth(GameConstants::TURRET_HEALTH, GameConstants::TURRET_SHIELD, GameConstants::TURRET_RECHARGE_TIME));

	GameObject* gameObject = new GameObject(componentList, GameObject::TURRET, GameObject::N_TURRET_GUN, getNextID(),false, true);

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}


GameObject* GameObjectManager::createPlayerDefender()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics("../assets/models/players/defender/defender.obj"));
	}
	componentList.push_back(new ComponentVision(100));
	componentList.push_back(new ComponentAITurret());
	componentList.push_back(new ComponentTargetAim());
	componentList.push_back(new ComponentProjectileLauncher(200,&ComponentProjectileLauncher::normalBulletShoot));

	GameObject* gameObject = new GameObject(componentList, GameObject::TURRET, GameObject::N_TURRET_BULLET, getNextID(),false);

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}


GameObject* GameObjectManager::createInventoryElement(const char* modelPath,  int objectName,unsigned int price, bool wall, int tag, const char* modelPathStatic)
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());

		if(!wall)
		{
			componentList.push_back(new ComponentInventory(modelPath,price,modelPathStatic));
		}
		else
		{
			componentList.push_back(new ComponentWallConstruction(modelPath,price));
		}
	}
	//componentList.push_back(new ComponentCollider(4));

	componentList.push_back(new ComponentVision(30));
	GameObject* gameObject = new GameObject(componentList, (GameObject::Tag)tag, (GameObject::Name)objectName, getNextID(),false, true);

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createInventoryTurret(int price)
{
	return createInventoryElement(Resources::MESH_INVENTORY_TURRET_2_TOP,GameObject::N_INVENTORY_TURRET_PROJECTILE,price,false,GameObject::INVENTORY,Resources::MESH_INVENTORY_TURRET_2_BOTTOM);
}

GameObject* GameObjectManager::createInventoryTurret2(int price)
{
	return createInventoryElement(Resources::MESH_INVENTORY_TURRET_TOP,GameObject::N_INVENTORY_TURRET_BULLET,price,false,GameObject::INVENTORY,"../assets/models/inventory/turrets/turret_basic/turret_basic_bot.obj");
}

GameObject* GameObjectManager::createInventoryTurret2Canon(int price)
{
	return createInventoryElement(Resources::MESH_INVENTORY_TURRET_2_CANON_TOP,GameObject::N_INVENTORY_TURRET_2_CANON,price,false,GameObject::INVENTORY,Resources::MESH_INVENTORY_TURRET_2_CANON_BOTTOM);
}

GameObject* GameObjectManager::createInventoryTurretBigCanon(int price)
{
	return createInventoryElement(Resources::MESH_INVENTORY_TURRET_BIG_CANON_TOP,GameObject::N_INVENTORY_TURRET_BIG_CANON,price,false,GameObject::INVENTORY,Resources::MESH_INVENTORY_TURRET_BIG_CANON_BOTTOM);
}

GameObject* GameObjectManager::createInventoryTurret1Canon(int price)
{
	return createInventoryElement(Resources::MESH_INVENTORY_TURRET_1_CANON_TOP,GameObject::N_INVENTORY_TURRET_1_CANON,price,false,GameObject::INVENTORY,Resources::MESH_INVENTORY_TURRET_1_CANON_BOTTOM);
}

GameObject* GameObjectManager::createInventoryTurretGun(int price)
{
	return createInventoryElement(Resources::MESH_INVENTORY_TURRET_GUN_TOP,GameObject::N_INVENTORY_TURRET_GUN,price,false,GameObject::INVENTORY,Resources::MESH_INVENTORY_TURRET_GUN_BOTTOM);
}

GameObject* GameObjectManager::createInventoryMine(int price)
{
	return createInventoryElement(Resources::MESH_INVENTORY_MINE,GameObject::N_INVENTORY_MINE,price,false,GameObject::INVENTORY);
}

GameObject* GameObjectManager::createInventoryMine2(int price)
{
	return createInventoryElement(Resources::MESH_INVENTORY_MINE_2,GameObject::N_MINE2,price,false,GameObject::INVENTORY);
}

GameObject* GameObjectManager::createInventoryMine3(int price)
{
	return createInventoryElement(Resources::MESH_INVENTORY_MINE_3,GameObject::N_MINE3,price,false,GameObject::INVENTORY);
}

GameObject* GameObjectManager::createInventoryWall(int price)
{
	return createInventoryElement(Resources::MESH_INVENTORY_ENERGY_WALL,GameObject::N_INVENTORY_WALL,price,true,GameObject::INVENTORY);
}

GameObject* GameObjectManager::createBuildingTurret(Vector2d position)
{
	return createBuildingElement(Resources::MESH_INVENTORY_TURRET_2_TOP,GameObject::N_BUILDING_TURRET_PROJECTILE,GameObject::BUILDING_TURRET_PROJECTILE,position,Resources::MESH_INVENTORY_TURRET_2_BOTTOM);
}

GameObject* GameObjectManager::createBuildingTurret2(Vector2d position)
{
	return createBuildingElement(Resources::MESH_INVENTORY_TURRET_TOP, GameObject::N_BUILDING_TURRET_BULLET,GameObject::BUILDING_TURRET_BULLET,position,"../assets/models/inventory/turrets/turret_basic/turret_basic_bot.obj");
}

GameObject* GameObjectManager::createBuildingTurret2Canon(Vector2d position)
{
	return createBuildingElement(Resources::MESH_INVENTORY_TURRET_2_CANON_TOP, GameObject::N_BUILDING_TURRET_2_CANON,GameObject::BUILDING_TURRET_2_CANON,position,Resources::MESH_INVENTORY_TURRET_2_CANON_BOTTOM);
}

GameObject* GameObjectManager::createBuildingTurretBigCanon(Vector2d position)
{
	return createBuildingElement(Resources::MESH_INVENTORY_TURRET_BIG_CANON_TOP, GameObject::N_BUILDING_TURRET_BIG_CANON,GameObject::BUILDING_TURRET_BIG_CANON,position,Resources::MESH_INVENTORY_TURRET_BIG_CANON_BOTTOM);
}

GameObject* GameObjectManager::createBuildingTurret1Canon(Vector2d position)
{
	return createBuildingElement(Resources::MESH_INVENTORY_TURRET_1_CANON_TOP, GameObject::N_BUILDING_TURRET_1_CANON,GameObject::BUILDING_TURRET_1_CANON,position,Resources::MESH_INVENTORY_TURRET_1_CANON_BOTTOM);
}

GameObject* GameObjectManager::createBuildingTurretGun(Vector2d position)
{
	return createBuildingElement(Resources::MESH_INVENTORY_TURRET_GUN_TOP, GameObject::N_BUILDING_TURRET_GUN,GameObject::BUILDING_TURRET_GUN,position,Resources::MESH_INVENTORY_TURRET_GUN_BOTTOM);
}

GameObject* GameObjectManager::createBuildingMine(Vector2d position)
{
	return createBuildingElement(Resources::MESH_INVENTORY_MINE,GameObject::N_BUILDING_MINE,GameObject::BUILDING_MINE,position);
}

GameObject* GameObjectManager::createBuildingMine2(Vector2d position)
{
	return createBuildingElement(Resources::MESH_INVENTORY_MINE_2,GameObject::N_MINE2,GameObject::BUILDING_MINE2,position);
}

GameObject* GameObjectManager::createBuildingMine3(Vector2d position)
{
	return createBuildingElement(Resources::MESH_INVENTORY_MINE_3,GameObject::N_MINE3,GameObject::BUILDING_MINE3,position);
}

GameObject* GameObjectManager::createBuildingWall(Vector2d position)
{
	return createBuildingElement(Resources::MESH_INVENTORY_ENERGY_WALL,GameObject::N_BUILDING_WALL,GameObject::BUILDING_WALL,position);
}

GameObject* GameObjectManager::createBuildingEnergyWall(GameObject* creator1, GameObject* creator2)
{
	float distance = creator1->position.getDistanceFrom(creator2->position);
	Vector2d center = creator2->position+((creator1->position-creator2->position).normalize() * distance/2);

	std::vector<Component*> componentList;
	componentList.push_back(new ComponentCreatingWall(creator1,creator2));

	GameObject* gameObject = new GameObject(componentList, GameObject::BUILDING_ENERGY_WALL, GameObject::N_BUILDING_WALL, getNextID(),false,true,center);

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createBuildingElement(const char* modelPath,  int objectName, int tag, Vector2d position, const char* modelPathStatic)
{
	std::vector<Component*> componentList;
	//componentList.push_back(new ComponentDebugGameObject());

	componentList.push_back(new ComponentConstruction(modelPath,modelPathStatic));

	componentList.push_back(new ComponentCollider(4));
	componentList.push_back(new ComponentHealth(GameConstants::BUILDING_ELEMENT_HEALTH, GameConstants::BUILDING_ELEMENT_SHIELD, GameConstants::BUILDING_ELEMENT_RECHARGE_TIME));

	GameObject* gameObject = new GameObject(componentList, (GameObject::Tag)tag, (GameObject::Name)objectName, getNextID(),false,true,position);

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createMine()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_MINE));
		componentList.push_back(new ComponentMiniMap("../assets/images/minimap/x.png", DebugColor::GREEN));
	}
	componentList.push_back(new ComponentVision(10));
	componentList.push_back(new ComponentAIMine(100)); 
	GameObject* gameObject = new GameObject(componentList, GameObject::MINE, GameObject::N_MINE, getNextID(),false, true);
	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createMine2()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_MINE_2));
		componentList.push_back(new ComponentMiniMap("../assets/images/minimap/x.png", DebugColor::GREEN));
	}
	componentList.push_back(new ComponentVision(10));
	componentList.push_back(new ComponentAIMine(200)); 
	GameObject* gameObject = new GameObject(componentList, GameObject::MINE, GameObject::N_MINE2, getNextID(),false, true);
	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createMine3()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_MINE_3));
		componentList.push_back(new ComponentMiniMap("../assets/images/minimap/x.png", DebugColor::GREEN));
	}
	componentList.push_back(new ComponentVision(10));
	componentList.push_back(new ComponentAIMine(50)); 
	GameObject* gameObject = new GameObject(componentList, GameObject::MINE, GameObject::N_MINE3, getNextID(),false, true);
	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createExplosion(float damage)
{

	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentCreateDebris(8));
	}
	componentList.push_back(new ComponentVision(15));
	componentList.push_back(new ComponentExplosionDamage(damage));


	GameObject* gameObject = new GameObject(componentList, GameObject::EXPLOSION, GameObject::N_EXPLOSION, getNextID(),false, true);
	gameObject->rotation = 45;
	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createSpark()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics("../assets/models/animations/spark/spark.obj"));
		componentList.push_back(new ComponentAutoDestroy(GameConstants::SPARK_DESTROY_TIME));
	}

	GameObject* gameObject = new GameObject(componentList, GameObject::PARTICLES, GameObject::N_SPARK, getNextID(),false);

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createFire()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics("../assets/models/animations/fire/fire.obj"));
		componentList.push_back(new ComponentAutoDestroy(GameConstants::FIRE_DESTROY_TIME));
	}

	GameObject* gameObject = new GameObject(componentList, GameObject::PARTICLES, GameObject::N_SPARK, getNextID(),false);

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

void GameObjectManager::animationFire(Vector2d position)
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentAnimationArea(0,position));
	}

	GameObject* gameObject = new GameObject(componentList, GameObject::PARTICLES, GameObject::N_SPARK, getNextID(),false);

	tempGameObjectList.push_back(gameObject);

	gameObject->position = position;
}

void GameObjectManager::animationHeal(Vector2d position)
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentAnimationArea(1,position));
	}

	GameObject* gameObject = new GameObject(componentList, GameObject::PARTICLES, GameObject::N_SPARK, getNextID(),false);

	tempGameObjectList.push_back(gameObject);

	gameObject->position = position;
}

GameObject* GameObjectManager::createEnemyBasic()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_ENEMY_BASIC));
		componentList.push_back(new ComponentGraphicsShadow(Resources::SPRITE_SHADOW_ENEMY_BASIC,true));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::RED));
		componentList.push_back(new ComponentGraphicsShield(false,1));

		componentList.push_back(new ComponentCreateDebris(8));
	}

	componentList.push_back(new ComponentCollider(7));
	componentList.push_back(new ComponentVision(100));
	componentList.push_back(new ComponentBehaviour());
	componentList.push_back(new ComponentHealth(150,150, 1));
	componentList.push_back(new ComponentAIEnemy(60));
	componentList.push_back(new ComponentTargetAim());
	componentList.push_back(new ComponentProjectileLauncher(400,&ComponentProjectileLauncher::normalBulletShoot));
	componentList.push_back(new ComponentTargetPath(20));
	componentList.push_back(new ComponentMovement(40));
	componentList.push_back(new ComponentGiveExp(15));

	GameObject* gameObject = new GameObject(componentList, GameObject::ENEMY, GameObject::N_ENEMY_BASIC, getNextID(),true,false,Vector2d(832,-59+Math::random(3)));

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

void GameObjectManager::createBasicDebris(Vector2d velocity, Vector2d position)
{
	if(!GameManager::getInstance()->isServer())
	{
		velocity.x = 1;
		velocity.y = 1;

		Vector2d velocity1 = velocity.rotateBy(Math::random(20) + 45,Vector2d(0,0)).normalize();
		Vector2d velocity2 = velocity.rotateBy(Math::random(20) + 90,Vector2d(0,0)).normalize();
		Vector2d velocity3 = velocity.rotateBy(Math::random(20) + 135,Vector2d(0,0)).normalize();
		Vector2d velocity4 = velocity.rotateBy(Math::random(20) + 180,Vector2d(0,0)).normalize();
		Vector2d velocity5 = velocity.rotateBy(Math::random(20) + 225,Vector2d(0,0)).normalize();
		Vector2d velocity6 = velocity.rotateBy(Math::random(20) + 270,Vector2d(0,0)).normalize();
		Vector2d velocity7 = velocity.rotateBy(Math::random(20) + 314,Vector2d(0,0)).normalize();
		createGenericDebris(Resources::MESH_DEBRIS_1,velocity1*2,position);
		createGenericDebris(Resources::MESH_DEBRIS_2,velocity2*2,position);
		createGenericDebris(Resources::MESH_DEBRIS_3,velocity3*2,position);
		createGenericDebris(Resources::MESH_DEBRIS_4,velocity4*2,position);
		createGenericDebris(Resources::MESH_DEBRIS_5,velocity5*2,position);
		createGenericDebris(Resources::MESH_DEBRIS_1,velocity6*2,position);
		createGenericDebris(Resources::MESH_DEBRIS_2,velocity7*2,position);
		createGenericDebris(Resources::MESH_DEBRIS_3,velocity,position);
		createMuzzleFlash()->position = position-velocity;
		createMuzzleFlash()->position = position+velocity*2;
	}
}
GameObject* GameObjectManager::createGenericDebris(const char* meshFileName,Vector2d velocity, Vector2d position)
{
	std::vector<Component*> componentList;

	componentList.push_back(new ComponentDebris(meshFileName,velocity,position));

	GameObject* gameObject = new GameObject(componentList, GameObject::DEBRIS, GameObject::N_ENEMY_BASIC, getNextID(),true);

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createEnemyBomber()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_ENEMY_BOMBER));
		componentList.push_back(new ComponentGraphicsShadow(Resources::SPRITE_SHADOW_ENEMY_BOMBER,true));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::RED));
		componentList.push_back(new ComponentCreateDebris(12));
		componentList.push_back(new ComponentGraphicsShield(false,1.5));
	}
	//	componentList.push_back(new ComponentRandomMovement());

	componentList.push_back(new ComponentCollider(7));
	componentList.push_back(new ComponentVision(100));
	componentList.push_back(new ComponentBehaviour());
	componentList.push_back(new ComponentHealth(1000,1000, 1));
	componentList.push_back(new ComponentTargetAim());
	componentList.push_back(new ComponentProjectileLauncher(200,&ComponentProjectileLauncher::enemyBomberShoot));
	componentList.push_back(new ComponentTargetPath(13));
	componentList.push_back(new ComponentMovement(40));
	componentList.push_back(new ComponentGiveExp(15));
	componentList.push_back(new ComponentAIBomber(60));


	GameObject* gameObject = new GameObject(componentList, GameObject::ENEMY, GameObject::N_ENEMY_BOMBER, getNextID(),true,false,Vector2d(832,-59+Math::random(3)));
	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createEnemyDefender(GameObject* target)
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_ENEMY_DEFENDER));
		componentList.push_back(new ComponentGraphicsShadow(Resources::SPRITE_SHADOW_ENEMY_DEFENDER,true));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::RED));
		componentList.push_back(new ComponentCreateDebris(4));
		componentList.push_back(new ComponentGraphicsShield(false,0.7));
	}

	componentList.push_back(new ComponentCollider(4));
	componentList.push_back(new ComponentVision(100));
	componentList.push_back(new ComponentBehaviour());
	componentList.push_back(new ComponentHealth(100, 100, 1));
	//componentList.push_back(new ComponentAIEnemy(50));
	componentList.push_back(new ComponentTargetAim());
	componentList.push_back(new ComponentProjectileLauncher(500,&ComponentProjectileLauncher::normalBulletShoot));
	componentList.push_back(new ComponentTargetPath(26, -1));
	componentList.push_back(new ComponentMovement(40));
	componentList.push_back(new ComponentGiveExp(15));
	componentList.push_back(new ComponentAIDefender(target,100));

	GameObject* gameObject = new GameObject(componentList, GameObject::ENEMY, GameObject::N_ENEMY_DEFENDER, getNextID(), true,false,Vector2d(832,-59+Math::random(3)));
	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::voidGameObject()
{
	std::vector<Component*> componentList;
	GameObject* gameObject = new GameObject(componentList, GameObject::GHOST, GameObject::N_GHOST, getNextID(), true);
	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createEnemyStill()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_MINE));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_X, DebugColor::RED));
	}
	componentList.push_back(new ComponentTargetPath(10));
	//componentList.push_back(new ComponentCollider(5));
	//componentList.push_back(new ComponentVision(200));
	componentList.push_back(new ComponentHealth(100,100, 1));

	GameObject* gameObject = new GameObject(componentList, GameObject::ENEMY, GameObject::N_ENEMY_DEFENDER, getNextID(), true);
	gameObject->position = Vector2d(Math::random(10,100),Math::random(10,100));
	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

void GameObjectManager::createFuryEnemies()
{
	GameObject* ship1 = NULL;
	GameObject* ship2 = NULL;
	ship1 = createEnemyCombat();
	ship2 = createEnemyCombat();

	Message message;
	message.type = Message::ASSIGN_GAME_OBJECT;
	message.gameObject = ship1;
	ship2->broadcastMessage(message);
	message.gameObject = ship2;
	ship1->broadcastMessage(message);
}

GameObject* GameObjectManager::createEnemyCombat()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_ENEMY_FURY));
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::WHITE));
		componentList.push_back(new ComponentGraphicsShadow(Resources::SPRITE_SHADOW_ENEMY_FURY,true));
		componentList.push_back(new ComponentAIFury(mainPlayer));
		componentList.push_back(new ComponentCreateDebris(10));
		componentList.push_back(new ComponentGraphicsShield(false,1));
	}
	else
	{
		componentList.push_back(new ComponentAIFury(NULL));
	}
	componentList.push_back(new ComponentCollider(7));
	componentList.push_back(new ComponentVision(100));
	componentList.push_back(new ComponentBehaviour());
	componentList.push_back(new ComponentHealth(500, 500, 1));
	//componentList.push_back(new ComponentAIEnemy(20));
	componentList.push_back(new ComponentTargetAim());
	componentList.push_back(new ComponentProjectileLauncher(200,&ComponentProjectileLauncher::normalShoot));
	componentList.push_back(new ComponentTargetPath(13));
	componentList.push_back(new ComponentMovement(60));
	componentList.push_back(new ComponentGiveExp(25));

	GameObject* gameObject = new GameObject(componentList, GameObject::ENEMY, GameObject::N_ENEMY_FURY, getNextID(), true,false, Vector2d(832,-59+Math::random(3)));

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject* GameObjectManager::createRemoteEnemyBasic()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_ENEMY_BASIC));
		componentList.push_back(new ComponentGraphicsShadow(Resources::SPRITE_SHADOW_ENEMY_BASIC,true));
		componentList.push_back(new ComponentMovementInterpolation());
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::RED));
		componentList.push_back(new ComponentCreateDebris(8));
		componentList.push_back(new ComponentGraphicsShield(false,1));
	}
	componentList.push_back(new ComponentCollider(7));
	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, GameObject::N_REMOTE_ENEMY, getNextID(), true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createRemoteFury()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_ENEMY_FURY));
		componentList.push_back(new ComponentGraphicsShadow(Resources::SPRITE_SHADOW_ENEMY_FURY,true));
		componentList.push_back(new ComponentMovementInterpolation());
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::WHITE));
		componentList.push_back(new ComponentCreateDebris(10));
		componentList.push_back(new ComponentGraphicsShield(false,1));
	}
	componentList.push_back(new ComponentCollider(7));
	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, GameObject::N_REMOTE_ENEMY, getNextID(), true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createRemoteDefender()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_ENEMY_DEFENDER));
		componentList.push_back(new ComponentGraphicsShadow(Resources::SPRITE_SHADOW_ENEMY_DEFENDER,true));
		componentList.push_back(new ComponentMovementInterpolation());
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::RED));
		componentList.push_back(new ComponentCreateDebris(3));
		componentList.push_back(new ComponentGraphicsShield(false,0.7));
	}
	componentList.push_back(new ComponentCollider(7));
	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, GameObject::N_REMOTE_ENEMY, getNextID(), true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createRemoteEnemyBomber()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_ENEMY_BOMBER));
		componentList.push_back(new ComponentGraphicsShadow(Resources::SPRITE_SHADOW_ENEMY_BOMBER,true));
		componentList.push_back(new ComponentMovementInterpolation());
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::RED));
		componentList.push_back(new ComponentCreateDebris(8));
		componentList.push_back(new ComponentGraphicsShield(false,1.5));
	}
	componentList.push_back(new ComponentCollider(7));
	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, GameObject::N_REMOTE_ENEMY, getNextID(), true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createMuzzleFlash()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics("../assets/models/animations/spark/muzzle.obj",true));
		componentList.push_back(new ComponentAutoDestroy(20));
	}

	GameObject* gameObject = new GameObject(componentList, GameObject::PARTICLES, GameObject::N_SPARK, getNextID(),false);

	tempGameObjectList.push_back(gameObject);
	return gameObject;
}

GameObject * GameObjectManager::createRepairSystem()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_ANIMATION_REPAIR, true));
	}
	componentList.push_back(new ComponentVision(40));
	componentList.push_back(new ComponentRepairSystem());

	GameObject* gameObject = new GameObject(componentList, GameObject::LOCAL, GameObject::N_REPAIR_SYSTEM, getNextID(), true);

	tempGameObjectList.push_back(gameObject);
	return gameObject;

}



// ------------------ PROYECTILES --------------------

// Bala. Se mueve directa en una direccion establecida en su creacion
// WARNING: El parametro damage no se utiliza
GameObject* GameObjectManager::createBullet(float damage)
{
	std::vector<Component*> componentList;
	//componentList.push_back(new ComponentDebugGameObject());
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_BULLET_GREEN, true));
	}
	componentList.push_back(new ComponentAutoDestroy(GameConstants::BULLET_DESTROY_TIME));
	componentList.push_back(new ComponentVision(GameConstants::BULLET_COLLISION_RADIUS));
	componentList.push_back(new ComponentAIProjectile(50));
	componentList.push_back(new ComponentTargetPath(200, 0));
	componentList.push_back(new ComponentMovement(300));

	GameObject* gameObject = new GameObject(componentList, GameObject::BULLET, GameObject::N_BULLET, getNextID(), false);
	tempGameObjectList.push_back(gameObject);


	return gameObject;
}

GameObject* GameObjectManager::createBulletBomber()
{
	std::vector<Component*> componentList;
	//componentList.push_back(new ComponentDebugGameObject());
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_BULLET_GREEN, true));
	}
	componentList.push_back(new ComponentAutoDestroy(GameConstants::BULLET_DESTROY_TIME));
	componentList.push_back(new ComponentVision(GameConstants::BULLET_COLLISION_RADIUS));
	componentList.push_back(new ComponentAIProjectile(GameConstants::BULLET_DAMAGE_BOMBER));
	componentList.push_back(new ComponentTargetPath(200, 0));
	componentList.push_back(new ComponentMovement(300));

	GameObject* gameObject = new GameObject(componentList, GameObject::BULLET, GameObject::N_BULLET_BOMBER, getNextID(), false);
	tempGameObjectList.push_back(gameObject);


	return gameObject;
}
GameObject* GameObjectManager::createBulletFighter()
{
	std::vector<Component*> componentList;
	//componentList.push_back(new ComponentDebugGameObject());
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_BULLET_GREEN, true));
	}
	componentList.push_back(new ComponentAutoDestroy(GameConstants::BULLET_DESTROY_TIME));
	componentList.push_back(new ComponentVision(GameConstants::BULLET_COLLISION_RADIUS));
	componentList.push_back(new ComponentAIProjectile(GameConstants::BULLET_DAMAGE_FIGHTER));
	componentList.push_back(new ComponentTargetPath(200, 0));
	componentList.push_back(new ComponentMovement(300));

	GameObject* gameObject = new GameObject(componentList, GameObject::BULLET, GameObject::N_BULLET_FIGHTER, getNextID(), false);
	tempGameObjectList.push_back(gameObject);


	return gameObject;
}
GameObject* GameObjectManager::createBulletEngineer()
{
	std::vector<Component*> componentList;
	//componentList.push_back(new ComponentDebugGameObject());
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_BULLET_GREEN, true));
	}
	componentList.push_back(new ComponentAutoDestroy(GameConstants::BULLET_DESTROY_TIME));
	componentList.push_back(new ComponentVision(GameConstants::BULLET_COLLISION_RADIUS));
	componentList.push_back(new ComponentAIProjectile(GameConstants::BULLET_DAMAGE_ENGINEER));
	componentList.push_back(new ComponentTargetPath(200, 0));
	componentList.push_back(new ComponentMovement(300));

	GameObject* gameObject = new GameObject(componentList, GameObject::BULLET, GameObject::N_BULLET_ENGINEER, getNextID(), false);
	tempGameObjectList.push_back(gameObject);


	return gameObject;
}
GameObject* GameObjectManager::createBulletBattleship()
{
	std::vector<Component*> componentList;
	//componentList.push_back(new ComponentDebugGameObject());
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_BULLET_GREEN, true));
	}
	componentList.push_back(new ComponentAutoDestroy(GameConstants::BULLET_DESTROY_TIME));
	componentList.push_back(new ComponentVision(GameConstants::BULLET_COLLISION_RADIUS));
	componentList.push_back(new ComponentAIProjectile(GameConstants::BULLET_DAMAGE_BATTLESHIP));
	componentList.push_back(new ComponentTargetPath(200, 0));
	componentList.push_back(new ComponentMovement(300));

	GameObject* gameObject = new GameObject(componentList, GameObject::BULLET, GameObject::N_BULLET_BATTLESHIP, getNextID(), false);
	tempGameObjectList.push_back(gameObject);


	return gameObject;
}




// Proyectil. Puede modificar su direccion para moverse hacia un objetivo en movimiento
GameObject* GameObjectManager::createProjectile( float damage)
{
	std::vector<Component*> componentList;
	//componentList.push_back(new ComponentDebugGameObject());
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_ROCKET));
	}
	componentList.push_back(new ComponentAutoDestroy(GameConstants::PROJECTILE_DESTROY_TIME));
	componentList.push_back(new ComponentVision(GameConstants::BULLET_COLLISION_RADIUS));
	componentList.push_back(new ComponentAIProjectile(10));
	componentList.push_back(new ComponentTargetPath(1000, 5));
	componentList.push_back(new ComponentMovement(100));

	GameObject* gameObject = new GameObject(componentList, GameObject::PROJECTILE, GameObject::N_PROJECTILE, getNextID(), false);
	tempGameObjectList.push_back(gameObject);


	return gameObject;
}

//No se utiliza
GameObject* GameObjectManager::createFireBolt(float damage)
{
	std::vector<Component*> componentList;
	//componentList.push_back(new ComponentDebugGameObject());
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_BULLET, true));
	}
	componentList.push_back(new ComponentAutoDestroy(GameConstants::BULLET_DESTROY_TIME));
	componentList.push_back(new ComponentVision(GameConstants::BULLET_COLLISION_RADIUS));
	componentList.push_back(new ComponentAIProjectile(-1));
	componentList.push_back(new ComponentTargetPath(200, 5));
	componentList.push_back(new ComponentMovement(100));

	GameObject* gameObject = new GameObject(componentList, GameObject::RAY, GameObject::N_RAY, getNextID(), false);
	tempGameObjectList.push_back(gameObject);


	return gameObject;
}

GameObject* GameObjectManager::createRay(GameObject* shooter)
{
	std::vector<Component*> componentList;
	//componentList.push_back(new ComponentDebugGameObject());
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_RAY, true));
	}
	//componentList.push_back(new ComponentAutoDestroy(GameConstants::BULLET_DESTROY_TIME));
	componentList.push_back(new ComponentRay(shooter));
	GameObject* gameObject = new GameObject(componentList, GameObject::RAY, GameObject::N_RAY, getNextID(), false);
	tempGameObjectList.push_back(gameObject);


	return gameObject;
}

GameObject* GameObjectManager::createRemoteRay()
{
	std::vector<Component*> componentList;
	//componentList.push_back(new ComponentDebugGameObject());
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_RAY, true));
		componentList.push_back(new ComponentMovementInterpolation());
	}
	//componentList.push_back(new ComponentAutoDestroy(GameConstants::BULLET_DESTROY_TIME));
	//componentList.push_back(new ComponentRemoteRay());
	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, GameObject::N_RAY, getNextID(), false);
	tempGameObjectList.push_back(gameObject);


	return gameObject;
}

GameObject* GameObjectManager::createTargetProjectile( float damage)
{
	std::vector<Component*> componentList;
	//componentList.push_back(new ComponentDebugGameObject());
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_ROCKET));
	}
	componentList.push_back(new ComponentAutoDestroy(GameConstants::PROJECTILE_DESTROY_TIME));
	componentList.push_back(new ComponentVision(GameConstants::BULLET_COLLISION_RADIUS));
	componentList.push_back(new ComponentAIMine(3));
	componentList.push_back(new ComponentTargetPath(1000, 5));
	componentList.push_back(new ComponentMovement(100));

	GameObject* gameObject = new GameObject(componentList, GameObject::PROJECTILE, GameObject::N_PROJECTILE, getNextID(), false);
	tempGameObjectList.push_back(gameObject);


	return gameObject;
}

// ------------------ PROYECTILES --------------------

// Bala. Se mueve directa en una direccion establecida en su creacion
GameObject* GameObjectManager::createEnemyBullet(float damage)
{
	std::vector<Component*> componentList;
	//componentList.push_back(new ComponentDebugGameObject());
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_BULLET_RED,true));
	}
	componentList.push_back(new ComponentAutoDestroy(GameConstants::BULLET_DESTROY_TIME));
	componentList.push_back(new ComponentVision(GameConstants::BULLET_COLLISION_RADIUS));
	componentList.push_back(new ComponentAIProjectile(20));
	componentList.push_back(new ComponentTargetPath(200, 0));
	componentList.push_back(new ComponentMovement(300));

	GameObject* gameObject = new GameObject(componentList, GameObject::BULLET, GameObject::N_ENEMY_BULLET, getNextID(), true);
	tempGameObjectList.push_back(gameObject);


	return gameObject;
}

// Proyectil. Puede modificar su direccion para moverse hacia un objetivo en movimiento
GameObject* GameObjectManager::createEnemyProjectile( float damage)
{
	std::vector<Component*> componentList;
	//componentList.push_back(new ComponentDebugGameObject());
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_ROCKET));
	}
	componentList.push_back(new ComponentAutoDestroy(GameConstants::PROJECTILE_DESTROY_TIME));
	componentList.push_back(new ComponentVision(GameConstants::BULLET_COLLISION_RADIUS));
	componentList.push_back(new ComponentTargetPath(100, 10));
	componentList.push_back(new ComponentAIProjectile(50));
	componentList.push_back(new ComponentMovement(200));

	GameObject* gameObject = new GameObject(componentList, GameObject::PROJECTILE, GameObject::N_PROJECTILE, getNextID(), true);
	tempGameObjectList.push_back(gameObject);


	return gameObject;
}


GameObject* GameObjectManager::getById( ID id )
{
	for(auto gameObject : gameObjectList)
	{
		if(gameObject->getId() == id)
		{
			return gameObject;
		}
	}
	return NULL;
}

ID GameObjectManager::getNextID()
{
	return nextID++;
}

GameObject* GameObjectManager::createRemoteTurret()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_TURRET_2_TOP));
		componentList.push_back(new ComponentGraphicsStatics(Resources::MESH_INVENTORY_TURRET_2_BOTTOM));
		componentList.push_back(new ComponentMovementInterpolation());
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentGraphicsShield(true,1.55));

	}
	componentList.push_back(new ComponentCollider(7));

	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, GameObject::N_REMOTE_TURRET, getNextID(), false,true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createRemoteTurretBasic()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_TURRET_TOP));
		componentList.push_back(new ComponentGraphicsStatics("../assets/models/inventory/turrets/turret_basic/turret_basic_bot.obj"));
		componentList.push_back(new ComponentMovementInterpolation());
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentGraphicsShield(true,1.25));


	}
	componentList.push_back(new ComponentCollider(7));

	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, GameObject::N_REMOTE_TURRET, getNextID(), false,true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createRemoteTurret2Canon()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_TURRET_2_CANON_TOP));
		componentList.push_back(new ComponentGraphicsStatics(Resources::MESH_INVENTORY_TURRET_2_CANON_BOTTOM));
		componentList.push_back(new ComponentMovementInterpolation());
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentGraphicsShield(true,1.25));

	}
	componentList.push_back(new ComponentCollider(7));

	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, GameObject::N_REMOTE_TURRET, getNextID(), false,true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}



GameObject* GameObjectManager::createRemoteTurretBigCanon()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_TURRET_BIG_CANON_TOP));
		componentList.push_back(new ComponentGraphicsStatics(Resources::MESH_INVENTORY_TURRET_BIG_CANON_BOTTOM));
		componentList.push_back(new ComponentMovementInterpolation());
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentGraphicsShield(true,1.3));
	}
	componentList.push_back(new ComponentCollider(7));

	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, GameObject::N_REMOTE_TURRET, getNextID(), false,true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}


GameObject* GameObjectManager::createRemoteTurret1Canon()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_TURRET_1_CANON_TOP));
		componentList.push_back(new ComponentGraphicsStatics(Resources::MESH_INVENTORY_TURRET_1_CANON_BOTTOM));
		componentList.push_back(new ComponentMovementInterpolation());
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentGraphicsShield(true,1.2));

	}
	componentList.push_back(new ComponentCollider(7));

	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, GameObject::N_REMOTE_TURRET, getNextID(), false,true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}


GameObject* GameObjectManager::createRemoteTurretGun()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_TURRET_GUN_TOP));
		componentList.push_back(new ComponentGraphicsStatics(Resources::MESH_INVENTORY_TURRET_GUN_BOTTOM));
		componentList.push_back(new ComponentMovementInterpolation());
		componentList.push_back(new ComponentMiniMap(Resources::SPRITE_CIRCLE, DebugColor::GREEN));
		componentList.push_back(new ComponentGraphicsShield(true,1.1));

	}
	componentList.push_back(new ComponentCollider(7));

	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, GameObject::N_REMOTE_TURRET, getNextID(), false,true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}


GameObject* GameObjectManager::createRemoteBullet()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_BULLET, true));

		//componentList.push_back(new ComponentGraphics("../assets/enemy.3ds", Math::random(0, DebugColor::_COLOR_COUNT)));
	}
	componentList.push_back(new ComponentAutoDestroy(GameConstants::BULLET_DESTROY_TIME));
	componentList.push_back(new ComponentTargetPath(200));
	componentList.push_back(new ComponentMovement(300));
	componentList.push_back(new ComponentVision(GameConstants::BULLET_COLLISION_RADIUS));
	componentList.push_back(new ComponentAIProjectile(20));

	//componentList.push_back(new ComponentCollider(7)); //Los disparos son remotes y te empujan

	GameObject* gameObject = new GameObject(componentList, GameObject::BULLET, GameObject::N_BULLET, getNextID(), false);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createRemoteProjectile()
{
	return createRemote(Resources::MESH_PROJECTILE_ROCKET, GameObject::N_PROJECTILE);
}

GameObject* GameObjectManager::createRemoteMine()
{
	return createRemote(Resources::MESH_INVENTORY_MINE, GameObject::N_MINE);
}

GameObject* GameObjectManager::createRemoteMine2()
{
	return createRemote(Resources::MESH_INVENTORY_MINE_2, GameObject::N_MINE2);
}

GameObject* GameObjectManager::createRemoteMine3()
{
	return createRemote(Resources::MESH_INVENTORY_MINE_3, GameObject::N_MINE3);
}

GameObject* GameObjectManager::createRemoteExplosion()
{
	return createRemote(Resources::MESH_ANIMATION_BOOM, GameObject::N_MINE);
}

GameObject* GameObjectManager::createRemoteRepairSystem()
{
	//return createRemote(Resources::MESH_ANIMATION_REPAIR, GameObject::N_REPAIR_SYSTEM, true);
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_ANIMATION_REPAIR, true));
		componentList.push_back(new ComponentMovementInterpolation());
	}
	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, GameObject::N_REPAIR_SYSTEM, getNextID(), true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createSkillBomb(Vector2d pos)
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_ANIMATION_REPAIR, true));
	}
	componentList.push_back(new ComponentVision(20));
	componentList.push_back(new ComponentExplosionDamage(500));

	GameObject* gameObject = new GameObject(componentList, GameObject::SKILL, GameObject::N_SKILL_BOMB, getNextID(), false,false,pos);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createSkillHeal(Vector2d pos)
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_ANIMATION_REPAIR, true));
	}

	componentList.push_back(new ComponentVision(20));
	componentList.push_back(new ComponentExplosionDamage(-50));

	GameObject* gameObject = new GameObject(componentList, GameObject::SKILL, GameObject::N_SKILL_HEAL, getNextID(), true,false,pos);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createSkillMultimpleShot(Vector2d posPlayer)
{
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
		projectileOrBullet->position = posPlayer + position;
		projectileOrBullet->velocity = position/2;
		message.target = posPlayer + position*5000;
		projectileOrBullet->broadcastMessage(message);
	}

	return projectileOrBullet;
}

GameObject* GameObjectManager::createSkillTargetProjectile()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_ANIMATION_REPAIR, true));
		componentList.push_back(new ComponentMovementInterpolation());
	}
	componentList.push_back(new ComponentAutoDestroy(3000));

	GameObject* gameObject = new GameObject(componentList, GameObject::SKILL, GameObject::N_SKILL_TARGET_PROJECTILE, getNextID(), true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createRemoteSkillHeal()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_ANIMATION_REPAIR, true));
		componentList.push_back(new ComponentMovementInterpolation());
		componentList.push_back(new ComponentAnimationArea(1,Vector2d()));
	}
	//componentList.push_back(new ComponentAutoDestroy(3000));

	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, GameObject::N_SKILL_HEAL, getNextID(), true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createRemoteSkillBomb()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics(Resources::MESH_ANIMATION_REPAIR, true));
		componentList.push_back(new ComponentMovementInterpolation());
		componentList.push_back(new ComponentAnimationArea(0,Vector2d()));
	}
	//componentList.push_back(new ComponentAutoDestroy(3000));

	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, GameObject::N_SKILL_BOMB, getNextID(), true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createRemote( const char* modelPath, int objectName, bool transparent )
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(modelPath, transparent));
		componentList.push_back(new ComponentMovementInterpolation());
	}
	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, (GameObject::Name)objectName, getNextID(), false);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectManager::createRemoteWall()
{
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		//componentList.push_back(new ComponentDebugGameObject());
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_ENERGY_WALL));
		componentList.push_back(new ComponentMovementInterpolation());
	}
	GameObject* gameObject = new GameObject(componentList, GameObject::REMOTE, GameObject::N_WALL, getNextID(), false,true);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}

int GameObjectManager::getNumberOfEnemies()
{
	int numberOfEnemies=0;

	for (std::size_t i = 0; i < this->gameObjectList.size(); i++)
	{
		if(gameObjectList[i]->getTag()==GameObject::ENEMY )
		{
			numberOfEnemies++;
		}
	}
	return numberOfEnemies;
}


void GameObjectManager::killAllEnemies() //Mata a todos los enemigos
{
	//for (std::size_t i = 0; i < this->gameObjectList.size(); i++)		
	for (std::size_t i = 0; i < this->gameObjectList.size(); i++)	
	{
		if(gameObjectList[i]->getTag()==GameObject::ENEMY)		
		{
			gameObjectList[i]->kill();//gameObjectList[i]->kill(); 
		}
	}
}

void GameObjectManager::killInventories()
{
	for (std::size_t i = 0; i < this->gameObjectList.size(); i++)	
	{
		if(gameObjectList[i]->getTag()==GameObject::INVENTORY)			
			gameObjectList[i]->kill();//gameObjectList[i]->kill(); 
	}
}

unsigned int GameObjectManager::getUpdateTime()
{
	return updateTime;
}

GameObject* GameObjectManager::getMainPlayer()
{
	return mainPlayer;

}



void GameObjectManager::loadModels()
{
	std::vector<Component*> componentList;

	if(!gameManager->isServer())
	{
		// Ordenados por frecuencia de acceso
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_BULLET_RED));
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_BULLET_BLUE));
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_BULLET_GREEN));
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_ROCKET));
		componentList.push_back(new ComponentGraphics(Resources::MESH_PROJECTILE_RAY));

		componentList.push_back(new ComponentGraphics(Resources::MESH_SHIELD));

		componentList.push_back(new ComponentGraphics(Resources::MESH_ENEMY_BASIC));
		componentList.push_back(new ComponentGraphics(Resources::MESH_ENEMY_BOMBER));
		componentList.push_back(new ComponentGraphics(Resources::MESH_ENEMY_DEFENDER));
		componentList.push_back(new ComponentGraphics(Resources::MESH_ENEMY_FURY));

		componentList.push_back(new ComponentGraphics(Resources::MESH_PLAYER_BOMBER));
		componentList.push_back(new ComponentGraphics(Resources::MESH_PLAYER_ENGINEER));
		componentList.push_back(new ComponentGraphics(Resources::MESH_PLAYER_BATTLESHIP));
		componentList.push_back(new ComponentGraphics(Resources::MESH_PLAYER_FIGHTER));

		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_TURRET_2_TOP));
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_TURRET_2_BOTTOM));
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_MINE));
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_ENERGY));
		componentList.push_back(new ComponentGraphics(Resources::MESH_INVENTORY_ENERGY_WALL));

		componentList.push_back(new ComponentGraphics(Resources::MESH_BUILDING_BASE));
		componentList.push_back(new ComponentGraphics(Resources::MESH_BUILDING_MINI_BASE));
		componentList.push_back(new ComponentGraphics(Resources::MESH_BUILDING_MINI_HANGAR));
		componentList.push_back(new ComponentGraphics(Resources::MESH_BUILDING_HANGAR));
		componentList.push_back(new ComponentGraphics(Resources::MESH_BUILDING_EXTRACTOR));
	}

	GameObject* gameObject = new GameObject(componentList, GameObject::OTHER, GameObject::N_OTHER, getNextID(), false);
	tempGameObjectList.push_back(gameObject);
}

GameObject* GameObjectManager::createMap()
{
	//Todavia no esta listo el modelo
	std::vector<Component*> componentList;
	if(!gameManager->isServer())
	{
		componentList.push_back(new ComponentGraphics("../assets/models/map/map.obj"));
	}else
	{
		std::cout << "WARNING: Intentando crear el mapa en el servidor cuando solo tiene graficos\n";
		return NULL;
	}

	GameObject* gameObject = new GameObject(componentList, GameObject::GROUND, GameObject::N_GROUND, getNextID(), false);

	tempGameObjectList.push_back(gameObject);

	return gameObject;
}
