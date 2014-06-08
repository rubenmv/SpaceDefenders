#include "ReplicaManager.h"

#include "NetworkEngine.h"
#include "GameManager.h"
#include "StateMenuMain.h"


RakNet::Connection_RM3* ReplicaManager::AllocConnection( const RakNet::SystemAddress &systemAddress, RakNet::RakNetGUID rakNetGUID ) const
{
	if(GameManager::getInstance()->isServer())
	{
		GameManager::getInstance()->getNetworkEngine()->newPlayerConnected(systemAddress, rakNetGUID);
	}

	return new Connection(systemAddress,rakNetGUID);
}

void ReplicaManager::DeallocConnection( RakNet::Connection_RM3 *connection ) const
{
	if(GameManager::getInstance()->isServer())
	{
		GameManager::getInstance()->getNetworkEngine()->playerDisconnected(connection->GetSystemAddress(), connection->GetRakNetGUID());
	} else
	{
		//GameManager::getInstance()->changeState(StateMenuMain::getInstance());
	}
	delete connection;
}

GameObject* ReplicaManager::getGameObjectByNetworkID( RakNet::NetworkID id )
{
	return (GameObject*) GetReplicaByNetworkID(id, 0);
}
