#pragma once
#include <iostream>

//Una vez terminado, pasar includes al cpp
#include "raknet/ReplicaManager3.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/BitStream.h"
#include "raknet/MessageIdentifiers.h"
#include "raknet/ReplicaManager3.h"
#include "raknet/NetworkIDManager.h"
#include "raknet/RakSleep.h"
#include "raknet/RakString.h"
#include "raknet/GetTime.h"
#include "raknet/SocketLayer.h"
#include "raknet/Gets.h"



class Connection : public RakNet::Connection_RM3 
{
public:
	Connection(const RakNet::SystemAddress &systemAddress, RakNet::RakNetGUID guid) : RakNet::Connection_RM3(systemAddress, guid){}

	virtual ~Connection(void){}
	
	// Makes all messages between ID_REPLICA_MANAGER_DOWNLOAD_STARTED and ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE arrive in one tick
	//bool QueryGroupDownloadMessages(void) const {return true;}

	virtual RakNet::Replica3* AllocReplica(RakNet::BitStream* allocationId, RakNet::ReplicaManager3* replicaManager3);

};

