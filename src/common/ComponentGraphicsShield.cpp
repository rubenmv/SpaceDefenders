#include "ComponentGraphicsShield.h"
#include "NodeMesh.h"
#include "GraphicsEngine.h"
#include "GameManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"

ComponentGraphicsShield::ComponentGraphicsShield(bool groundShield, float radius)
{
	GraphicsEngine* graphicsEngine = GameManager::getInstance()->getGraphicsEngine();
	node = graphicsEngine->addNodeMesh(Resources::MESH_SHIELD, true,0,Vector2d(-500,-500),0,radius);

	if(!node)
	{
		node = graphicsEngine->addNodeMesh("../assets/others/error.obj");
	}
	
	
	this->groundShield = groundShield;
	//node->setScale(radius);
	intensity = 0;
	enabled = true;
	node->setIntensity(intensity);
	//node->setVisible(false);
	//Inicializamos impacto
	initImpact();

	
}


ComponentGraphicsShield::~ComponentGraphicsShield(void)
{

	delete node;
	node = 0;
}

void ComponentGraphicsShield::update()
{

	Vector3d newPosition = parent->position.asVector3d();
	
	if(groundShield)
	{
		newPosition.y = -5;
	}

	node->setPosition(newPosition);

	if(intensity > 0)
	{
		updateShield();
		
	}
	//Este trozo tengo que pensar en comno optimizarlo mas, solo se debe actualizar cuando la intensidad del escudo principal sea > 0, pero la ultima vez 
	// Pero se tiene que apagar todos o apagar de golpe, lo que prefirais
	updateImpacts();
	sendImpacts();
}

void ComponentGraphicsShield::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::SHIELD_CHANGED:
		changedShield(message.target);

		break;
	case Message::INCOMING_DAMAGE:	
		if(message.value != 100000000) //Parche para solucionar el problema de Usar la R
		{
			activateShield();
			if(enabled)
			{
				impactEnabled( message.target );
			}					
			
		}
		break;	
	}
}
//Controla si al jugador le queda escudo
void ComponentGraphicsShield::changedShield( Vector2d target )
{

	if(target.x <= 0 && enabled)
	{
		node->setVisible(false);
		enabled = false;
		intensity = 0;
		node->setIntensity(intensity);
		up = false;
		initImpact();

	}else if(target.x >0 && !enabled)
	{		
		node->setVisible(true);
		up = true;
		enabled = true;
		initImpact();
	}
}
//Activa el escudo
void ComponentGraphicsShield::activateShield()
{
	if(enabled)
	{
		up = true;
		node->setVisible(true);
		if(intensity == 0)
		{
			intensity = 0.001;
		}

	}
	
	
}

void ComponentGraphicsShield::updateShield()
{
	if(up)
	{
		intensity += 5 * GameManager::getInstance()->getDeltaTime();//*5;
		if(intensity > 0.8)
		{
			intensity = 1;
			up = false;
		}
	}else{
		
		intensity -= 5 * GameManager::getInstance()->getDeltaTime();
		if(intensity < 0)
		{
			intensity = 0;
		}
	}
	node->setIntensity(intensity);
}




void ComponentGraphicsShield::impactEnabled( Vector2d position )
{

	Vector2d direction = position - parent->position;// - parent->position ; //velocity pero ahora es la posicion 	
//	direction.y = 0;
	direction.normalize();

	for(int i = 0; i<4;i++)
	{
		Vector2d impact = Vector2d(impactList[i*4+0],impactList[i*4+1]);

		if(direction.getSqrDistanceFrom(impact) < 0.0025) //Si al cuadrado 0.05
		{
			impactList[i*4+2] = 1; //Controla que suba o baje el escudo
			//impactList[nextImpact*4+3] += 0; //Intensidad
			//std::cout<<"Entra: "<<direction.getDistanceFrom(impact)<<"\n";
			return;
		}

	}


	if(nextImpact >= 4) //Por ahora el maximo es 4
	{
		nextImpact = 0;
	}
	
	
	impactList[nextImpact*4+0] = direction.x; //Posicion X
	impactList[nextImpact*4+1] = direction.y; // Posicion y
	impactList[nextImpact*4+2] = 1; //Controla que suba o baje el escudo
	impactList[nextImpact*4+3] = 0; //Intensidad
	
	nextImpact++;

}

void ComponentGraphicsShield::updateImpacts()
{


	for (int i = 0; i<4;i++)
	{
		float upImpacts;
		float intensityImpact;

		upImpacts = impactList[i*4+2]; //Controla que suba o baje el escudo
		intensityImpact = impactList[i*4+3]; //Intensidad

		if(upImpacts >= 1)
		{
			intensityImpact += 5 * GameManager::getInstance()->getDeltaTime();//*5;
			if(intensityImpact > 0.8)
			{
				intensityImpact = 1;
				upImpacts = false;
			}
		}else{
		
			intensityImpact -= 5 * GameManager::getInstance()->getDeltaTime();
			if(intensityImpact < 0)
			{
				intensityImpact = 0;
			}
		}
		impactList[i*4+2] = upImpacts; //Controla que suba o baje el escudo
		impactList[i*4+3] = intensityImpact ; //Intensidad
	}
}

void ComponentGraphicsShield::sendImpacts()
{
	
	for (int i = 0; i<4;i++)
	{
		impactListTemp[i*4+0] = impactList[i*4+0]; //Posicion X
		impactListTemp[i*4+1] = impactList[i*4+1]; // Posicion y
		impactListTemp[i*4+2] = 0; //Controla que suba o baje el escudo
		impactListTemp[i*4+3] = impactList[i*4+3]; //Intensidad
	}
	//Lo enviamos
	node->setImpactList(impactListTemp);
}

void ComponentGraphicsShield::initImpact()
{
	nextImpact = 0;
	for(int i=0; i < 16; i++)
	{
		impactList[i] = 0;
	}
	sendImpacts();
}
