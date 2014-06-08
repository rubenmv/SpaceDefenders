#include "Connection.h"
#include <iostream>
#include "GameManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "NetworkEngine.h"

RakNet::Replica3* Connection::AllocReplica(RakNet::BitStream* allocationId, RakNet::ReplicaManager3* replicaManager3)
{
	unsigned char name;
	allocationId->Read(name);

	GameObjectManager* gameObjectManager = GameManager::getInstance()->getGameObjectManager();
	
	if(name == GameObject::N_MAIN_PLAYER)
	{
		return (RakNet::Replica3*) gameObjectManager->createRemotePlayer();
	}

	if(name == GameObject::N_BULLET) return (RakNet::Replica3*) gameObjectManager->createBullet(20);

	if(name == GameObject::N_BULLET_BOMBER) return (RakNet::Replica3*) gameObjectManager->createBulletBomber();

	if(name == GameObject::N_BULLET_FIGHTER) return (RakNet::Replica3*) gameObjectManager->createBulletFighter();

	if(name == GameObject::N_BULLET_ENGINEER) return (RakNet::Replica3*) gameObjectManager->createBulletEngineer();

	if(name == GameObject::N_BULLET_BATTLESHIP) return (RakNet::Replica3*) gameObjectManager->createBulletBattleship();

	if(name == GameObject::N_ENEMY_BULLET) return (RakNet::Replica3*) gameObjectManager->createEnemyBullet(20);

	if(name == GameObject::N_PROJECTILE) return (RakNet::Replica3*) gameObjectManager->createRemoteProjectile();

	if(name == GameObject::N_ENEMY_BASIC) return (RakNet::Replica3*) gameObjectManager->createRemoteEnemyBasic();

	if(name == GameObject::N_TURRET_PROJECTILE) return (RakNet::Replica3*) gameObjectManager->createRemoteTurret();

	if(name == GameObject::N_TURRET_BULLET) return (RakNet::Replica3*) gameObjectManager->createRemoteTurretBasic();

	if(name == GameObject::N_TURRET_2_CANON) return (RakNet::Replica3*) gameObjectManager->createRemoteTurret2Canon();

	if(name == GameObject::N_TURRET_1_CANON) return (RakNet::Replica3*) gameObjectManager->createRemoteTurret1Canon();

	if(name == GameObject::N_TURRET_BIG_CANON) return (RakNet::Replica3*) gameObjectManager->createRemoteTurretBigCanon();
	
	if(name == GameObject::N_MINE) return (RakNet::Replica3*) gameObjectManager->createRemoteMine();

	if(name == GameObject::N_WALL) return (RakNet::Replica3*) gameObjectManager->createRemoteWall();

	if(name == GameObject::N_EXPLOSION) return (RakNet::Replica3*) gameObjectManager->createExplosion(20);

	if(name == GameObject::N_ENEMY_FURY) return (RakNet::Replica3*) gameObjectManager->createRemoteFury();

	if(name == GameObject::N_ENEMY_DEFENDER) return (RakNet::Replica3*) gameObjectManager->createRemoteDefender();

	if(name == GameObject::N_ENEMY_BOMBER) return (RakNet::Replica3*) gameObjectManager->createRemoteEnemyBomber();
	
	if(name == GameObject::N_BOMBER_PLAYER) return (RakNet::Replica3*) gameObjectManager->createRemoteBomber();

	if(name == GameObject::N_BATTLESHIP_PLAYER) return (RakNet::Replica3*) gameObjectManager->createRemoteBattleship();

	if(name == GameObject::N_ENGINEER_PLAYER) return (RakNet::Replica3*) gameObjectManager->createRemoteEngineer();

	if(name == GameObject::N_FIGHTER_PLAYER) return (RakNet::Replica3*) gameObjectManager->createRemoteFighter();

	if(name == GameObject::N_REPAIR_SYSTEM) return (RakNet::Replica3*) gameObjectManager->createRemoteRepairSystem();
	
	if(name == GameObject::N_SKILL_BOMB) return (RakNet::Replica3*) gameObjectManager->createRemoteSkillBomb();

	if(name == GameObject::N_SKILL_HEAL) return (RakNet::Replica3*) gameObjectManager->createRemoteSkillHeal();

	if(name == GameObject::N_SPARK) return (RakNet::Replica3*) gameObjectManager->createSpark();

	if(name == GameObject::N_BUILDING_MINE) return (RakNet::Replica3*) gameObjectManager->createBuildingMine(Vector2d());

	//if(name == GameObject::N_ENERGY_WALL) return (RakNet::Replica3*) gameObjectManager->createEnergyWall(Vector2d(),Vector2d());

	if(name == GameObject::N_BUILDING_TURRET_PROJECTILE) return (RakNet::Replica3*) gameObjectManager->createBuildingTurret(Vector2d());

	if(name == GameObject::N_BUILDING_TURRET_BULLET) return (RakNet::Replica3*) gameObjectManager->createBuildingTurret2(Vector2d());

	if(name == GameObject::N_BUILDING_TURRET_2_CANON) return (RakNet::Replica3*) gameObjectManager->createBuildingTurret2Canon(Vector2d());

	if(name == GameObject::N_BUILDING_TURRET_BIG_CANON) return (RakNet::Replica3*) gameObjectManager->createBuildingTurretBigCanon(Vector2d());
	
	if(name == GameObject::N_BUILDING_TURRET_1_CANON) return (RakNet::Replica3*) gameObjectManager->createBuildingTurret1Canon(Vector2d());

	if(name == GameObject::N_BASE) return (RakNet::Replica3*) gameObjectManager->createRemoteBase();
	
	if(name == GameObject::N_MINI_BASE) return (RakNet::Replica3*) gameObjectManager->createRemoteMiniBase();

	if(name == GameObject::N_MINI_HANGAR) return (RakNet::Replica3*) gameObjectManager->createRemoteMiniHangar();

	if(name == GameObject::N_HANGAR) return (RakNet::Replica3*) gameObjectManager->createRemoteHangar();

	if(name == GameObject::N_EXTRACTORS) return (RakNet::Replica3*) gameObjectManager->createRemoteExtractors();

	if(name == GameObject::N_RAY) return (RakNet::Replica3*) gameObjectManager->createRemoteRay();

	if(name == GameObject::N_MINE2) return (RakNet::Replica3*) gameObjectManager->createRemoteMine2();

	if(name == GameObject::N_MINE3) return (RakNet::Replica3*) gameObjectManager->createRemoteMine3();

	return 0;
}
