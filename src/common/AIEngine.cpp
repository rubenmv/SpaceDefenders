#include "AIEngine.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "Vector2d.h"
#include "GameObject.h"
#include "Clock.h"
#include "EventManager.h"
#include "Path.h"
#include "Message.h"
#include "StateGameOver.h"

const int AIEngine::ENEMY_MULTIPLIER=2;
const int AIEngine::INITIAL_ENEMIES=64;
const int AIEngine::DEFEND_TIME=3000; //3 segundos

const float AIEngine::BASIC_MULTIPLIER = 20; //20
const float AIEngine::BOMBER_MULTIPLIER = 0.8; //0.8
const float AIEngine::FURY_MULTIPLIER = 0.5; //0.5


AIEngine::AIEngine(void)
{

	numberBase = 0;
	gameObjectManager = GameManager::getInstance()->getGameObjectManager();
	levelRound = 0;
//	totalEnemiesRound = INITIAL_ENEMIES;
	totalEnemiesRoundInGame = 0;
	createEnemyPool();
	enemiesRound = totalEnemiesRound;
	
	numEnemiesWave = 0;
	state = DEFENSE; //0 oleada, 1 defensa
	totalEnemiesKilled = 0;
	gameOver = false;
	deathClock = Clock();
	globalWaveClock = Clock(); //Para calcular el tiempo total de la partida
	buildingClock = Clock(); //controla el tiempo de construccion
	

	createPaths();
}


AIEngine::~AIEngine(void)
{
}

void AIEngine::update()
{
	if(numberBase <= 0)
	{

			if(GameManager::getInstance()->isServer())
			{
				GameManager::getInstance()->getNetworkEngine()->sendGameOverMessageToClients(levelRound,globalWaveClock,totalEnemiesKilled);
			}
			else
			{
				StateGameOver::getInstance()->setData(levelRound, globalWaveClock, totalEnemiesKilled);
				GameManager::getInstance()->changeState(StateGameOver::getInstance());
			}
		
	}
	else
	{
		updatePaths();
		switch(state)
		{

		case State_AIEngine::WAVE: //oleada
			updateWave();
			break;
		case State_AIEngine::DEFENSE:
			updateDefense();
			break;

		}
	}
}

void AIEngine::createEnemies(int num)
{

	for(int i = 0; i < num;i++)
	{
		selectEnemyOfTheEnemyPool();
	}

}


void AIEngine::updateWave() 
{
	if(enemiesRound <= 0 && numEnemiesWave <= 0)
	{
		buildingClock.restart();
		state = State_AIEngine::DEFENSE;
	}else
	{
		//nueva oleada
		if(numEnemiesWave <= (totalEnemiesRound/6) )
		{
			if((totalEnemiesRound/4 - numEnemiesWave) <= enemiesRound)
			{
				createEnemies(totalEnemiesRound/4 - numEnemiesWave);
				enemiesRound -= totalEnemiesRound/4 - numEnemiesWave;
				totalEnemiesRoundInGame += totalEnemiesRound/4 - numEnemiesWave;
				numEnemiesWave += totalEnemiesRound/4 - numEnemiesWave;



			}else
			{
				createEnemies(enemiesRound);
				totalEnemiesRoundInGame += enemiesRound;
				numEnemiesWave += enemiesRound;
				enemiesRound -= enemiesRound;

			}
		}
	}
}


void AIEngine::endGame()
{
	numberBase--;
	if(numberBase <= 0)
	{
		std::cout<<"Game Over \n";
	}
}

void AIEngine::addBase()
{
	numberBase++;
}

void AIEngine::createPaths()
{
	//Inicial

	firstPath = new Path(Vector2d(832,-59),Vector2d(781,-112));

	/////A

	Path* pathA11 = new Path(Vector2d(781,-112), Vector2d(487,-88));
	Path* pathA12 = new Path(Vector2d(781,-112), Vector2d(613,-290));
	Path* pathA13 = new Path(Vector2d(781,-112), Vector2d(806, -460));


	/////B
	Path* pathB11 = new Path(Vector2d(487,-88), Vector2d(241,-226));
	Path* pathB12 = new Path(Vector2d(487,-88), Vector2d(385,-373));

	Path* pathB22 = new Path(Vector2d(613,-290), Vector2d(385,-373));
	Path* pathB23 = new Path(Vector2d(613,-290), Vector2d(562,-564));

	Path* pathB33 = new Path(Vector2d(806, -460), Vector2d(562,-564));
	Path* pathB34 = new Path(Vector2d(806, -460), Vector2d(700, -777));

	/////C
	Path* pathC11 = new Path(Vector2d(241,-226), Vector2d(120,-465));
	Path* pathC12 = new Path(Vector2d(241,-226), Vector2d(363,-599));
			 
	Path* pathC21 = new Path(Vector2d(385,-373), Vector2d(120,-465));
	Path* pathC22 = new Path(Vector2d(385,-373), Vector2d(363,-599));
			  
	Path* pathC32 = new Path( Vector2d(562,-564), Vector2d(363,-599));
	Path* pathC33 = new Path( Vector2d(562,-564), Vector2d(441,-798));
			  
	Path* pathC42 = new Path( Vector2d(700, -777), Vector2d(363,-599));
	Path* pathC43 = new Path( Vector2d(700, -777), Vector2d(441,-798));


	/////D
	Path* pathD11 = new Path(Vector2d(120,-465), Vector2d(142,-777));
	Path* pathD21 = new Path(Vector2d(363,-599), Vector2d(142,-777));
	Path* pathD31 = new Path(Vector2d(441,-798), Vector2d(142,-777));

	// Asignacion de siguiente camino

	//Inicial

	firstPath->addNextPath(pathA11);
	firstPath->addNextPath(pathA12);
	firstPath->addNextPath(pathA13);

	/////A
	pathA11->addNextPath(pathB11);
	pathA11->addNextPath(pathB12);

	pathA12->addNextPath(pathB22);
	pathA12->addNextPath(pathB23);

	pathA13->addNextPath(pathB33);
	pathA13->addNextPath(pathB34);

	/////B
	pathB11->addNextPath(pathC11);
	pathB11->addNextPath(pathC12);

	pathB12->addNextPath(pathC21);
	pathB12->addNextPath(pathC22);

	pathB22->addNextPath(pathC21);
	pathB22->addNextPath(pathC22);

	pathB23->addNextPath(pathC32);
	pathB23->addNextPath(pathC33);

	pathB33->addNextPath(pathC32);
	pathB33->addNextPath(pathC33);

	pathB34->addNextPath(pathC42);
	pathB34->addNextPath(pathC43);

	////B

	pathC11->addNextPath(pathD11);

	pathC12->addNextPath(pathD21);

	pathC21->addNextPath(pathD11);

	pathC22->addNextPath(pathD21);

	pathC32->addNextPath(pathD21);

	pathC33->addNextPath(pathD31);

	pathC42->addNextPath(pathD21);

	pathC43->addNextPath(pathD31);

	
	pathList.push_back(firstPath);

	pathList.push_back(pathA11);
	pathList.push_back(pathA12);
	pathList.push_back(pathA13);

	pathList.push_back(pathB11);
	pathList.push_back(pathB12);
	pathList.push_back(pathB22);
	pathList.push_back(pathB23);
	pathList.push_back(pathB33);
	pathList.push_back(pathB34);


	pathList.push_back(pathC11);
	pathList.push_back(pathC12);
	pathList.push_back(pathC21);
	pathList.push_back(pathC22);
	pathList.push_back(pathC32);
	pathList.push_back(pathC33);
	pathList.push_back(pathC42);
	pathList.push_back(pathC43);

	pathList.push_back(pathD11);
	pathList.push_back(pathD21);
	pathList.push_back(pathD31);
}


void AIEngine::updatePaths()
{
	for(std::size_t i = 0; i < pathList.size() ; i++)
	{
		pathList[i]->update();
	}
}


int AIEngine::getNextPath( int name , std::vector<Path*> pathList )
{
	if(pathList.size() > 0 && name == GameObject::N_ENEMY_BOMBER)
	{
		int path = pathWithMoreStructs( pathList );
		if(pathList[path]->getNumberOfStructs() > 0) //si es mayor que 0 devuelve ese si no seguiria el procedimiento normal
		{
			return path;
		}
	}
	if(pathList.size() > 0)
	{
		return selectNextPath(pathList);
	}

	return -1;
}


void AIEngine::updateDefense()
{
	//Nueva ronda
	if( buildingClock.getElapsedTime() >= DEFEND_TIME)
	{
		levelRound++;
		if(GameManager::getInstance()->getNetworkEngine()->isStarted() && GameManager::getInstance()->isServer())
		{
			GameManager::getInstance()->getNetworkEngine()->messageNextWave(levelRound);
		}
		/*Message message;
		message.type = Message::NEXT_WAVE;
		GameManager::getInstance()->getGameObjectManager()->getMainPlayer()->broadcastMessage(message);
		*/

		createEnemyPool();
		//totalEnemiesRound = totalEnemiesRound*ENEMY_MULTIPLIER;
		totalEnemiesRoundInGame = 0; 
		enemiesRound = totalEnemiesRound;
		waveTime = enemiesRound*1000;//1 segundo por enemigo
		state = State_AIEngine::WAVE;
	}
}

void AIEngine::selectEnemyOfTheEnemyPool()
{		
	int enemy = Math::random(EnemyPool.size());
	switch (EnemyPool[enemy])
	{
	case GameObject::N_ENEMY_BOMBER:
		gameObjectManager->createEnemyBomber();
		break;
	case GameObject::N_ENEMY_FURY:
		gameObjectManager->createFuryEnemies();
		break;
	case GameObject::N_ENEMY_BASIC:
		gameObjectManager->createEnemyBasic();
		break;
	default:
		break;
	}
	
	EnemyPool.erase(EnemyPool.begin()+enemy);

}

void AIEngine::createEnemyPool()
{
	EnemyPool.clear();

	
	int basic = 0;
	int bomber = 0;
	int fury = 0;

	basic = levelRound*BASIC_MULTIPLIER;
	for (int i = 0; i<basic; i++)
	{
		EnemyPool.push_back(GameObject::N_ENEMY_BASIC);
	}
	
	if(levelRound > 2) //Empieza a soltar bombarderos 2
	{
		bomber = levelRound*BOMBER_MULTIPLIER;	
		for (int i = 0; i<bomber; i++)
		{
			EnemyPool.push_back(GameObject::N_ENEMY_BOMBER);
		}
	}
	
	if(levelRound > 4)  //Empieza a soltar bombarderos cazas 4
	{
		fury = levelRound*FURY_MULTIPLIER;
		for (int i = 0; i<fury; i++)
		{
			EnemyPool.push_back(GameObject::N_ENEMY_FURY);
		}
	}

	totalEnemiesRound = basic+fury+bomber;
}



void AIEngine::addStruct(Vector2d position)
{

	Path* path = firstPath;

	for(size_t i = 1; i < pathList.size(); i++)
	{
		if(distanceFromThePath(position,path) > distanceFromThePath(position,pathList[i]))
		{
			path = pathList[i];
		}
	}

	path->addStruct();
}

void AIEngine::deleteStruct(Vector2d position)
{

	Path* path = firstPath;

	for(size_t i = 1; i < pathList.size(); i++)
	{
		if(distanceFromThePath(position,path) > distanceFromThePath(position,pathList[i]))
		{
			path = pathList[i];
		}
	}

	path->deleteStruct();
}


float AIEngine::distanceFromThePath( Vector2d position, Path* path )
{
	Vector2d start  = path->getStart();
	Vector2d end = path->getEnd();
	Vector2d normalPoint = Vector2d::getNormalPoint(position, start, end);

	//COmprobamos que el punto esta entra el principio y el fin, si no es cogemos el final
	if (normalPoint.x < Math::min_(start.x,end.x) || normalPoint.x > Math::max_(start.x,end.x) ) 
	{
		if(start.getDistanceFrom(normalPoint) < end.getDistanceFrom(normalPoint))
		{
			normalPoint = start;
		}else
		{
			normalPoint = end;
		}
	}
	

	float distance = position.getDistanceFrom(normalPoint);

	return distance;

}

int AIEngine::pathWithMoreStructs( std::vector<Path*> nextPathList )
{

	int path = 0;

	for(size_t i = 1; i < nextPathList.size();i++)
	{

		if(nextPathList[path]->getNumberOfStructs() < nextPathList[i]->getNumberOfStructs())
		{
			path = i;
		}
	}

	return path;
}

int AIEngine::selectNextPath( std::vector<Path*> pathList )
{

	bool selectedPath = true; //se usa para comprobar si en alguno de los casos no se cumple el requisito


	for(size_t i = 0; i < pathList.size(); i++)
	{
		selectedPath = true;

		if(pathList[i]->getPreference())
		{
			//si ya se han equilibrado se vuelve a poner preferencia falso			
			for(size_t j = 0; j < pathList.size(); j++)
			{
				if(pathList[i]->getNumberOfEnemies() < pathList[j]->getNumberOfEnemies())
				{
					selectedPath = false;					
				}								
			}

			//Si entra es que vuelve a estar equilibrado
			if(selectedPath) 
			{
				pathList[i]->setPreference(false);
			}
			return i;

		}


		selectedPath = true;
		for(size_t j = 0; j < pathList.size(); j++)
		{
			if(pathList[i]->getNumberOfEnemies() > pathList[j]->getNumberOfEnemies()*0.5 )
			{
				selectedPath = false;	
				
			}
		}

		//Comprobamos si tiene mas del doble que los demas y le damos preferencia
		if(selectedPath) 
		{
			pathList[i]->setPreference(true);
			return i;
		}	


	}

	return Math::random(pathList.size());

}


Path* AIEngine::newPath( GameObject* basic )
{
	Path* path = firstPath;

	for(size_t i = 1; i < pathList.size(); i++)
	{
		if(distanceFromThePath(basic->position,path) > distanceFromThePath(basic->position,pathList[i]))
		{
			path = pathList[i];
		}
	}

return path;

}