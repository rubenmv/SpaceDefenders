#include "GameObject.h"
#include "Component.h"
#include "Message.h"
#include "GameManager.h"
#include "AIEngine.h"
#include "NetworkEngine.h"
#include "DebugTools.h"
#include <algorithm>
#include "GameObjectManager.h"
GameObject::GameObject( std::vector<Component*> componentList, Tag tag, Name name, ID id, bool enemyTeam,bool staticObject, Vector2d position) :
	componentList(componentList), tag(tag), name(name), id(id), enemyTeam(enemyTeam), dead(false), readyToBeDeleted(false), staticObject(staticObject), position(position), rotation(0), velocity(Vector2d(0,0))
{
	for (auto component : componentList)
	{
		component->setParent(this);
	}

}


GameObject::~GameObject()
{

	if(WasReferenced() && GameManager::getInstance()->isServer() && tag != MAIN_PLAYER)
	{
		BroadcastDestruction();
	}
	else if(WasReferenced() && tag == MAIN_PLAYER && this == GameManager::getInstance()->getGameObjectManager()->getMainPlayer())
	{
		BroadcastDestruction();
	}
	// Elimina los componentes
	while(!componentList.empty())
	{
		delete componentList.back();
		componentList.pop_back();
	}

	if(this->isStatic())
	{
		GameManager::getInstance()->getAIEngine()->deleteStruct(this->position);
	}
}

void GameObject::update()
{

	if(!dead)
	{
		for (auto component : componentList)
		{
			component->update();
		}
	}
}

void GameObject::awakeComponents()
{
	for (auto component : componentList)
	{
		component->awake();
	}
}



void GameObject::broadcastMessage(Message message)
{
	if(!dead)
	{
	/*	bool isServer = GameManager::getInstance()->isServer();
		if(!isServer)
		{
			DebugTools* debugTools = GameManager::getInstance()->getDebugTools();
			if (debugTools != NULL && debugTools->isShowingDebug())
			{
				GameManager::getInstance()->getDebugTools()->newMessageSend(this, message);
			}
		}*/
		for (auto component : componentList)
		{
			component->onMessage(message);
		}
	}
}

//Raknet

RakNet::RM3ConstructionState GameObject::QueryConstruction (RakNet::Connection_RM3 *destinationConnection, RakNet::ReplicaManager3 *replicaManager3)
{
	bool isServer = GameManager::getInstance()->isServer();// Igual es mejor getType() != SERVER 
	if(tag == MAIN_PLAYER || tag == BULLET || tag == LOCAL) //El main player remote tambien tiene que entrar igual que main player por eso no tiene de tag remote
	{
		return RakNet::Replica3::QueryConstruction_ClientConstruction(destinationConnection, isServer); 
	} else 
	{
		return RakNet::Replica3::QueryConstruction_ServerConstruction(destinationConnection, isServer); 
	}
}
bool GameObject::QueryRemoteConstruction (RakNet::Connection_RM3 *sourceConnection)
{
	bool isServer = GameManager::getInstance()->isServer();
	if(tag == MAIN_PLAYER || tag == BULLET || tag == LOCAL)
	{
		return RakNet::Replica3::QueryRemoteConstruction_ClientConstruction(sourceConnection, isServer); 

	} else 
	{
		return RakNet::Replica3::QueryRemoteConstruction_ServerConstruction(sourceConnection, isServer); 
	}
}

void GameObject::SerializeConstruction(RakNet::BitStream* constructionBitstream, RakNet::Connection_RM3* destinationConnection)
{
	constructionBitstream[0].WriteAlignedBytes((const unsigned char*)&position, sizeof(position));
	constructionBitstream[0].WriteAlignedBytes((const unsigned char*)&rotation, sizeof(rotation));	
	constructionBitstream[0].WriteAlignedBytes((const unsigned char*)&velocity, sizeof(rotation));


}
bool GameObject::DeserializeConstruction (RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection)
{
	constructionBitstream[0].ReadAlignedBytes((unsigned char*)&position, sizeof(position));
	constructionBitstream[0].ReadAlignedBytes((unsigned char*)&rotation, sizeof(rotation));
	constructionBitstream[0].ReadAlignedBytes((unsigned char*)&velocity, sizeof(velocity));


	return true;
}

void GameObject::SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection)
{
	destructionBitstream[0].WriteAlignedBytes((const unsigned char*)&position, sizeof(position));
	destructionBitstream[0].WriteAlignedBytes((const unsigned char*)&rotation, sizeof(rotation));	
}

bool GameObject::DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection)
{
	if(name == N_REMOTE_ENEMY)
	{
		Message message;
		message.type = Message::DEATH;
		broadcastMessage(message);
		kill();
		return false;
	}
	Message message;
	message.type = Message::NEW_SNAPSHOT;
	destructionBitstream[0].ReadAlignedBytes((unsigned char*)&message.target, sizeof(message.target));
	destructionBitstream[0].ReadAlignedBytes((unsigned char*)&message.value, sizeof(message.value));

	//unsigned int ping = GameManager::getInstance()->getNetworkEngine()->getLastPing();
	//unsigned int ping = 0;
	//message.time =  deserializeParameters->timeStamp + ping;
	message.time =  Clock::currentTime();

	bool isServer = GameManager::getInstance()->isServer();
	
	broadcastMessage(message);

	message.type = Message::DEATH_SNAPSHOT;
	//Envia el mensaje de que cuando llegue al ultimo snapshot se muera
	broadcastMessage(message);

	return true;
}

RakNet::RM3ActionOnPopConnection GameObject::QueryActionOnPopConnection (RakNet::Connection_RM3 *droppedConnection) const
{
	bool isServer = GameManager::getInstance()->isServer();
	if(isServer)
	{
		return RakNet::Replica3::QueryActionOnPopConnection_Server(droppedConnection); 
	} else 
	{
		return RakNet::Replica3::QueryActionOnPopConnection_Client(droppedConnection); 
	}
}

RakNet::RM3QuerySerializationResult GameObject::QuerySerialization (RakNet::Connection_RM3 *destinationConnection)
{
	if(tag == BULLET )
	{
		return RakNet::RM3QSR_NEVER_CALL_SERIALIZE;
	}

	bool isServer = GameManager::getInstance()->isServer();

	if(tag == MAIN_PLAYER || tag == LOCAL)
	{
		return RakNet::Replica3::QuerySerialization_ClientSerializable(destinationConnection, isServer); 
	} else 
	{
		return RakNet::Replica3::QuerySerialization_ServerSerializable(destinationConnection, isServer); 
	}
	
}

RakNet::RM3SerializationResult GameObject::Serialize(RakNet::SerializeParameters *serializeParameters)
{
	//WriteAligned() es mas rapido que Write() pero desperdicia bits para que los datos ocupen bytes completos
	serializeParameters->outputBitstream[0].WriteAlignedBytes((const unsigned char*)&position, sizeof(position));
	serializeParameters->outputBitstream[0].WriteAlignedBytes((const unsigned char*)&rotation, sizeof(rotation));	
	serializeParameters->messageTimestamp = Clock::currentTime();
	return RakNet::RM3SR_BROADCAST_IDENTICALLY;
}
void GameObject::Deserialize(RakNet::DeserializeParameters *deserializeParameters)
{
	Message message;
	message.type = Message::NEW_SNAPSHOT;
	deserializeParameters->serializationBitstream[0].ReadAlignedBytes((unsigned char*)&message.target, sizeof(message.target));
	deserializeParameters->serializationBitstream[0].ReadAlignedBytes((unsigned char*)&message.value, sizeof(message.value));

	//unsigned int ping = GameManager::getInstance()->getNetworkEngine()->getLastPing();
	//unsigned int ping = 0;
	//message.time =  deserializeParameters->timeStamp + ping;
	message.time =  Clock::currentTime();

	bool isServer = GameManager::getInstance()->isServer();

	if(isServer)
	{
		position = message.target;
		rotation = message.value;
	} else
	{
		broadcastMessage(message);
	}
}



bool GameObject::deleteIfReady( GameObject* gameObject )
{
	if(gameObject->isDead())
	{
		
		if(gameObject->readyToBeDeleted)
		{
			if(gameObject->isEnemyTeam() && gameObject->getTag() == ENEMY && gameObject->getName() != N_ENEMY_DEFENDER)
			{

				GameManager::getInstance()->getAIEngine()->discountEnemy();
			}
			
			delete gameObject;
			return true;
		}
		
		gameObject->readyToBeDeleted = true;
	}
	return false;
}

bool GameObject::removeDeadOnes( GameObject* gameObject )
{
	if(gameObject->isDead())
	{
		//delete gameObject;
		return true;
	}
	return false;
}

void GameObject::removeDeadOnesFromList(std::list<GameObject*> &gameObjectList )
{
	if(!gameObjectList.empty())
	{
		gameObjectList.erase(std::remove_if(gameObjectList.begin(), gameObjectList.end(), removeDeadOnes), gameObjectList.end());
	}
}

void GameObject::removeDeadOnesFromVector( std::vector<GameObject*> &gameObjectList )
{
	gameObjectList.erase(std::remove_if(gameObjectList.begin(), gameObjectList.end(), removeDeadOnes), gameObjectList.end());
}



const char* GameObject::TAG_STRINGS[] = 
{
	"PLAYER",
	"MAIN_PLAYER",
	"ENEMY",
	"EXPLOSION",
	"SPARK",
	"BUILDING_TURRET_PROJECTILE",
	"BUILDING_TURRET_2_CANON",
	"BUILDING_TURRET_BIG_CANON",
	"BUILDING_TURRET_1_CANON",
	"BUILDING_TURRET_GUN",
	"TURRET",
	"BUILDING_TURRET_BULLET",
	"BUILDING_MINE",
	"BUILDING_MINE2",
	"BUILDING_MINE3",
	"MINE",
	"BUILDING_WALL",
	"WALL",
	"BUILDING_ENERGY_WALL",
	"ENERGY_WALL",
	"TRAP",
	"GROUND",
	"TARGET",
	"PROJECTILE",
	"BULLET",
	"CAMERA",
	"REMOTE",
	"RAY",
	"INVENTORY",
	"BASE",
	"SKILL",
	"GHOST",
	"DEBRS",
	"LOCAL",
	"OTHER"
};

const char* GameObject::NAME_STRINGS[] = 
{
	"MAIN PLAYER",
	"N_BOMBER_PLAYER",
	"N_BATTLESHIP_PLAYER",
	"N_ENGINEER_PLAYER",
	"N_FIGHTER_PLAYER",
	"REMOTE PLAYER",
	"REMOTE ENEMY",
	"REMOTE TURRET",
	"ENEMY BOMBER",
	"ENEMY COMBAT",
	"ENEMY BASIC",
	"ENEMY DEFENDER",
	"EXPLOSION",
	"SPARK",
	"TURRET_PROJECTILE",
	"INVENTORY_TURRET_PROJECTILE",
	"BUILDING_TURRET_PROJECTILE",
	"TURRET_BULLET",
	"INVENTORY_TURRET_BULLET",
	"BUILDING_TURRET_BULLET",
	"TURRET_2_CANON",
	"INVENTORY_TURRET_2_CANON",
	"BUILDING_TURRET_2_CANON",
	"TURRET_BIG_CANON",
	"INVENTORY_TURRET_BIG_CANON",
	"BUILDING_TURRET_BIG_CANON",
	"TURRET_1_CANON",
	"INVENTORY_TURRET_1_CANON",
	"BUILDING_TURRET_1_CANON",
	"TURRET_BIG_CANON",
	"INVENTORY_TURRET_GUN",
	"BUILDING_TURRET_GUN",
	"MINE",
	"INVENTORY_MINE",
	"BUILDING_MINE",
	"MINE2",
	"BUILDING_MINE2",
	"MINE3",
	"BUILDING_MINE3",
	"WALL",
	"ENERGY_WALL",
	"INVENTORY_WALL",
	"BUILDING_WALL",
	"TRAP",
	"GROUND",
	"PROJECTILE",
	"BULLET",
	"BULLET_BOMBER",
	"BULLET_FIGHTER",
	"BULLET_ENGINEER",
	"BULLET_BATTLESHIP",
	"ENEMY_BULLET",
	"TARGET",
	"FREE_CAMERA",
	"MINES_GENERATOR",
	"RAY",
	"MINI_HANGAR",
	"MINI_BASE",
	"HANGAR",
	"EXTRACTORS",
	"BASE",
	"N_OTHER",
	"N_GHOST",
	"N_REPAIR_SYSTEM",
	"N_SKILL_MULTIPLE_SHOT",
	"N_SKILL_BOMB",
	"N_SKILL_HEAL",
	"N_SKILL_TARGET_PROJECTILE"
};

void GameObject::setPosition(float x, float y) //Para cambiar la posicion de un gameobject
{
	Vector2d positionVector = Vector2d(x,y);
	this->position=positionVector;
}

bool GameObject::areEnemies( GameObject* gameObject, GameObject* parent )
{
	return gameObject->isEnemyTeam() != parent->isEnemyTeam();
}

void GameObject::kill()
{
	dead = true;
}

void GameObject::reference()
{
	GameManager::getInstance()->getNetworkEngine()->reference(this);
}

void GameObject::DeallocReplica( RakNet::Connection_RM3 *sourceConnection )
{
	bool isServer = GameManager::getInstance()->isServer();

	if(isServer)
	{
		kill();
	} 
}


bool GameObject::selectPrefEnemy(GameObject* target1,GameObject* target2)
{
	//Accede a basura, hace falta comprobar
	if(target1 != NULL && target2 != NULL)
	{
		if(target1->getName() > target2->getName()) //Funciona mientras en el enum esten ordenados
		{
			return true;
		}

	}


	return false;
}


bool  GameObject::selectPrefTurret(GameObject* target1,GameObject* target2)
{
	//Accede a basura, hace falta comprobar
	if(target1 != NULL && target2 != NULL)
	{
		if(target1->getName() < target2->getName()) //Funciona mientras en el enum esten ordenados
		{
			return true;
		}
	}
	return false;
}

void GameObject::awake()
{
	awakeComponents();
	reference();
	//Quizas en el futuro se lleve a otro sitio
	if(isStatic() && tag != MINE)
	{
		GameManager::getInstance()->getAIEngine()->addStruct(this->position);
	}

}
