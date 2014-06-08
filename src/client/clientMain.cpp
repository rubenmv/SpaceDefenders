
#include <iostream>
#include "../common/GameManager.h"


#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup") //Desactiva la consola

int main ()
{
	std::cout << "Space Defenders - Game Manager\n";

	GameManager* gameManager = GameManager::getInstance();

	do
	{
		gameManager->init();
		gameManager->run();
		gameManager->cleanup();
	} while (gameManager->isRestart());

	std::cout << "\n\nFinalizando...\n\n";
	
	//getchar();

	return 0;
}
