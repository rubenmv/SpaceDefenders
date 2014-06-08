#include "StateBenchmark.h"
#include <iostream>
#include <sstream>
#include <vector>
#include "../GraphicsEngine.h"
#include "../GameManager.h"
#include "../GameObject.h"
#include "../GameObjectManager.h"
#include "../EventManager.h"
#include "../Component.h"
#include "../ComponentGraphics.h"
#include "../ComponentCamera.h"
#include "../DebugTools.h"
#include "../AIEngine.h"
#include "../CollisionManager.h"
#include <time.h>
#include "../StateMenuMain.h"
#include "../PhysicsManager.h"
// Singleton
StateBenchmark* StateBenchmark::pInstance = NULL;

StateBenchmark::~StateBenchmark(void)
{
	mapManager = NULL;
}


StateBenchmark* StateBenchmark::getInstance()
{
	if(pInstance == NULL)
		pInstance = new StateBenchmark();

	return pInstance;
}


StateBenchmark::StateBenchmark()
{
	name = State::StateName::TEST_WAVES;
	frames = 0;
	numberBasics = 0;
	numberTurrets = 0;
	numberBomber = 0;
	numberFury = 0;
	numberMines = 0;
}

void StateBenchmark::init()
{

	frames = 0;
	numberBasics = 0;
	numberTurrets = 0;
	numberBomber = 0;
	numberFury = 0;

	gameManager = GameManager::getInstance();

	gameObjectManager = gameManager->getGameObjectManager();
	collisionManager = gameManager->getCollisionManager();
	physicsManager = gameManager->getPhysicsManager();
	mapManager = gameManager->getMapManager();
	gameManager->getGraphicsEngine()->createDebugMap(mapManager->getCollisionMap());

	aiEngine =  gameManager->getAIEngine();

	gameObjectManager->createWholeBase(Vector2d(144,-770));
	GameObject* player = gameObjectManager->createFighter();
	player->position = Vector2d(548,-329);
	gameManager->enableDebugMode();
	createWaves();
	createFurys();
	createBombers();
	createMines();
	createTurrets();
	createWall();
	//GameObject* bomber = gameObjectManager->createEnemyBomber();
	gameManager->getGraphicsEngine()->initWorld();

	std::cout<<"Basic: "<<numberBasics<<std::endl;
	std::cout<<"Mines: "<<numberMines<<std::endl;
	std::cout<<"Bomber: "<<numberBomber<<std::endl;
	std::cout<<"Caza: "<<numberFury<<std::endl;
	std::cout<<"Turrets: "<<numberTurrets<<std::endl;
	clock.start();

}

void StateBenchmark::createWaves()
{
	Vector2d init;
	Vector2d end;
	float count = 8;
	init = Vector2d(832,-59);
	end = Vector2d(487,-88);

	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(613,-290);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(806, -460);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	
	//2do Sector
	init = Vector2d(487,-88);
	end = Vector2d(241,-226);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(385,-373);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}

	init = Vector2d(613,-290);
	end = Vector2d(385,-373);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(562,-564);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}

	init = Vector2d(806, -460);
	end = Vector2d(562,-564);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(700, -777);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}

	//3er Sector
	init = Vector2d(241,-226);
	end = Vector2d(120,-465);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(363,-599);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}

	init = Vector2d(385,-373);
	end = Vector2d(120,-465);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(363,-599);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}

	init = Vector2d(562,-564);
	end = Vector2d(363,-599);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(441,-798);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}

	init = Vector2d(700, -777);
	end = Vector2d(363,-599);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(441,-798);
	for(float i=1/count;i<=1;i += 1/count)
	{
		GameObject* enemy = gameObjectManager->createEnemyBasic();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
}

void StateBenchmark::createMines()
{
	for(int i=0;i<900;i+=150)
	{
		for(int j=0;j>-900;j-=150)
		{
			Vector2d pos = Vector2d(i,j);
			if(mapManager->isEmpty(pos))
			{
				GameObject* enemy = gameObjectManager->createMine();
				enemy->position = Vector2d(i,j);
				numberMines++;
			}
		}
	}
}

void StateBenchmark::createTurrets()
{
	Vector2d start;
	Vector2d end;
	float cant = 2;
	start = Vector2d(832,-59) + Vector2d(10,10);
	end = Vector2d(487,-88) + Vector2d(-10,-10);;

	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}
	end = Vector2d(613,-290) + Vector2d(-10,-10);;
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret2();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}
	end = Vector2d(806, -460) + Vector2d(-10,-10);;
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}
	
	//2do Sector
	start = Vector2d(487,-88) + Vector2d(10,10);
	end = Vector2d(241,-226) + Vector2d(-10,-10);;
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret2();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}
	end = Vector2d(385,-373) + Vector2d(-10,-10);;
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}

	start = Vector2d(613,-290) + Vector2d(10,10);
	end = Vector2d(385,-373) + Vector2d(-10,-10);;
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret2();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}
	end = Vector2d(562,-564) + Vector2d(-10,-10);;
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}

	start = Vector2d(806, -460) + Vector2d(10,10);
	end = Vector2d(562,-564) + Vector2d(-10,-10);;
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret2();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}
	end = Vector2d(700, -777) + Vector2d(-10,-10);;
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}

	//3er Sector
	start = Vector2d(241,-226) + Vector2d(10,10);
	end = Vector2d(120,-465) + Vector2d(-10,-10);;
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret2();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}
	end = Vector2d(363,-599) + Vector2d(-10,-10);;
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}

	start = Vector2d(385,-373) + Vector2d(10,10);
	end = Vector2d(120,-465) + Vector2d(-10,-10);;
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret2();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}
	end = Vector2d(363,-599) + Vector2d(-10,-10);;
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}

	start = Vector2d(562,-564) + Vector2d(10,10);
	end = Vector2d(363,-599) + Vector2d(-10,-10);;
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret2();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}
	end = Vector2d(441,-798) + Vector2d(-10,-10);;
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}

	start = Vector2d(700, -777) + Vector2d(10,10);
	end = Vector2d(363,-599) + Vector2d(-10,-10);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret2();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}
	end = Vector2d(441,-798) + Vector2d(-10,-10);;
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createTurret();
		enemy->position = Vector2d::lerp(start,end,i);
		numberTurrets++;
	}
}

void StateBenchmark::createWall()
{
	gameObjectManager->createEnergyWall(Vector2d (500,-290),Vector2d (574,-380));
	gameObjectManager->createEnergyWall(Vector2d (654,-481),Vector2d (796,-556));
	gameObjectManager->createEnergyWall(Vector2d (470,-560),Vector2d (523,-665));
	gameObjectManager->createEnergyWall(Vector2d (327,-420),Vector2d (220,-333));
	gameObjectManager->createEnergyWall(Vector2d (225,-654),Vector2d (263,-748));
	gameObjectManager->createEnergyWall(Vector2d (205,-630),Vector2d (98,-640));
}

void StateBenchmark::createFurys()
{
	for(int i=0;i<=10;i++)
	{
		gameObjectManager->createFuryEnemies();
		numberFury++;
		numberFury++;
	}
}

void StateBenchmark::createBombers()
{
	Vector2d init;
	Vector2d end;
	float cant = 1;
	init = Vector2d(832,-59);
	end = Vector2d(487,-88);

	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(613,-290);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(806, -460);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	
	//2do Sector
	init = Vector2d(487,-88);
	end = Vector2d(241,-226);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(385,-373);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}

	init = Vector2d(613,-290);
	end = Vector2d(385,-373);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(562,-564);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}

	init = Vector2d(806, -460);
	end = Vector2d(562,-564);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(700, -777);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}

	//3er Sector
	init = Vector2d(241,-226);
	end = Vector2d(120,-465);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(363,-599);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}

	init = Vector2d(385,-373);
	end = Vector2d(120,-465);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(363,-599);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}

	init = Vector2d(562,-564);
	end = Vector2d(363,-599);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(441,-798);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}

	init = Vector2d(700, -777);
	end = Vector2d(363,-599);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
	end = Vector2d(441,-798);
	for(float i=1/cant;i<=1;i += 1/cant)
	{
		GameObject* enemy = gameObjectManager->createEnemyBomber();
		enemy->position = Vector2d::lerp(init,end,i);
		numberBasics++;
	}
}

void StateBenchmark::cleanup()
{
}

void StateBenchmark::update(GameManager* gameManager)
{
	if(physicsManager != NULL)	physicsManager->update();
	gameObjectManager->update();
	collisionManager->update();
	gameManager->getDebugTools()->update();
	frames++;

	//std::cout<<"Frames: "<<frames<<std::endl;
	//std::cout<<"NumberBasics: "<<numberBasics<<std::endl;
	if(frames > 600)
	{
		int time = clock.restart();
		std::cout << "Tiempo: " << time << std::endl;
		std::cout << "FPS: " << 600/(time/1000.0) << std::endl;


		GameManager::getInstance()->changeState(StateMenuMain::getInstance());
	}
}


