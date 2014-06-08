#include "ServerBrowser.h"
#include "NetworkEngine.h"
#include "GameManager.h"
#include "StateClientLobby.h"

ServerBrowser::ServerBrowser(void)
{
	gameManager = GameManager::getInstance();
}

ServerBrowser::~ServerBrowser(void)
{
}

void ServerBrowser::clear()
{
	serverInfoList.clear();
}

bool ServerBrowser::addServer(ServerInfo server)
{
	//std::cout<<serverInfoList.size()<<std::endl;
	if(serverInfoList.size() > 0)
	{
		bool found=false;
		for(std::size_t i = 0; i < serverInfoList.size();i++)
		{
			if(server.ip == serverInfoList[i].ip)
			{
				found = true;
				i = serverInfoList.size();
			}
		}
		if(!found)
		{
			serverInfoList.push_back(server);
			return true;
		}
	}
	else
	{
		serverInfoList.push_back(server);
		return true;
	}

	return false;
}

void ServerBrowser::joinServer(int id)
{
	NetworkEngine* networkEngine = gameManager->getNetworkEngine();
	if(!serverInfoList.empty() && id >= 0)
	{
		if(networkEngine->connectToServer(serverInfoList[id].ip.ToString()))
		{
			std::cout << serverInfoList[id].ip.ToString() << std::endl;
			bool success = networkEngine->waitUntilConnected(2000);
			if(success)
			{
				GameManager::getInstance()->changeState(StateClientLobby::getInstance());
			}
		}
	}
}

bool ServerBrowser::isServerFull(int id)
{
	if(serverInfoList[id].players < 4)
	{
		return false;
	}else{
		return true;
	}
	
}