#include <iostream>
#include "../common/GameManager.h"
#include <windows.h> 

//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup") //Desactiva la consola
BOOL CtrlHandler( DWORD fdwCtrlType ) 
{ 
	switch( fdwCtrlType ) 
	{ 
		// CTRL-CLOSE: confirm that the user wants to exit. 
	case CTRL_CLOSE_EVENT: 
		GameManager::getInstance()->cleanup();
		return( TRUE ); 
		// Pass other signals to the next handler. 
	case CTRL_BREAK_EVENT: 
		return FALSE; 

	case CTRL_LOGOFF_EVENT: 
		return FALSE; 

	case CTRL_SHUTDOWN_EVENT: 
		return FALSE; 

	default: 
		return FALSE; 
	} 
} 

int main ()
{

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE ); 

	std::cout << "Space Defenders - Game Server\n";
	
	GameManager &gameManager = * GameManager::getInstance();
	// Arrancando motores
	gameManager.setIsServer(true);
	gameManager.init();
	gameManager.run();

	std::cout << "\n\nFinalizando\n\n";
	//system("del server.log");
	return 0;
}
