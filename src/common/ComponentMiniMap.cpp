#include "ComponentMiniMap.h"
#include "GraphicsEngine.h"
#include "GameManager.h"
#include "MinimapSprite.h"


ComponentMiniMap::ComponentMiniMap(const char* spriteFilename, int color)
{   
	clock = Clock();

	//Calcula cuantos frames tiene que dejar pasar para repartirse entre todos los frames
	//Estos sprites empiezan a partir del id 4602
	this->spriteFilename = spriteFilename;
	this->color = color;
}


ComponentMiniMap::~ComponentMiniMap(void)
{
	
	delete minimapSprite;
   
}


void ComponentMiniMap::update()
{
	//Reparte los updates por varios frames para que no se los tenga que comer todos un frame y 
	//se note una bajada de rendimiento cada vez que le toque actualizar a todos los sprites
	//Solo entraria durante los primeros updates
	//Va retrasando el reloj cada frame mientras siga quedando delay
	if(delay > 0) 
	{
		delay--;	
		clock.restart();
		return;
	}
	if(clock.getElapsedTime() >= 1000/REFRESH_RATE)
	{

		GameManager* gameManager = GameManager::getInstance();
		//Es mas sencillo trabajar con posiciones en el grid que en coordenadas asi para ajustar en el minimapa	
		Vector2d positionOnGrid = gameManager->getMapManager()->getFrame(parent->position);
		positionOnGrid = Vector2d(positionOnGrid.y, positionOnGrid.x);

		//CAmbiar para que salga el objeto en el centro de la posicion que le pasas
		int x = gameManager->getGraphicsEngine()->getWindowSize().x-180+(parent->position.x/scale); 
		int y = gameManager->getGraphicsEngine()->getWindowSize().y-175+(-parent->position.y/scale);
		
		minimapSprite->setPosition(Vector2d(x,y));
		
		clock.restart();
	}

}

void ComponentMiniMap::awake()
{
	float mapSize = (float)GameManager::getInstance()->getMapManager()->getMapSize();
	//sacamos una proporción de tamaño segun el tamaño del mapa
	scale = mapSize/MINIMAP_SIZE;
	// 4x4 es el tamaño del sprite

	minimapSprite = new MinimapSprite(spriteFilename,Vector2d(0,0),Vector2d(16,16));

	minimapSprite->setColor(color);
	float size = CONSTANT_SCALE/scale;
	if(size < 6)
	{
		size = 5;

	}
	minimapSprite->setScale(Vector2d(size,size));
	delay = (minimapSprite->getId()-4602) % (60/REFRESH_RATE);

}

