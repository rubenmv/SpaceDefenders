#pragma once
#pragma warning( disable : 4804) //Por utilizar el booleano Dead como un numero en isDead()

#include <iostream>
#include <list>
#include <memory>
#include <vector>

#include "Vector2d.h"
#include "raknet/ReplicaManager3.h"
#include "raknet/RakString.h"
#include "ID.h"

class GameObject;
class GameManager;
class Component;
struct Message;


class GameObject : public RakNet::Replica3
{
	
public:

	// Lista de tags con los distintos tipos de gameObjects
	// Cualquier cambio en esta lista tambien tiene que hacerse en TAG_STRINGS (al final del .cpp)
	enum Tag{ 
		PLAYER,
		MAIN_PLAYER,
		ENEMY,
		EXPLOSION,
		PARTICLES,
		BUILDING_TURRET_PROJECTILE,
		BUILDING_TURRET_2_CANON,
		BUILDING_TURRET_BIG_CANON,
		BUILDING_TURRET_1_CANON,
		BUILDING_TURRET_GUN,
		TURRET,
		BUILDING_TURRET_BULLET,
		BUILDING_MINE,
		BUILDING_MINE2,
		BUILDING_MINE3,
		MINE,
		BUILDING_WALL,
		WALL,
		BUILDING_ENERGY_WALL,
		ENERGY_WALL,
		TRAP,
		GROUND,
		TARGET,
		PROJECTILE,
		BULLET,
		CAMERA,
		REMOTE,
		RAY,
		INVENTORY,
		BASE,
		SKILL,
		OTHER,
		GHOST,
		DEBRIS,
		LOCAL, //Se utiliza para cosas que se crean en los clientes y no en el servidor
		TAG_COUNT
	};

	static const char* TAG_STRINGS[];

	friend std::ostream& operator<<(std::ostream& os, enum Tag n)
	{
		if (n >= TAG_COUNT || n < 0) return os << "???";
		return os << TAG_STRINGS[n];
	}
	friend std::wostream& operator<<(std::wostream& os, enum Tag n)
	{
		if (n >= TAG_COUNT || n < 0) return os << "???";
		return os << TAG_STRINGS[n];
	}

	// Lista de nombres de los distintos tipos de gameObjects
	// Cualquier cambio en esta lista tambien tiene que hacerse en NAME_STRINGS (al final del .cpp)
	enum Name{
		N_MAIN_PLAYER,
		N_BOMBER_PLAYER,
		N_BATTLESHIP_PLAYER,
		N_ENGINEER_PLAYER,
		N_FIGHTER_PLAYER,
		N_REMOTE_PLAYER,
		N_REMOTE_ENEMY,
		N_REMOTE_TURRET,
		N_ENEMY_BOMBER,
		N_ENEMY_FURY,
		N_ENEMY_BASIC,
		N_ENEMY_DEFENDER,
		N_EXPLOSION,
		N_SPARK,
		N_TURRET_PROJECTILE,
		N_INVENTORY_TURRET_PROJECTILE,
		N_BUILDING_TURRET_PROJECTILE,
		N_TURRET_BULLET,
		N_INVENTORY_TURRET_BULLET,
		N_BUILDING_TURRET_BULLET,
		N_TURRET_2_CANON,
		N_INVENTORY_TURRET_2_CANON,
		N_BUILDING_TURRET_2_CANON,
		N_TURRET_BIG_CANON,
		N_INVENTORY_TURRET_BIG_CANON,
		N_BUILDING_TURRET_BIG_CANON,
		N_TURRET_1_CANON,
		N_INVENTORY_TURRET_1_CANON,
		N_BUILDING_TURRET_1_CANON,
		N_TURRET_GUN,
		N_INVENTORY_TURRET_GUN,
		N_BUILDING_TURRET_GUN,
		N_MINE,
		N_INVENTORY_MINE,
		N_BUILDING_MINE,
		N_MINE2,
		N_MINE3,
		N_WALL,
		N_ENERGY_WALL,
		N_INVENTORY_WALL,
		N_BUILDING_WALL,
		N_TRAP,
		N_GROUND,
		N_PROJECTILE,
		N_BULLET,
		N_BULLET_BOMBER,
		N_BULLET_FIGHTER,
		N_BULLET_ENGINEER,
		N_BULLET_BATTLESHIP,
		N_ENEMY_BULLET,
		N_TARGET,
		N_FREE_CAMERA,
		N_MINES_GENERATOR,
		N_RAY,
		N_MINI_HANGAR,
		N_MINI_BASE,
		N_HANGAR,
		N_EXTRACTORS,
		N_BASE,
		N_OTHER,
		N_GHOST,
		N_REPAIR_SYSTEM,
		N_SKILL_MULTIPLE_SHOT,
		N_SKILL_BOMB,
		N_SKILL_HEAL,
		N_SKILL_TARGET_PROJECTILE,
		N_SKILL_TARGET_HEAL,
		NAME_COUNT
	};

	static const char* NAME_STRINGS[];

	friend std::ostream& operator<<(std::ostream& os, enum Name n)
	{
		if (n >= NAME_COUNT || n < 0) return os << "???";
		return os << NAME_STRINGS[n];
	}
	friend std::wostream& operator<<(std::wostream& os, enum Name n)
	{
		if (n >= NAME_COUNT || n < 0) return os << "???";
		return os << NAME_STRINGS[n];
	}

	Vector2d position;
	float rotation;
	Vector2d velocity;
	Vector2d acceleration;

	GameObject(std::vector<Component*> componentList, Tag tag, Name name, ID id, bool enemyTeam,bool staticObject=false,Vector2d position=Vector2d());
	virtual ~GameObject();

	bool compareTag(Tag tag)
	{
		return this->tag == tag;
	}
	bool isDead()
	{
		//Normalmente cuando se ha borrado el gameObject, dead suele valer distinto de false(0) o true(1)
		if(dead > 2)
		{
			//Para saber donde se puede poner un breakpoint y mirar la pilla de llamadas
			std::cout << "######### WARNING: ACCEDIENDO A MEMORIA BORRADA DE UN GAMEOBJECT #########\n";
		}
		return dead;
	}

	bool isEnemyTeam()
	{
		return enemyTeam;
	}

	//Indica si el objeto esta fijo siempre en el mundo como las torretas o las minas
	bool isStatic()
	{
		return staticObject;
	}

	bool isPlayer()
	{
		return(tag==MAIN_PLAYER);
	}


	void kill();

	virtual Name getName() 
	{
		return name;
	}

	ID getId()
	{
		//return GetNetworkID();
		return id;
	}

	bool readyToBeDeleted;

	virtual void update();

	void broadcastMessage(Message message);

	//Funciones de la interfaz Replica3 de RakNet

	virtual void WriteAllocationID(RakNet::Connection_RM3 *destinationConnection, RakNet::BitStream* allocationIdBitstream) const 
	{
		allocationIdBitstream->Write((unsigned char)name);
	}
	virtual RakNet::RM3ConstructionState QueryConstruction (RakNet::Connection_RM3 *destinationConnection, RakNet::ReplicaManager3 *replicaManager3);
	virtual bool QueryRemoteConstruction (RakNet::Connection_RM3 *sourceConnection);
	
	virtual void SerializeConstruction(RakNet::BitStream* constructionBitstream, RakNet::Connection_RM3* destinationConnection);
	virtual bool DeserializeConstruction (RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection);

	virtual void SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection);
	virtual bool DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection);

	virtual RakNet::RM3ActionOnPopConnection QueryActionOnPopConnection (RakNet::Connection_RM3 *droppedConnection) const;

	virtual void DeallocReplica(RakNet::Connection_RM3 *sourceConnection);

	virtual RakNet::RM3QuerySerializationResult QuerySerialization (RakNet::Connection_RM3 *destinationConnection);

	virtual RakNet::RM3SerializationResult Serialize(RakNet::SerializeParameters *serializeParameters);
	virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);

	//virtual void SerializeConstructionRequestRejected(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *requestingConnection);
	//virtual void DeserializeConstructionRequestRejected(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *rejectingConnection);

	virtual void OnPoppedConnection(RakNet::Connection_RM3 *droppedConnection){};
	Tag getTag(){return tag;};

	//Se utiliza en std::remove_if en GameObjectManager.update()
	static bool deleteIfReady(GameObject* gameObject);

	//Se utiliza en std::remove_if de removeDeadOnesFrom...
	static bool removeDeadOnes(GameObject* gameObject);

	// Quita de la lista los muertos pero no los borra
	static void removeDeadOnesFromList(std::list<GameObject*> &gameObjectList);
	// Quita del vector los muertos pero no los borra
	static void removeDeadOnesFromVector(std::vector<GameObject*> &gameObjectList);

	//Devuelve true si son de equipos contrarios y false si pertenecen al mismo equipo
	static bool areEnemies( GameObject* gameObject, GameObject* parent ); 

	void setPosition(float,float);

	void reference();

	//Indica la preferencia que se usa para ordenar la lista de objetivos de los enemigos
	static bool selectPrefEnemy(GameObject* target1,GameObject* target2);
	//Indica la preferencia que se usa para ordenar la lista de objetivos de las torretas
	static bool  GameObject::selectPrefTurret(GameObject* target1,GameObject* target2);
	
	void awake();

protected:
	Tag tag;
	bool dead; // Muerto, puede pasar a ser borrado
	bool enemyTeam; //Indica si pertenece o no a los enemigos
	std::vector<Component*> componentList;
	Name name;
	ID id;
	void awakeComponents();
	bool staticObject;
};


