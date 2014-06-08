#pragma once
#include <vector>
#include "raknet/RakNetTypes.h"

/*
	Mantiene y gestiona la lista de servidores con su informacion
*/

struct ServerInfo
{
	std::string name;
	RakNet::SystemAddress ip;
	unsigned int players;
	bool isLan;
	unsigned long ping;
};

class GameManager;

class ServerBrowser
{
public:
	ServerBrowser();
	virtual ~ServerBrowser();
	void clear();
	void joinServer(int id);
	bool isServerFull(int id);

	std::vector<ServerInfo> getServerInfoList()
	{
		return serverInfoList;
	}
	
	bool addServer(ServerInfo server);
private:

	std::vector<ServerInfo> serverInfoList;
	GameManager* gameManager;

	//Crear estructura en networkengine nombre, ip, jugadores, ping
	//Antes de actualizar la lista borrarlos uno a uno 
};

