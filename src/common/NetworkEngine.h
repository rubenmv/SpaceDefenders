#pragma once

#include "raknet/RakPeerInterface.h"
#include "raknet/ReplicaManager3.h"
#include "Connection.h"
#include <vector>
#include <iostream>
#include <sstream>
#include "raknet/RakString.h"
#include "raknet/PacketLogger.h"
#include "Clock.h"

#include "ServerBrowser.h"
#include "Message.h"

class GameObject;
class ReplicaManager;
class ProcessManager;


struct PlayerInfo
{
	RakNet::RakString name;
	RakNet::RakString character;
	RakNet::RakString type;
	RakNet::RakNetGUID guid;
	unsigned short ping;
	bool ready;
};

/*
	Motor de red. Se encarga de gestionar las conexiones, estados del servidor, lista de jugadores,
	maximo de conexiones a un servidor, etc.
	Hace uso de la librería Raknet.
*/

class NetworkEngine
{
public:

	static const int SERVER_PORT = 60000;
	static const int SERVER_MILLISECONDS_BETWEEN_UPDATES = 100;
	static const int MAX_PLAYERS = 4;

	NetworkEngine();
	~NetworkEngine();

	// Lanza un servidor desde el cliente y se conecta a este
	bool startClientAndAutoconnect();
	// Se conecta a un servido mediante su IP y clave
	bool connectToServer(std::string ip, std::string password = "");
	// Arranca un servidor (no permite desde clientes)
	bool startServer();
	// Actualiza el networkEngine
	void update();
	// Cierra el networkEngine
	void close();
	// Indica si el networkEngine esta arrancado
	bool isStarted() 
	{
		return started;
	}
	// Activa el lobby
	void setLobbyOn(bool lobbyOn)
	{
		this->lobbyOn = lobbyOn;
		updatePingResponse();
	}
	// Indica si el estado es de lobby
	bool isLobby()
	{
		return lobbyOn;
	}
	// Devuelve si esta conectado con el servidor,
	// si hace menos de 100ms que se ha llamado devuelve el ultimo resultado
	bool isConnected();

	//Avisa NetworkEngine de que se ha creado un nuevo GameObject
	void reference(GameObject* gameObject);
	unsigned int getLastPing();
	//Abre los sockets, pero no se conecta a nada
	bool startClient();

	bool tryToReconnect();

	bool waitUntilConnected( unsigned int timeout );

	void searchLanServers();
	std::vector<RakNet::RakString> getLanServers();
	void updatePlayerList(RakNet::Packet * packet);
	void sendToggleReadyToServer();
	void sendToggleCharacterToServer(int character);
	int getCharacter();
	void newPlayerConnected(const RakNet::SystemAddress &systemAddress, RakNet::RakNetGUID raknetGUID);
	void playerDisconnected(const RakNet::SystemAddress &systemAddress, RakNet::RakNetGUID raknetGUID);

	void sendStartGameToServer();
	void removeServerFile();
	void joinServer( int id );

	void giveExperience(float experience);
	void experienceForPlayers(RakNet::Packet * packet);
	void giveMeCharacter();
	void sendCharacterToClient(RakNet::Packet * packet);
	void putTheCharacter(RakNet::Packet * packet);
	GameObject* getMain()
	{
		return mainPlayer;
	}

	void setMain(GameObject* m)
	{
		mainPlayer = m;
	}

	//Envia un mensaje de gameobject con un cliente o servidor de destino y una id que referencia 
	//al gameobject que tiene que recibir el mensaje
	void sendGameObjectMessage(RakNet::RakNetGUID receptor, RakNet::NetworkID replicaID, Message message);
	void onGameObjectMessage( RakNet::Packet * packet );

	
	// ---------------------------------------------------------------------
	// CREAR SERVIDOR
	// Lanza un nuevo proceso servidor
	void launchServer();
	// Cierra el servidor
	void closeServer();
	// Indica si hay un servidor creado en el cliente
	bool isHosting()
	{
		if (serverProcessId > 0) // El servidor ha arrancado
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void messageCreateSkill(unsigned int name, Vector2d pos,Vector2d posPlayer = Vector2d(),RakNet::NetworkID id = -1);
	void createSkill(RakNet::Packet * packet);

	void messageSetWall(int col,int row);
	void messageSetEmpty(int col,int row);

	void setWall(RakNet::Packet * packet);
	void setEmpty(RakNet::Packet * packet);

	
	void messageNextWave(int numWave);
	void nextWave(RakNet::Packet * packet);

	void sendGameOverMessageToClients(int levelRound,Clock wastedTime,int enemiesKilled);
	void gameOverFromServer(RakNet::Packet * packet);
private:

	//Guarda la ip y el password del server para poder reconectarse
	static const char* SERVER_DATA_FILENAME; 

	void start();
	void waitForConnections();
	bool started;
	void updatePingResponse();


	bool canConnected(RakNet::SystemAddress id);
	//Se llama desde el cliente y es un paquete recibido del servidor
	void managePacketFromServer( RakNet::Packet * packet );
	//Se llama desde el servidor y es un paquete recibido del cliente
	void managePacketFromClient( RakNet::Packet * packet );

	void showMessageFromServer( RakNet::Packet * packet );
	void lanServerFound(RakNet::Packet * packet);
	void sendUpdatedPlayerList();
	void updatedPlayerListInServer();

	void updatePlayerReadyInServer(RakNet::Packet * packet);
	void updatePlayerCharacterInServer(RakNet::Packet * packet, RakNet::RakString character);
	void sendMessagePlayerDisconnected(RakNet::SystemAddress systemAddress);
	void reserveSlot( const RakNet::SystemAddress & systemAddress );
	void sendMessagePlayerReconnected(RakNet::SystemAddress systemAddress);
	void sendGameIsStartingToClients();
	void createFileServer();

	void encode( std::string &string);
	void decode( std::string &string);

	void sendMessage( unsigned char messageID );
	RakNet::SystemAddress serverAddress;
	//std::vector<RakNet::SystemAddress> connecteds;
	std::vector<RakNet::SystemAddress> disconnecteds;
	std::vector<PlayerInfo> playerList;
	RakNet::RakPeerInterface *peer;
	RakNet::SocketDescriptor *socketDescriptor;
	RakNet::PacketLogger* messageHandler;	
	RakNet::NetworkIDManager* networkIdManager;
	ReplicaManager* replicaManager;
	bool lobbyOn;

	bool _connectedCached; //Guarda el resultado de getConnectionState durante un tiempo para no pedirlo demasiadas veces
	Clock _connectedCachedClock;

	int aux;
	GameObject* mainPlayer;
	ServerBrowser serverBrowser;
	std::vector<ServerInfo> serverList;


	// ---------------------------------------------------------------------
	// CREAR SERVIDOR
	// Manejador de procesos (para que el cliente pueda lanzar servidor)
	ProcessManager *processManager;
	// ID del proceso al crear un servidor
	unsigned long serverProcessId; 
	// Despues de lanzar un servidor, espera a que arranque conectar automaticamente
	bool waitingForServer;
};

