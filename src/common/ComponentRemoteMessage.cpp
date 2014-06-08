#include "ComponentRemoteMessage.h"
#include "GameManager.h"
#include "NetworkEngine.h"
#include "GameObject.h"


void ComponentRemoteMessage::onMessage(const Message& message)
{
	if(messageType == message.type)
	{
		sendMessage(message);
	}
}

void ComponentRemoteMessage::sendMessage(Message message)
{
	/*if(parent->WasReferenced())
	{
		std::cout <<"Error: Enviando un mensaje en ComponentRemoteMessage de un objeto sin referenciar\n";
		return;
	}*/

	if(message.gameObject != NULL)
	{
		std::cout <<"Warning: Enviando un mensaje con un puntero a GameObject por ComponentRemoteMessage\n";
		//El puntero no se puede enviar por red, si hace falta en alguno hay maneras de solucionarlo, avisad y se mira
	}
	message.gameObject = NULL;

	RakNet::RakNetGUID guid = RakNet::UNASSIGNED_RAKNET_GUID;
	guid = parent->GetCreatingSystemGUID();

	//Si el creador es el que envia el mensaje entonces se envia a todos y si no solo se envia al creador
	/*if(parent->WasReferenced() && parent->replicaManager->GetMyGUIDUnified() != parent->GetCreatingSystemGUID())
	{
		guid = parent->GetCreatingSystemGUID();
	}*/
	GameManager::getInstance()->getNetworkEngine()->sendGameObjectMessage(guid, parent->GetNetworkID(), message);

}
