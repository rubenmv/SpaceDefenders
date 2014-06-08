#pragma once

#include "raknet/ReplicaManager3.h"
#include <iostream>
#include "Connection.h"

class GameObject;

class ReplicaManager : public RakNet::ReplicaManager3
{
public:
	virtual RakNet::Connection_RM3* AllocConnection(const RakNet::SystemAddress &systemAddress, RakNet::RakNetGUID rakNetGUID) const;
	virtual void DeallocConnection(RakNet::Connection_RM3 *connection) const;
	virtual GameObject* getGameObjectByNetworkID( RakNet::NetworkID id );
};
