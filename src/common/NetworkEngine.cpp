#include "NetworkEngine.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "GameManager.h"
#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "raknet/NetworkIDManager.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/GetTime.h"
#include "raknet/RakSleep.h"
#include "raknet/MessageIdentifiers.h"
#include "raknet/BitStream.h"
#include "NetworkMessages.h"
#include "ReplicaManager.h"
#include "StateMenuMain.h"
#include "tests/StateClientInGame.h"
#include "ServerBrowser.h"
#include "ProcessManager.h"
#include "StateServerInGame.h"
#include "CollisionManager.h"
#include "StateGameOver.h"
const char* NetworkEngine::SERVER_DATA_FILENAME = "../data/server.dat"; 


NetworkEngine::NetworkEngine()
{
	started = false;
	lobbyOn = true;
	aux = -1;
	mainPlayer = NULL;
	serverAddress = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
	_connectedCachedClock.start();
	_connectedCached = false;
	processManager = new ProcessManager();
	serverProcessId = 0; // el PID 0 es del sistema idle de windows
	waitingForServer = false;
}

NetworkEngine::~NetworkEngine()
{
	close();

	// Elimina el servidor
	GameManager* gameManager = GameManager::getInstance();
	if ( !gameManager->isServer() && serverProcessId != 0 ) // Si fuera un servidor no se va a cerrar si mismo...
	{
		processManager->killProcess(serverProcessId);
	}
	delete processManager;
}

std::vector<RakNet::RakString> NetworkEngine::getLanServers()
{
	//En el server browser se utiliza getLanServersList()

	std::vector<RakNet::RakString> serverList;
	RakNet::Packet *packet;

	peer->Ping("255.255.255.255", SERVER_PORT, false);

	RakNet::TimeMS timeout = RakNet::GetTimeMS() + 1000;

	while(RakNet::GetTimeMS() < timeout) // Bloquea durante 1s el juego
	{
		packet = peer->Receive();
		
		while(packet != 0) //Ha llegado un paquete
		{
			if (packet->data[0]==ID_UNCONNECTED_PONG)
			{
				RakNet::TimeMS time;
				RakNet::BitStream bsIn(packet->data,packet->length,false);
				bsIn.IgnoreBytes(1);
				bsIn.Read(time);

				//char playerData;
				//bsIn.Read(playerData);
				std::cout<<peer->NumberOfConnections()<<std::endl;
				//std::cout << "Server: " << packet->systemAddress.ToString() << ", players: "<< playerData << "/4, ping: "<< RakNet::GetTimeMS() - time << "ms\n";
				std::cout << "Server: " << packet->systemAddress.ToString() << ", ping: "<< RakNet::GetTimeMS() - time << "ms\n";
				serverList.push_back((RakNet::RakString) packet->systemAddress.ToString(false));
				timeout -= 500; // Si ha encontrado un servidor termina antes
			}
			peer->DeallocatePacket(packet);
			packet = peer->Receive();
		}		
		RakSleep(30);
	}

	return serverList;
}

void NetworkEngine::start()
{
	peer = RakNet::RakPeerInterface::GetInstance();
	
	socketDescriptor = new RakNet::SocketDescriptor(0,0);
	socketDescriptor->socketFamily=AF_INET;

	replicaManager = new ReplicaManager();
	messageHandler = new RakNet::PacketLogger();
	networkIdManager = new RakNet::NetworkIDManager();
	
	peer->AttachPlugin(replicaManager);
	//peer->AttachPlugin(messageHandler);
	
	messageHandler->SetPrintAcks(false);
	messageHandler->SetLogDirectMessages(false);

	replicaManager->SetDefaultPacketReliability(RELIABLE_ORDERED);
	//replicaManager->SetDefaultPacketReliability(UNRELIABLE);
	replicaManager->SetNetworkIDManager(networkIdManager);
	replicaManager->SetAutoSerializeInterval(SERVER_MILLISECONDS_BETWEEN_UPDATES);

	peer->SetTimeoutTime(5000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
	
}

/*bool NetworkEngine::startClientAndAutoconnect()
{
	bool startResult = startClient();

	if(!startResult)
	{
		return false; //Ha fallado al arrancar la conexion
	}
	std::vector<ServerInfo> serverList = getLanServersList();

	if(serverList.size() == 0)
	{
		std::cout << "ERROR :: No se ha encontrado ningun server\n";
		started = false;
		return false;
	} 

	std::string serverIP = serverList.front().ip.ToString();
	
	bool connectResult = connectToServer(serverIP);

	return connectResult;
}*/

// Antes se llamaba startClientBrowser
bool NetworkEngine::connectToServer(std::string ip, std::string password )
{
	GameManager* gameManager = GameManager::getInstance();

	if(gameManager->isServer())
	{
		std::cout << "ERROR: Se esta intentando connectarse a un server desde otro server\n";
		return false;
	}

	if(!started)
	{
		std::cout << "ERROR: El cliente necesita haber arrancado para poder conectarse\n";
		return false;
	}

	std::cout << "Connecting to " << ip << ":" << SERVER_PORT << "\n"; 

	RakNet::ConnectionAttemptResult result;

	if(password.length() > 0) //El server tiene password
	{
		result = peer->Connect(ip.c_str(), SERVER_PORT, password.c_str(), password.length());
	} else
	{
		result = peer->Connect(ip.c_str(), SERVER_PORT, 0, 0);
	}

	if(result == RakNet::CONNECTION_ATTEMPT_STARTED)
	{
		replicaManager->SetAutoSerializeInterval(SERVER_MILLISECONDS_BETWEEN_UPDATES);

		std::cout << "Client started with GUID: " << peer->GetMyGUID().ToString() << "\n";

		serverAddress = RakNet::SystemAddress(ip.c_str(), SERVER_PORT);
	} else
	{
		std::cout << "ERROR "<< result <<": El cliente no ha podido conectarse\n";
		return false;
	}
	
	
	return true;
}


void NetworkEngine::searchLanServers()
{
	serverBrowser.clear();
	peer->Ping("255.255.255.255", SERVER_PORT, false);
}

// Abre los sockets, pero no se conecta a nada
// Antes se llamaba StartServerBrowser
bool NetworkEngine::startClient()
{
	GameManager* gameManager = GameManager::getInstance();

	if(gameManager->isServer())
	{
		std::cout << "ERROR: Se esta intentando lanzar un cliente desde el server\n";
		return false;
	}

	if(started)
	{
		std::cout << "ERROR: El cliente ya estaba lanzado\n";
		return false;
	}

	start();

	socketDescriptor->port = 0;

	RakNet::StartupResult startupResult = peer->Startup(1, socketDescriptor, 1);

	if(startupResult != RakNet::RAKNET_STARTED)
	{
		std::cout << "ERROR: No se ha podido lanzar el client (StartupResult)\n";
		return false;
	}

	started = true;
	return true;
}

bool NetworkEngine::startServer()
{
	GameManager* gameManager = GameManager::getInstance();

	if(!gameManager->isServer())
	{
		std::cout << "ERROR: Se esta intentando lanzar el server desde un cliente\n";
		return false;
	}

	if(started)
	{
		std::cout << "ERROR: El server ya estaba lanzado\n";
		return false;
	}
	
	//start();

	peer = RakNet::RakPeerInterface::GetInstance();
	
	socketDescriptor = new RakNet::SocketDescriptor(0,0);
	socketDescriptor->socketFamily=AF_INET;

	replicaManager = new ReplicaManager();
	messageHandler = new RakNet::PacketLogger();
	networkIdManager = new RakNet::NetworkIDManager();
	
	socketDescriptor->port = SERVER_PORT;

	RakNet::StartupResult startupResult = peer->Startup(MAX_PLAYERS, socketDescriptor, 1);

	

	if(startupResult != RakNet::RAKNET_STARTED)
	{
		std::cout << "ERROR: No se ha podido lanzar el server (StartupResult)\n";
		return false;
	}

	peer->AttachPlugin(replicaManager);
	replicaManager->SetNetworkIDManager(networkIdManager);
	peer->SetMaximumIncomingConnections(MAX_PLAYERS);

	//peer->AttachPlugin(messageHandler);
	messageHandler->SetPrintAcks(false);
	replicaManager->SetAutoSerializeInterval(SERVER_MILLISECONDS_BETWEEN_UPDATES);

	//peer->SetTimeoutTime(5000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
	
	std::cout << "Starting server in port " << SERVER_PORT << "\n"; 

	std::cout << "Server started with GUID: " << peer->GetMyGUID().ToString() << "\n";
	started = true;

	//replicaManager->SetAutoManageConnections(false,false);

	//replicaManager->Reference(GameManager::getInstance()->getGameObjectManager()->createEnemy());

	return true;
}

bool NetworkEngine::canConnected(RakNet::SystemAddress id)
{
	RakNet::SystemAddress connecteds[MAX_PLAYERS];
	unsigned short size = MAX_PLAYERS;
	//unsigned short pos = peer->NumberOfConnections();
	//peer->GetConnectionList(connecteds,&pos);
	peer->GetConnectionList((RakNet::SystemAddress*) &connecteds,&size);

	if(disconnecteds.size() > 0)
	{
		std::vector<RakNet::SystemAddress>::iterator it = disconnecteds.begin();
		do
		{
			if(id.EqualsExcludingPort(*it))
			{
				disconnecteds.erase(it);
				return true;
			}
			it++;
		}while(it != disconnecteds.end());
	}

	for(int i=0;i<MAX_PLAYERS;i++)
	{
		if(connecteds[i].EqualsExcludingPort(id))
		{
			return false;
		}
	}

	return false;
}

void NetworkEngine::update()
{
	if(started)
	{
		RakNet::Packet *packet;
		packet = peer->Receive();
		GameManager* gameManager = GameManager::getInstance();

		while(packet)
		{
			if(gameManager->isServer())
			{
				managePacketFromClient(packet);
			} else
			{
				managePacketFromServer(packet);
			}

			if(started)
			{
				peer->DeallocatePacket(packet);
				packet = peer->Receive();
			}
			else
			{
				return;
			}
		}

		// Si el cliente ha lanzado un servidor y esta esperando a que se cree
		if ( waitingForServer )
		{
			// Actualiza la lista
			gameManager->getGraphicsEngine()->clearServerTable();
			searchLanServers(); // Si hay uno nuevo se autoconecta
		}
	} else
	{
		std::cout << "ERROR: Actualizando NetworkEngine sin haberse lanzado\n";
		exit(-1);
	}
}

void NetworkEngine::close()
{
	mainPlayer = NULL;

	if(started)
	{
		started = false;
		peer->Shutdown(100,0);
		RakNet::RakPeerInterface::DestroyInstance(peer);
	}
	started = false;
}



void NetworkEngine::reference(GameObject* gameObject)
{
	if(gameObject != NULL && started)
	{
		replicaManager->Reference((RakNet::Replica3*)gameObject);
	}
}

unsigned int NetworkEngine::getLastPing()
{	
	return peer->GetLastPing(peer->GetGUIDFromIndex(0));
}

void NetworkEngine::waitForConnections()
{
	RakNet::Packet *packet;
	packet = peer->Receive();
	while(packet)
	{
		peer->DeallocatePacket(packet);
		packet = peer->Receive();
	}
}

void NetworkEngine::updatePingResponse()
{
	RakNet::BitStream data;

	if(isLobby())
	{
		data.Write1();
		unsigned char numPlayers = (unsigned char) peer->NumberOfConnections();
		data.Write(numPlayers);
		std::cout << "Total players: " << (unsigned int) numPlayers << std::endl;
		peer->SetOfflinePingResponse((char *)data.GetData(), data.GetNumberOfBytesUsed());
	} else
	{
		data.Write0();
	}

	peer->SetOfflinePingResponse((char *)data.GetData(), data.GetNumberOfBytesUsed());
}

// Un nuevo jugador accede
// Solo se llama desde el servidor
void NetworkEngine::newPlayerConnected( const RakNet::SystemAddress &systemAddress, RakNet::RakNetGUID raknetGUID )
{
	std::cout << "New player connected\n";

	if(isLobby())
	{
		//Se actualiza el numero de jugadores que devuelve al buscar servidores
		updatePingResponse();
		//Se actualiza la lista
		updatedPlayerListInServer();
		//Se envia a los players la lista actualizada de players
		sendUpdatedPlayerList();

	} else //La partida esta empezada
	{
		if(canConnected(systemAddress))
		{
			//Se envia un string de que el player X se ha reconectado
			sendMessagePlayerReconnected(systemAddress);
		}
		else
		{
			//std::cout<<"Tirarle\n";
			//peer->CloseConnection(systemAddress, true);
		}
	}
}

//Solo se llama desde el servidor
void NetworkEngine::playerDisconnected( const RakNet::SystemAddress &systemAddress, RakNet::RakNetGUID raknetGUID )
{
	std::cout << "Jugador desconectado\n";
	if(isLobby())
	{
		//Se actualiza el numero de jugadores que devuelve al buscar servidores
		updatePingResponse();
		for(std::size_t i = 0; i < playerList.size(); i++)
		{
			if(playerList[i].guid == raknetGUID)
			{
				playerList.erase(playerList.begin()+i);
				break;
			}
		}
		//Se actualiza la lista
		updatedPlayerListInServer();
		//Se envia a los players la lista actualizada de players
		sendUpdatedPlayerList();

	} else //La partida esta empezada
	{
		//Se reserva un sitio al player desconectado por si quiere reconectarse
		//reserveSlot(systemAddress);
		//Se enviar un mensaje a los players de que el player X se ha desconectado
		sendMessagePlayerDisconnected(systemAddress);
	}
}

//Se llama desde el cliente y es un paquete recibido del servidor
void NetworkEngine::managePacketFromServer( RakNet::Packet * packet )
{
	switch (packet->data[0])
	{
	//Mensajes de Raknet
	case ID_DISCONNECTION_NOTIFICATION:
		removeServerFile();
		close();
	case ID_CONNECTION_LOST:
		close();
		GameManager::getInstance()->changeState(StateMenuMain::getInstance());
		break;
	case ID_INVALID_PASSWORD:
		std::cout << "ERROR: El servidor al que intentabas reconectarte no es el mismo del que hay ahora\n";
		removeServerFile();
		close();
		GameManager::getInstance()->changeState(StateMenuMain::getInstance());
	case ID_UNCONNECTED_PONG:
		lanServerFound(packet);
		break;

	//Nuestros mensajes
	case ID_SERVER_PLAYER_LIST:
		//Recoger jugadores 
		//El problema es como avisa desde aquí al estado para que actualice la GUI
		updatePlayerList(packet);
		break;
	case ID_SERVER_STRING_MESSAGE:
		showMessageFromServer(packet);

		break;
	case ID_SERVER_GAME_STARTING:
		createFileServer();
		if(!waitingForServer) //Parche para que se conecte sin cambiar de estado para Test Online
		{
			GameManager::getInstance()->changeState(StateClientInGame::getInstance());
		}
		waitingForServer = false;
		break;

	case ID_SERVER_YOUR_CHARACTER:
		putTheCharacter(packet);
		break;
	case ID_SERVER_GIVE_EXPERIENCE:
		experienceForPlayers(packet);
		break;
	case ID_GLOBAL_GAMEOBJECT_MESSAGE:
		onGameObjectMessage(packet);
		break;
	case ID_SET_EMPTY:
		setEmpty(packet);
		break;
	case ID_SET_WALL:
		setWall(packet);
		break;
	case ID_GAME_OVER:
		gameOverFromServer(packet);
		break;
	case ID_NEXT_WAVE:
		nextWave(packet);
		break;
	}
}

//Se llama desde el servidor y es un paquete recibido del cliente
void NetworkEngine::managePacketFromClient( RakNet::Packet * packet )
{
	//std::cout<<(int)packet->data[0]<<std::endl;
	switch (packet->data[0])
	{
	case ID_CLIENT_START_GAME:
		//Se podria comprobar que lo ha enviado el primer player conectado
		//Solo el deberia empezar la partida. Y si no es el ignorar el mensaje
		//Avisar a los clientes que la partida va a empezar
		sendGameIsStartingToClients();
		//Tiene que cambiar de estado al de la partida
		break;
	case ID_CLIENT_PLAYER_TOGGLE_READY:
		updatePlayerReadyInServer(packet);
		break;
	case ID_CLIENT_PLAYER_TOGGLE_BOMBER:
		updatePlayerCharacterInServer(packet, RakNet::RakString("Bomber"));
		break;
	case ID_CLIENT_PLAYER_TOGGLE_ENGINEER:
		updatePlayerCharacterInServer(packet, RakNet::RakString("Engineer"));
		break;
	case ID_CLIENT_PLAYER_TOGGLE_BATTLESHIP:
		updatePlayerCharacterInServer(packet, RakNet::RakString("Battleship"));
		break;
	case ID_CLIENT_PLAYER_TOGGLE_FIGHTER:
		updatePlayerCharacterInServer(packet, RakNet::RakString("Fighter"));
		break;
	case ID_GET_CHARACTER:
		sendCharacterToClient(packet);
		break;
	case ID_GLOBAL_GAMEOBJECT_MESSAGE:
		onGameObjectMessage(packet);
		break;
	case ID_CREATE_SKILL:
		createSkill(packet);
		break;
	}
}

void NetworkEngine::updatePlayerList(RakNet::Packet * packet)
{
	RakNet::BitStream bsIn(packet->data,packet->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID)); //En otros sitios esta como bsIn.IgnoreBytes(1), es lo mismo
	unsigned short size;

	bsIn.Read(size);

	/*std::vector<PlayerInfo> playerListTemp;
	playerListTemp.push_back(playerList);
	*/
	playerList.clear();
	for(int i=0;i< size;i++)
	{
		PlayerInfo player;

		//RakNet::SystemAddress ip;
		bsIn.Read(player.guid);
		//player.guid = ip;

		RakNet::RakString string;
		bsIn.Read(string);
		player.name = string;

		unsigned short ping;
		bsIn.Read(ping);
		player.ping = ping;

		bool ready = false;
		bsIn.Read(ready);

		player.ready = ready;
		
		RakNet::RakString string2;
		bsIn.Read(string2);
		player.character = string2;

		playerList.push_back(player);
		
	}


	GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	graphicsEngine->updateTablePlayers(playerList);

	unsigned int readys = 0;
	for(std::size_t i = 0; i < playerList.size(); i++)
	{
		if(playerList[i].ready && !playerList[i].character.IsEmpty())
		{
			readys++;
		}
	}
	if(readys >= playerList.size())
	{
		graphicsEngine->putBegin(true);
	} 
	else
	{
		graphicsEngine->putBegin(false);
	}
}



void NetworkEngine::sendUpdatedPlayerList()
{
	//Envia un ID_SERVER_PLAYER_LIST
	RakNet::BitStream bitStream;
	bitStream.Write((unsigned char)ID_SERVER_PLAYER_LIST);


	//Escribir el numero total de jugadores
	unsigned short size;
	size = playerList.size();
	bitStream.Write(size);
	
	//Escribir nombre del jugador, nave y ping.
	for(std::size_t i=0;i< playerList.size();i++)
	{
		bitStream.Write(playerList[i].guid);		
		bitStream.Write(playerList[i].name);
		bitStream.Write(playerList[i].ping);
		bitStream.Write(playerList[i].ready);
		bitStream.Write(playerList[i].character);
	}

	peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}


//Actualizar listaplayers solo servidor
void NetworkEngine::updatedPlayerListInServer()
{
	//recogemos lista de conexiones;
	RakNet::SystemAddress connections[MAX_PLAYERS];
	unsigned short size = MAX_PLAYERS;
	peer->GetConnectionList((RakNet::SystemAddress*) &connections,&size);
	//Limpiar lista

	//playerList.clear();

	//Escribir nombre del jugador, nave y ping.
	for(int i=0;i< size;i++)
	{
		bool found = false;
		for(std::size_t k = 0; k < playerList.size(); k++)
		{
			if(playerList[k].guid == peer->GetGuidFromSystemAddress(connections[i]))
			{
				found = true;
				playerList[k].ping = (unsigned short) peer->GetAveragePing(connections[i]);
				break;
			}
		}
		if(!found)
		{
			PlayerInfo player;
			player.guid = peer->GetGuidFromSystemAddress(connections[i]); 
			RakNet::RakString string("Player %d",(i+1));
			player.name = string;
			player.ping = (unsigned short) peer->GetAveragePing(connections[i]);
			player.ready = false;
			playerList.push_back(player);
		}
	}
	
	
}

//Solo desde el servidor, recibe una direccion a la que debe cambiar el estado a listo o a no listo
void NetworkEngine::updatePlayerReadyInServer(RakNet::Packet * packet)
{
	for(std::size_t i=0;i< playerList.size();i++)
	{
		if(playerList[i].guid  == packet->guid )
		{
			playerList[i].ready = !playerList[i].ready;
		}
	}
	//Se envia a los players la lista actualizada de players
	sendUpdatedPlayerList();
}

void NetworkEngine::updatePlayerCharacterInServer(RakNet::Packet * packet, RakNet::RakString character)
{
	for(std::size_t i=0;i< playerList.size();i++)
	{
		if(playerList[i].guid  == packet->guid )
		{
			playerList[i].character = character;
		}
	}
	//Se envia a los players la lista actualizada de players
	sendUpdatedPlayerList();
}


void NetworkEngine::sendMessagePlayerDisconnected(RakNet::SystemAddress systemAddress)
{
	//Envia un ID_SERVER_MESSAGE con un RakNet::String;
	RakNet::BitStream bitStream;
	RakNet::RakString string("Player %s has disconnected from the game\n", systemAddress.ToString());
	bitStream.Write((unsigned char)ID_SERVER_STRING_MESSAGE);
	bitStream.Write(string);
	peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

//Se llama desde playerDisconnected()
//Mete el player en la lista de desconectados
void NetworkEngine::reserveSlot( const RakNet::SystemAddress & systemAddress )
{
	disconnecteds.push_back(systemAddress);
}

void NetworkEngine::sendMessagePlayerReconnected(RakNet::SystemAddress systemAddress)
{
	//Envia un ID_SERVER_MESSAGE con un RakNet::String;
	RakNet::BitStream bitStream;
	RakNet::RakString string("Player %s has reconnected to the game\n", systemAddress.ToString());
	bitStream.Write((unsigned char)ID_SERVER_STRING_MESSAGE);
	bitStream.Write(string);
	peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkEngine::sendStartGameToServer()
{
	sendMessage(ID_CLIENT_START_GAME);
}

void NetworkEngine::sendGameIsStartingToClients()
{
	unsigned int readys = 0;
	for(std::size_t i = 0; i < playerList.size(); i++)
	{
		if(playerList[i].ready && !playerList[i].character.IsEmpty())
		{
			readys++;
		}
	}
	if(readys >= playerList.size())
	{

		const char* password = peer->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString();
		peer->SetIncomingPassword(password, strlen(password));

		sendMessage(ID_SERVER_GAME_STARTING);
		GameManager::getInstance()->changeState(StateServerInGame::getInstance());
		return;
	} else
	{
		std::cout << "No estan todos los jugadores listos\n";
	}
}

void NetworkEngine::showMessageFromServer(RakNet::Packet * packet )
{
	RakNet::RakString message;
	RakNet::BitStream bsIn(packet->data,packet->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID)); //En otros sitios esta como bsIn.IgnoreBytes(1), es lo mismo
	bsIn.Read(message);

	std::string string = message.C_String();
	std::wstring text = std::wstring(string.begin(), string.end());
	const wchar_t *textWchar = text.c_str();
	//En un futuro aparecera en el chat o donde salgan los mensajes pero no estos no son de Debug
	GameManager::getInstance()->getGraphicsEngine()->setDebugText(textWchar);
}

//Devuelve si esta conectado con el servidor, si hace menos de 100ms que se ha llamado devuelve el ultimo resultado
bool NetworkEngine::isConnected()
{
	if(serverAddress == RakNet::UNASSIGNED_SYSTEM_ADDRESS)
	{
		return false;
	}

	//Se hace porque el peer->GetConnectionState(address) no se puede llamar muchas veces seguidas
	if(_connectedCachedClock.getElapsedTime() < 200)
	{
		//_connectedCachedClock.restart();
		return _connectedCached;
	}

	RakNet::AddressOrGUID address(serverAddress);
	RakNet::ConnectionState connectionState = peer->GetConnectionState(address);

	_connectedCached = peer->GetConnectionState(address) == RakNet::IS_CONNECTED;
	_connectedCachedClock.restart();

	return _connectedCached;
}

void NetworkEngine::sendMessage( unsigned char messageID )
{
	RakNet::BitStream data;
	data.Write(messageID);
	peer->Send(&data, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

//Manda un mensaje diciendo si esta lito o no;
void NetworkEngine::sendToggleReadyToServer()
{
	sendMessage(ID_CLIENT_PLAYER_TOGGLE_READY);
}

void NetworkEngine::sendToggleCharacterToServer(int character)
{
	if(character == 0)
	{
		sendMessage(ID_CLIENT_PLAYER_TOGGLE_BATTLESHIP);
	}
	else if(character == 1)
	{
		sendMessage(ID_CLIENT_PLAYER_TOGGLE_FIGHTER);
	}
	else if(character == 2)
	{
		sendMessage(ID_CLIENT_PLAYER_TOGGLE_ENGINEER);
	}
	else if(character == 3)
	{
		sendMessage(ID_CLIENT_PLAYER_TOGGLE_BOMBER);
	}
}

//pregunta a los clientes si estan listos o no

void NetworkEngine::createFileServer()
{
   // Crea un fichero de salida
   std::ofstream fs(SERVER_DATA_FILENAME, std::ios::out); 
   std::string cad = serverAddress.ToString();
  
   encode(cad);
   // Enviamos una cadena al fichero de salida:
   if(aux == -1)
   {
	   for(size_t i=0;i<playerList.size();i++)
	   {
	   if(playerList[i].guid == peer->GetMyGUID())
	   {
			   fs << i;
	   }
   }
   }
   else
   {
	   fs << aux;
   }
   fs << std::endl;
   fs << cad;
   fs << std::endl;
   fs << peer->GetGuidFromSystemAddress(serverAddress).ToString();

   // Cerrar el fichero, 
   // para luego poder abrirlo para lectura:
   fs.close();
}

//Codifica sumando 3 al valor ascii de cada uno de los caracteres
void NetworkEngine::encode( std::string &string )
{
	for(std::size_t i = 0; i < string.length(); i++)
	{
		string[i] += 3;
	}
}

void NetworkEngine::decode( std::string &string )
{
	for(std::size_t i = 0; i < string.length(); i++)
	{
		string[i] -= 3;				
	}
}

void NetworkEngine::removeServerFile()
{
	std::remove(SERVER_DATA_FILENAME);
}

bool NetworkEngine::tryToReconnect()
{
	std::ifstream entrada(SERVER_DATA_FILENAME, std::ifstream::in);
	std::string server;
	std::string port;
	std::string password;
	std::string listPos;
	if(!entrada.good()) //No hay fichero o no se puede abrir
	{
		return false;
	}
	std::getline(entrada, listPos);
	std::getline(entrada, server, (char)('|'+3));
	std::getline(entrada, port); //No se utiliza por ahora
	std::getline(entrada, password);
	entrada.close();

	if(server.length() == 0) //El fichero esta vacio?
	{
		return false;
	}

	decode(server);

	std::cout << "Encontrado server para reconectarse\n";
	//std::cout << "Server ip: " << server << std::endl;
	//std::cout << "Password: " << password << std::endl;

	bool connnectionAttempSuccess = connectToServer(server, password);

	if(!connnectionAttempSuccess) //Ha fallado al conectarse
	{
		return false;
	}

	unsigned int timeout = 1000;
	bool connectionSuccess = waitUntilConnected(timeout);
	
	if(!connectionSuccess)
	{
		removeServerFile();
		close();
		startClient();
	}
	else
	{
		aux = atoi(listPos.c_str());
	}

	return connectionSuccess;
}

bool NetworkEngine::waitUntilConnected( unsigned int timeout )
{
	Clock clock;
	while(clock.getElapsedTime() < timeout)
	{
		if(isConnected())
		{
			return true;
		}
	}
	//Ha pasado el timeout, puede ser que la contraseña fuese incorrecta
	std::cout << "Error: Timeout al conectarse al servidor\n";
	return false;
}

// El cliente encuentra servidores
void NetworkEngine::lanServerFound(RakNet::Packet * packet)
{
	RakNet::TimeMS time;
	RakNet::BitStream bsIn(packet->data,packet->length,false);

	bsIn.IgnoreBytes(1);
	bsIn.Read(time);
	bool serverIsInLobby = bsIn.ReadBit();

	if(serverIsInLobby) //Solo muestra los que no han empezado
	{
		unsigned char numPlayers;
		bsIn.Read(numPlayers);
		
		ServerInfo server;			
		server.ip = packet->systemAddress.ToString(false);
		server.ping = RakNet::GetTimeMS() - time ;
		server.players = numPlayers;
		server.isLan = true;
		if(numPlayers < 0 || numPlayers > MAX_PLAYERS) //Se ignoran los servers rotos
		{
			return; 
		}
		//add server en browser
		if(serverBrowser.addServer(server))
		{
			GameManager::getInstance()->getGraphicsEngine()->addServerToBrowserTable(server);
		}
		
		// Es un cliente que ha arrancado un servidor y esta esperando
		if (!GameManager::getInstance()->isServer() && waitingForServer)
		{
			// Se autoconecta a este ultimo
			waitingForServer = false;
			joinServer( serverBrowser.getServerInfoList().size() - 1 );
		}
	}
}

void NetworkEngine::joinServer( int id )
{
	serverBrowser.joinServer(id);
}

int NetworkEngine::getCharacter()
{
	int ret=0;

	for(size_t i=0;i<playerList.size();i++)
	{
		if(playerList[i].guid == peer->GetMyGUID())
		{
			if(playerList[i].character == RakNet::RakString("Bomber"))
			{
				ret = 1;
			}
			else if(playerList[i].character == RakNet::RakString("Engineer"))
			{
				ret = 2;
			}
			else if(playerList[i].character == RakNet::RakString("Battleship"))
			{
				ret = 3;
			}
			else if(playerList[i].character == RakNet::RakString("Fighter"))
			{
				ret = 4;
			}
			i=playerList.size();
		}
	}

	return ret;
}

void NetworkEngine::giveExperience(float experience)
{
	RakNet::BitStream bitStream;
	bitStream.Write((unsigned char)ID_SERVER_GIVE_EXPERIENCE);
	bitStream.Write(experience);
	peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkEngine::giveMeCharacter()
{
	RakNet::BitStream bitStream;
	bitStream.Write((unsigned char)ID_GET_CHARACTER);
	bitStream.Write(aux);
	bitStream.Write(peer->GetMyGUID());
	peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkEngine::sendCharacterToClient(RakNet::Packet * packet)
{
	RakNet::BitStream bsIn(packet->data,packet->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID)); //En otros sitios esta como bsIn.IgnoreBytes(1), es lo mismo
	int size;
	RakNet::RakNetGUID mynewguid;
	bsIn.Read(size);
	bsIn.Read(mynewguid);

	if(size != -1)
	{
		playerList[size].guid = mynewguid;

		RakNet::BitStream bitStream;
		bitStream.Write((unsigned char)ID_SERVER_YOUR_CHARACTER);
		bitStream.Write(mynewguid);
		bitStream.Write(playerList[size].character);
		peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		/*for(size_t i=0;i<list.Size();i++)
		{
			std::cout<<std::endl<<std::endl<<((GameObject*)list[i])->position<<std::endl<<std::endl;
		}*/
	}


}

void NetworkEngine::experienceForPlayers(RakNet::Packet *packet)
{
	RakNet::BitStream bsIn(packet->data,packet->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID)); //En otros sitios esta como bsIn.IgnoreBytes(1), es lo mismo
	float experience;

	bsIn.Read(experience);

	Message message;
	message.type = Message::ADD_EXPERIENCE;
	message.value = experience;
	GameObject* receiver = GameManager::getInstance()->getGameObjectManager()->getMainPlayer();
	if(receiver != NULL)
	{
		receiver->broadcastMessage(message);
	}
}

void NetworkEngine::putTheCharacter(RakNet::Packet * packet)
{
	RakNet::BitStream bsIn(packet->data,packet->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID)); //En otros sitios esta como bsIn.IgnoreBytes(1), es lo mismo
	GameObject* mainPlayer = NULL;
	GameObjectManager *gameObjectManager = GameManager::getInstance()->getGameObjectManager();
	RakNet::RakString name;
	RakNet::RakNetGUID guid;

	bsIn.Read(guid);
	bsIn.Read(name);
	if(guid == peer->GetMyGUID())
	{
		if(name == RakNet::RakString("Battleship"))
		{
			mainPlayer = gameObjectManager->createBattleship();
		}
		else if(name == RakNet::RakString("Bomber"))
		{
			mainPlayer = gameObjectManager->createBomber();
		}
		else if(name == RakNet::RakString("Fighter"))
		{
			mainPlayer = gameObjectManager->createFighter();
		}
		else if(name == RakNet::RakString("Engineer"))
		{
			mainPlayer = gameObjectManager->createEngineer();	
		}

		this->mainPlayer = mainPlayer;
	}
}

// Abre un servidor en otro proceso
void NetworkEngine::launchServer()
{
	if (serverProcessId == 0)
	{
		serverProcessId = processManager->createProcess("SD_Server.exe");
		waitingForServer = true; // Espera a que arranque para que le conecte automaticamente
	}
	else
	{
		std::cout << "ERROR: Este cliente ya ha arrancado un servidor" << std::endl;
	}
}

void NetworkEngine::closeServer()
{
	if (serverProcessId > 0)
	{
		processManager->killProcess(serverProcessId);
		serverProcessId = 0;
	}
}

void NetworkEngine::sendGameObjectMessage(RakNet::RakNetGUID receptor, RakNet::NetworkID replicaID, Message message)
{
	if(!started)
	{
		//std::cout<<"Error: Intentando enviar un mensaje y la red esta desconectada\n";
		return;
	}
	if(!GameManager::getInstance()->isServer()) 
	{		
		receptor = peer->GetGuidFromSystemAddress(serverAddress);
		//receptor = RakNet::UNASSIGNED_RAKNET_GUID;
	}
	
	if(receptor == peer->GetMyGUID())
	{
		std::cout<<"WARNING: Enviando un RemoteMessage a si mismo, posible bug\n";
	}

	RakNet::BitStream bitStream;
	bitStream.Write((unsigned char)ID_GLOBAL_GAMEOBJECT_MESSAGE);
	bitStream.Write(replicaID);
	bitStream.Write(message);
	peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE, 0, receptor, false);
	
}

void NetworkEngine::onGameObjectMessage( RakNet::Packet * packet )
{
	RakNet::BitStream bsIn(packet->data,packet->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID)); //En otros sitios esta como bsIn.IgnoreBytes(1), es lo mismo
	RakNet::NetworkID id;
	Message message;

	bsIn.Read(id);
	bsIn.Read(message);

	GameObject *gameObject = replicaManager->getGameObjectByNetworkID(id);
	if(gameObject != NULL)
	{
		gameObject->broadcastMessage(message);
	}
}

void NetworkEngine::messageCreateSkill(unsigned int name,Vector2d pos,Vector2d posPlayer,RakNet::NetworkID id)
{
	RakNet::BitStream bitStream;
	bitStream.Write((unsigned char)ID_CREATE_SKILL);
	bitStream.Write(name);
	bitStream.Write(pos);
	bitStream.Write(posPlayer);
	bitStream.Write(id);
	peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE, 0,RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkEngine::sendGameOverMessageToClients(int levelRound,Clock wastedTime,int enemiesKilled)
{
	wastedTime.pause();
	RakNet::BitStream bitStream;
	bitStream.Write((unsigned char)ID_GAME_OVER);
	bitStream.Write(levelRound);
	bitStream.Write(wastedTime);
	bitStream.Write(enemiesKilled);
	peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE, 0,RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	update();
	GameManager::getInstance()->quit();
}

void NetworkEngine::gameOverFromServer(RakNet::Packet * packet)
{
	RakNet::BitStream bsIn(packet->data,packet->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	int levelRound;
	Clock wastedTime;
	int enemiesKilled;

	bsIn.Read(levelRound);
	bsIn.Read(wastedTime);
	bsIn.Read(enemiesKilled);

	StateGameOver::getInstance()->setData(levelRound,wastedTime,enemiesKilled);
	GameManager::getInstance()->changeState(StateGameOver::getInstance());
}

void NetworkEngine::createSkill(RakNet::Packet * packet)
{
	RakNet::BitStream bsIn(packet->data,packet->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID)); //En otros sitios esta como bsIn.IgnoreBytes(1), es lo mismo
	unsigned int name;
	Vector2d pos;
	Vector2d posPlayer;
	RakNet::NetworkID id;

	bsIn.Read(name);
	bsIn.Read(pos);
	bsIn.Read(posPlayer);
	bsIn.Read(id);
	GameObject* start = NULL;
	GameObject* end = NULL;
	Message message;
	
	switch (name)
	{
	case GameObject::N_BUILDING_MINE:
		GameManager::getInstance()->getGameObjectManager()->createBuildingMine(pos);
		break;
	case GameObject::N_MINE2:
		GameManager::getInstance()->getGameObjectManager()->createBuildingMine2(pos);
		break;
	case GameObject::N_MINE3:
		GameManager::getInstance()->getGameObjectManager()->createBuildingMine3(pos);
		break;
	case GameObject::N_BUILDING_TURRET_BULLET:
		GameManager::getInstance()->getGameObjectManager()->createBuildingTurret2(pos);
		break;
	case GameObject::N_BUILDING_TURRET_PROJECTILE:
		GameManager::getInstance()->getGameObjectManager()->createBuildingTurret(pos);
		break;
	case GameObject::N_BUILDING_TURRET_2_CANON:
		GameManager::getInstance()->getGameObjectManager()->createBuildingTurret2Canon(pos);
		break;
	case GameObject::N_BUILDING_TURRET_1_CANON:
		GameManager::getInstance()->getGameObjectManager()->createBuildingTurret1Canon(pos);
		break;
	case GameObject::N_BUILDING_TURRET_BIG_CANON:
		GameManager::getInstance()->getGameObjectManager()->createBuildingTurretBigCanon(pos);
		break;
	case GameObject::N_BUILDING_WALL:
		start = GameManager::getInstance()->getGameObjectManager()->createBuildingWall(pos);
		start->rotation = (posPlayer-pos).getAngle();
		end = GameManager::getInstance()->getGameObjectManager()->createBuildingWall(posPlayer);
		end->rotation = (pos-posPlayer).getAngle();
		GameManager::getInstance()->getGameObjectManager()->createBuildingEnergyWall(start,end);
		break;
	case GameObject::N_SKILL_MULTIPLE_SHOT:
		GameManager::getInstance()->getGameObjectManager()->createSkillMultimpleShot(posPlayer);
		break;
	case GameObject::N_SKILL_BOMB:
		GameManager::getInstance()->getGameObjectManager()->createSkillBomb(pos);
		break;
	case GameObject::N_SKILL_HEAL:
		GameManager::getInstance()->getGameObjectManager()->createSkillHeal(pos);
		break;
	case GameObject::N_SKILL_TARGET_PROJECTILE:
		message.type = Message::NEW_TARGET;
		message.gameObject = replicaManager->getGameObjectByNetworkID(id);
		GameObject* projectile;
		projectile = GameManager::getInstance()->getGameObjectManager()->createTargetProjectile(0);
	
		if(message.gameObject == NULL)
		{
			message.target = posPlayer + (pos-posPlayer)*5000;;
		}

		projectile->rotation = (pos - posPlayer).getAngle();
		projectile->position = posPlayer + Vector2d::getVector2dByAngle(-projectile->rotation)*7;
		projectile->velocity = (pos - posPlayer).normalize();

		projectile->broadcastMessage(message);
		break;
	case GameObject::N_SKILL_TARGET_HEAL:
		start = replicaManager->getGameObjectByNetworkID(id);

		if(start != NULL)
		{
			message.type = Message::INCOMING_DAMAGE;
			message.value = -100;

			start->broadcastMessage(message);
		}
		break;
	}
}

void NetworkEngine::messageSetWall(int col,int row)
{
	RakNet::BitStream bitStream;
	bitStream.Write((unsigned char)ID_SET_WALL);
	bitStream.Write(col);
	bitStream.Write(row);
	peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE, 0,RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkEngine::messageSetEmpty(int col,int row)
{
	RakNet::BitStream bitStream;
	bitStream.Write((unsigned char)ID_SET_EMPTY);
	bitStream.Write(col);
	bitStream.Write(row);
	peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE, 0,RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkEngine::setWall(RakNet::Packet * packet)
{
	RakNet::BitStream bsIn(packet->data,packet->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID)); //En otros sitios esta como bsIn.IgnoreBytes(1), es lo mismo
	int col;
	int row;

	bsIn.Read(col);
	bsIn.Read(row);

	GameManager::getInstance()->getMapManager()->setWall(col,row);
}

void NetworkEngine::setEmpty(RakNet::Packet * packet)
{
	RakNet::BitStream bsIn(packet->data,packet->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID)); //En otros sitios esta como bsIn.IgnoreBytes(1), es lo mismo
	int col;
	int row;

	bsIn.Read(col);
	bsIn.Read(row);

	GameManager::getInstance()->getMapManager()->setEmpty(col,row);
}

void NetworkEngine::nextWave(RakNet::Packet * packet)
{
	if(GameManager::getInstance()->getGameObjectManager()->getMainPlayer() != NULL)
	{
		Message message;
		message.type = Message::NEXT_WAVE;

		RakNet::BitStream bsIn(packet->data,packet->length,false);
		bsIn.IgnoreBytes(sizeof(RakNet::MessageID)); //En otros sitios esta como bsIn.IgnoreBytes(1), es lo mismo
		int wave;

		bsIn.Read(wave);
		
		message.value = wave;

		GameManager::getInstance()->getGameObjectManager()->getMainPlayer()->broadcastMessage(message);
	}

}

void NetworkEngine::messageNextWave(int numWave)
{

	RakNet::BitStream bitStream;
	bitStream.Write((unsigned char)ID_NEXT_WAVE);
	bitStream.Write(numWave);
	peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE, 0,RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}