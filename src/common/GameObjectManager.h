#pragma once

#include <vector>
#include "ID.h"
#include "ComponentInventory.h"
#include "ComponentConstruction.h"

#include "Clock.h"
#include "Vector2d.h"

class GameObject;
class GameManager;
class Vector2d;
class ComponentInventory;
class ComponentConstruction;
class Skill;

namespace GameConstants
{
	// Radios
	const float PLAYER_VISION_RADIUS = 205;
	const float PLAYER_COLLISION_RADIUS = 4;
	const float BASE_COLLISION_RADIUS = 25;
	const float BULLET_COLLISION_RADIUS = 3;

	// Stats
	const float BASE_HEALTH = 500;
	const float BASE_SHIELD = 500;
	const float BASE_RECHARGE_TIME = 10;

	const float TURRET_HEALTH = 20;
	const float TURRET_SHIELD = 200;
	const float TURRET_RECHARGE_TIME = 1;

	const float BUILDING_ELEMENT_HEALTH = 50;
	const float BUILDING_ELEMENT_SHIELD = 0;
	const float BUILDING_ELEMENT_RECHARGE_TIME = 100;

	// Bullets
	const int PLAYER_FIRE_RATE = 200;
	const float FIRE_DESTROY_TIME = 500;
	const float SPARK_DESTROY_TIME = 100;

	const float BULLET_DESTROY_TIME = 3000;
	const float PROJECTILE_DESTROY_TIME = 5000;

	const float BULLET_DAMAGE_BOMBER = 125;
	const float BULLET_DAMAGE_FIGHTER = 20;
	const float BULLET_DAMAGE_ENGINEER = 25;
	const float BULLET_DAMAGE_BATTLESHIP = 50;
	// Leveling
	const int LEVEL_DEFAULT = 1;
	const float EXP_DEFAULT = 0;
	const float MOD_HP = 0.3;
	const float MOD_DAMAGE = 0.3;

	static const wchar_t* COOLDOWN_MESSAGE = L"RECHARGING...";
	static const wchar_t* CANT_BUILD_MESSAGE = L"INVALID LOCATION";
	static const wchar_t* SKILL_UNAVAILABLE_MESSAGE = L"SKILL UNAVAILABLE";
	static const wchar_t* INSUFFICIENT_JADE_MESSAGE = L"INSUFFICIENT JADE";

}

namespace Resources
{
	// Debug y otros
	static const char* MESH_CUBE = "../assets/models/cube/cube.obj";
	static const char* MESH_ARROW = "../assets/models/others/arrow.obj";
	static const char* MESH_ERROR = "../assets/models/others/error.obj"; // Cuando falla la carga de un modelo

	// Players
	static const char* MESH_PLAYER_DEFAULT = "../assets/models/players/player.obj";
	static const char* MESH_PLAYER_BOMBER = "../assets/models/players/bomber/bomber.obj";
	static const char* MESH_PLAYER_ENGINEER = "../assets/models/players/engineer/engineer.obj";
	static const char* MESH_PLAYER_BATTLESHIP = "../assets/models/players/battleship/battleship.obj";
	static const char* MESH_PLAYER_FIGHTER = "../assets/models/players/fighter/fighter.obj";
	// Enemies
	static const char* MESH_ENEMY_BASIC = "../assets/models/enemies/basic/basic.obj";
	static const char* MESH_ENEMY_BOMBER = "../assets/models/enemies/bomber/bomber.obj";
	static const char* MESH_ENEMY_DEFENDER = "../assets/models/enemies/defender/defender.obj";
	static const char* MESH_ENEMY_FURY = "../assets/models/enemies/fury/fury.obj";
	// Buildings
	static const char* MESH_BUILDING_BASE = "../assets/models/base/base/base.obj";
	static const char* MESH_BUILDING_MINI_HANGAR = "../assets/models/base/mini_hangar/mini_hangar.obj";
	static const char* MESH_BUILDING_MINI_BASE = "../assets/models/base/mini_base/mini_base.obj";
	static const char* MESH_BUILDING_HANGAR = "../assets/models/base/hangar/hangar.obj";
	static const char* MESH_BUILDING_EXTRACTOR = "../assets/models/base/extractor/extractor.obj";

	// Sprites
	static const char* SPRITE_CIRCLE = "../assets/images/minimap/circle.png";
	static const char* SPRITE_TRIANGLE = "../assets/images/minimap/triangle.png";
	static const char* SPRITE_X = "../assets/images/minimap/x.png";

	static const char* SPRITE_FIREBALL = "../assets/models/particles/fireball.png";
	static const char* SPRITE_FIREBALL_BLUE = "../assets/models/particles/fireball_blue.png";
	static const char* SPRITE_SMOKE = "../assets/models/particles/smoke.png";
	static const char* SPRITE_SPARK = "../assets/models/particles/spark.png";

	// Otros, trampas...
	static const char* MESH_INVENTORY_MINE = "../assets/models/inventory/mines/mine.obj";
	static const char* MESH_INVENTORY_MINE_2 = "../assets/models/inventory/mines/mine2/mine2.obj";
	static const char* MESH_INVENTORY_MINE_3 = "../assets/models/inventory/mines/mine3/mine3.obj";
	static const char* MESH_INVENTORY_ENERGY = "../assets/models/inventory/walls/energy_wall/energy.obj";
	static const char* MESH_INVENTORY_ENERGY_WALL = "../assets/models/inventory/walls/energy_wall/energy_wall.obj";
	static const char* MESH_PROJECTILE_BULLET = "../assets/models/projectiles/bullet/bullet.obj";
	static const char* MESH_PROJECTILE_BULLET_RED = "../assets/models/projectiles/bullet/red_bullet.obj";
	static const char* MESH_PROJECTILE_BULLET_BLUE = "../assets/models/projectiles/bullet/blue_bullet.obj";
	static const char* MESH_PROJECTILE_BULLET_GREEN = "../assets/models/projectiles/bullet/green_bullet.obj";
	static const char* MESH_PROJECTILE_ROCKET = "../assets/models/projectiles/rocket/rocket.obj";
	static const char* MESH_PROJECTILE_RAY = "../assets/models/projectiles/ray/ray.obj";
	static const char* MESH_PROJECTILE_MUZZLE = "../assets/models/animations/spark/muzzle.obj";
	static const char* MESH_PROJECTILE_SPARK = "../assets/models/animations/spark/spark.obj";
	static const char* MESH_PROJECTILE_FIRE = "../assets/models/animations/fire/fire.obj";

	// Turret 2
	static const char* MESH_INVENTORY_TURRET_TOP = "../assets/models/inventory/turrets/turret_basic/turret_basic_top.obj";
	static const char* MESH_INVENTORY_TURRET_2_TOP = "../assets/models/inventory/turrets/turret_2/turret2top.obj";
	static const char* MESH_INVENTORY_TURRET_2_BOTTOM = "../assets/models/inventory/turrets/turret_2/turret2bot.obj";

	// Turret 2 Canon
	static const char* MESH_INVENTORY_TURRET_2_CANON_TOP = "../assets/models/inventory/turrets/turret_2_canon/turret_2_canon_top.obj";
	static const char* MESH_INVENTORY_TURRET_2_CANON_BOTTOM = "../assets/models/inventory/turrets/turret_2_canon/turret_2_canon_bot.obj";

	// Turret Big Canon
	static const char* MESH_INVENTORY_TURRET_BIG_CANON_TOP = "../assets/models/inventory/turrets/turret_big_canon/turret_big_canon_top.obj";
	static const char* MESH_INVENTORY_TURRET_BIG_CANON_BOTTOM = "../assets/models/inventory/turrets/turret_big_canon/turret_big_canon_bot.obj";

	// Turret 1 Canon
	static const char* MESH_INVENTORY_TURRET_1_CANON_TOP = "../assets/models/inventory/turrets/turret_1_canon/turret_1_canon_top.obj";
	static const char* MESH_INVENTORY_TURRET_1_CANON_BOTTOM = "../assets/models/inventory/turrets/turret_1_canon/turret_1_canon_bot.obj";

	// Turret Gun
	static const char* MESH_INVENTORY_TURRET_GUN_TOP = "../assets/models/inventory/turrets/turret_gun/turret_gun_top.obj";
	static const char* MESH_INVENTORY_TURRET_GUN_BOTTOM = "../assets/models/inventory/turrets/turret_gun/turret_gun_bot.obj";

	// Animaciones
	static const char* MESH_ANIMATION_BOOM = "../assets/models/animations/boom/boom.obj";
	static const char* MESH_ANIMATION_REPAIR = "../assets/models/animations/repair_system/repair_system.obj";	

	//Debris
	static const char* MESH_DEBRIS_1 = "../assets/models/debris/piece1.obj";
	static const char* MESH_DEBRIS_2 = "../assets/models/debris/piece2.obj";
	static const char* MESH_DEBRIS_3 = "../assets/models/debris/piece3.obj";
	static const char* MESH_DEBRIS_4 = "../assets/models/debris/piece5.obj";
	static const char* MESH_DEBRIS_5 = "../assets/models/debris/piece6.obj";

	//Sombras
	static const char* SPRITE_SHADOW_ENEMY_BOMBER = "../assets/models/enemies/bomber/shadow_bomber.obj";
	static const char* SPRITE_SHADOW_ENEMY_FURY = "../assets/models/enemies/fury/shadow_fury.obj";
	static const char* SPRITE_SHADOW_ENEMY_DEFENDER = "../assets/models/enemies/defender/shadow_defender.obj";
	static const char* SPRITE_SHADOW_ENEMY_BASIC = "../assets/models/enemies/basic/shadow_basic.obj";
	static const char* SPRITE_SHADOW_PLAYER_BOMBER = "../assets/models/players/bomber/shadow_bomber.obj";
	static const char* SPRITE_SHADOW_PLAYER_BATTLESHIP = "../assets/models/players/battleship/shadow_battleship.obj";
	static const char* SPRITE_SHADOW_PLAYER_ENGINEER = "../assets/models/players/engineer/shadow_engineer.obj";
	static const char* SPRITE_SHADOW_PLAYER_FIGHTER = "../assets/models/players/fighter/shadow_fighter.obj";

	// Sonidos
	static const char* SOUND_PLAYER_PROJECTILE = "../assets/audio/sounds/projectiles/bullet.wav";	

	// SHADERS

	//Shield
	static const char* MESH_SHIELD  = "../assets/models/players/shield/shield.obj";
	static const char* SHADER_SHIELD_VERT = "../assets/shaders/shield/shield.vert";
	static const char* SHADER_SHIELD_FRAG = "../assets/shaders/shield/shield.frag";

	//Terrain
	static const char* SHADER_TERRAIN_VERT = "../assets/shaders/terrain/terrain.vert";
	static const char* SHADER_TERRAIN_FRAG = "../assets/shaders/terrain/terrain.frag";

	static const char* TEXTURE_TERRAIN_MASK = "../assets/models/map/mask.png";
	static const char* TEXTURE_TERRAIN_RED = "../assets/models/map/red_tile.png";
	static const char* TEXTURE_TERRAIN_GREEN = "../assets/models/map/green_tile.png";
	static const char* TEXTURE_TERRAIN_BLUE = "../assets/models/map/blue_tile.png";

	// Postprocesado
	// Pass-through no aplica ningun efecto y sirve de base pasa el resto de efectos
	static const char* SHADER_POST_PASS_VERT = "../assets/shaders/pass_through/pass_through.vert";
	// El frag por defecto pega la textura tal cual
	static const char* SHADER_POST_PASS_FRAG = "../assets/shaders/pass_through/pass_through.frag";
	// Aqui el resto de .frag de cada efecto
	static const char* SHADER_POST_VIGNETTE = "../assets/shaders/postprocessing/vignette.frag";
	static const char* SHADER_POST_GLOW = "../assets/shaders/postprocessing/glow.frag";
	static const char* SHADER_POST_MINIME = "../assets/shaders/postprocessing/minime.frag";
	static const char* SHADER_POST_BW = "../assets/shaders/postprocessing/bw.frag";
	static const char* SHADER_POST_BLUR = "../assets/shaders/postprocessing/blur.frag";
	static const char* SHADER_POST_BILLBOARD = "../assets/shaders/postprocessing/billboard.frag";
}

//Contiene las listas de objetos y crea los distintos tipos de GameObjects (Factory)
class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void update();

	void clear();

	GameObject* getById(ID id);

	std::vector<GameObject*> getGameObjectList()
	{
		return gameObjectList;
	}

	// Players
	GameObject* createPlayer(int character, std::vector<Skill*> skills);
	GameObject* createPlayer();
	GameObject* createBomber();
	GameObject* createEngineer();
	GameObject* createBattleship();
	GameObject* createPlayerDefender();
	GameObject* createFighter();
	GameObject* createTurret();
	GameObject* createTurret2();
	GameObject* createTurret2Canon();
	GameObject* createTurretBigCanon();
	GameObject* createTurret1Canon();
	GameObject* createTurretGun();
	GameObject* createEnemyBasic();
	GameObject* createEnemyBomber();
	GameObject* createEnemyDefender(GameObject* target);
	GameObject* createEnemyStill();
	GameObject* voidGameObject();

	GameObject * createRepairSystem();

	void createFuryEnemies();
	GameObject* createMine();
	GameObject* createMine2();
	GameObject* createMine3();
	GameObject* createWall();
	// Proyectiles
	GameObject* createMuzzleFlash();
	GameObject* createProjectile(float damage);
	GameObject* createTargetProjectile( float damage);
	GameObject* createBullet(float damage);
	GameObject* createBulletBomber();
	GameObject* createBulletFighter();
	GameObject* createBulletEngineer();
	GameObject* createBulletBattleship();
	GameObject* createEnemyProjectile(float damage);
	GameObject* createEnemyBullet(float damage);
	GameObject* createExplosion(float damage);
	GameObject* createRay(GameObject* shooter);
	GameObject* createFireBolt(float damage);

	//Base
	GameObject* createMiniHangar(Vector2d position);
	GameObject* createMiniBase(Vector2d position);
	GameObject* createBase(Vector2d position);
	GameObject* createHangar(Vector2d position);
	GameObject* createExtractors(Vector2d position);
	void createWholeBase(Vector2d position);

	//Remote Base
	GameObject* createRemoteMiniHangar();
	GameObject* createRemoteMiniBase();
	GameObject* createRemoteBase();
	GameObject* createRemoteHangar();
	GameObject* createRemoteExtractors();

	// Online
	GameObject* createRemoteBomber();
	GameObject* createRemoteEngineer();
	GameObject* createRemoteBattleship();
	GameObject* createRemoteFighter();
	GameObject* createRemotePlayer(int charcater);
	GameObject* createRemotePlayer();
	GameObject* createRemoteEnemyBasic();
	GameObject* createRemoteRay();
	GameObject* createRemoteTurret();
	GameObject* createRemoteTurretBasic();
	GameObject* createRemoteTurret2Canon();
	GameObject* createRemoteTurretBigCanon();
	GameObject* createRemoteTurret1Canon();
	GameObject* createRemoteTurretGun();
	GameObject* createRemoteProjectile();
	GameObject* createRemoteBullet();
	GameObject* createRemoteMine();
	GameObject* createRemoteMine2();
	GameObject* createRemoteMine3();
	GameObject* createRemoteWall();
	GameObject* createRemoteExplosion();
	GameObject* createRemote(const char* modelPath,  int objectName, bool transparent=false);
	GameObject* createRemoteFury();
	GameObject* createRemoteDefender();
	GameObject* createRemoteEnemyBomber();
	GameObject* createRemoteRepairSystem();
	GameObject* createRemoteSkillHeal();
	GameObject* createRemoteSkillBomb();

	//Inventario
	GameObject* createInventoryTurret(int price);
	GameObject* createInventoryTurret2(int price);
	GameObject* createInventoryTurret2Canon(int price);
	GameObject* createInventoryTurretBigCanon(int price);
	GameObject* createInventoryTurret1Canon(int price);
	GameObject* createInventoryTurretGun(int price);
	GameObject* createInventoryMine(int price);
	GameObject* createInventoryMine2(int price);
	GameObject* createInventoryMine3(int price);
	GameObject* createInventoryWall(int price);
	GameObject* createBuildingTurret(Vector2d position);
	GameObject* createBuildingTurret2(Vector2d position);
	GameObject* createBuildingTurret2Canon(Vector2d position);
	GameObject* createBuildingTurretBigCanon(Vector2d position);
	GameObject* createBuildingTurret1Canon(Vector2d position);
	GameObject* createBuildingTurretGun(Vector2d position);
	GameObject* createBuildingMine(Vector2d position);
	GameObject* createBuildingMine2(Vector2d position);
	GameObject* createBuildingMine3(Vector2d position);
	GameObject* createBuildingWall(Vector2d position);
	GameObject* createBuildingEnergyWall(GameObject* creator1, GameObject* creator2);
	GameObject* createInventoryElement(const char* modelPath,  int objectName,unsigned int price,bool wall, int tag, const char* modelPathStatic = "");
	GameObject* createBuildingElement(const char* modelPath,  int objectName, int tag, Vector2d position, const char* modelPathStatic = "");
	GameObject* createEnergyWall(Vector2d start, Vector2d end);
	GameObject* createMap();

	//Animations
	GameObject* createSpark();
	GameObject* createFire();
	void animationFire(Vector2d position);
	void animationHeal(Vector2d position);
	//Debris
	GameObject* createGenericDebris(const char* meshFileName,Vector2d velocity, Vector2d position);
	void createBasicDebris(Vector2d velocity, Vector2d position);

	//Skills

	GameObject* createSkillBomb(Vector2d pos);
	GameObject* createSkillHeal(Vector2d pos);
	GameObject* createSkillMultimpleShot(Vector2d posPlayer);
	GameObject* createSkillTargetProjectile();



	int getNumberOfEnemies();
	GameObject* getMainPlayer();
	void killAllEnemies();
	void killInventories();
	unsigned int getUpdateTime();
	//Carga modelos.
	void loadModels();



private:
	ID getNextID();

	GameObject* createEnemyCombat();

	ID nextID;
	std::vector<GameObject*> gameObjectList;
	std::vector<GameObject*> tempGameObjectList;
	GameManager* gameManager;

	Clock updateClockGOM;
	int updateTime;
	GameObject* mainPlayer;
};

