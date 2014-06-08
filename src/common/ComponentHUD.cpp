#include "ComponentHUD.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "AIEngine.h"
#include "EventManager.h"
#include "GameObjectManager.h"
#include "FXEngine.h"

ComponentHUD::ComponentHUD(int jade, Vector2d health, Vector2d shield, int waves, int level, int exp, int nextExp)
{
	data.jade = jade;
	data.health = health;
	data.shield = shield;
	data.waves = waves;
	data.level = level;
	data.exp = exp;
	data.nextExp = nextExp;

	eventManager = GameManager::getInstance()->getEventManager();
	fxEngine = GameManager::getInstance()->getGraphicsEngine()->getFXEngine();
}


ComponentHUD::~ComponentHUD(void)
{
	fxEngine = NULL;
	eventManager = NULL;
}

void ComponentHUD::update()
{
	//Añade el cambio de ronda
	//data.waves = GameManager::getInstance()->getAIEngine()->getLevelRound();
	GameManager::getInstance()->getGraphicsEngine()->setHudData(data);
	
	if(!GameManager::getInstance()->getNetworkEngine()->isStarted())
	{
		data.waves = GameManager::getInstance()->getAIEngine()->getLevelRound();
	}
}

void ComponentHUD::onMessage(const Message& message)
{
	float perc = 0.f;
	switch (message.type)
	{
	case Message::HEALTH_CHANGED:
		{
			// La vida es cero y ahora se le revive
			if (data.health.x <= 0 && message.target.x > 0 )
			{
				fxEngine->damageEffect( false ); // El efecto desaparece con el tiempo
			}
			else
			{
				// Segun el porcentaje de vida...
				perc = message.target.x / message.target.y;

				// Valores entre 0 y 1
				if ( perc <= 0 )
				{
					fxEngine->damageEffect( true ); // El efecto es permanente
				}
				else if ( perc < 0.4 )
				{
					fxEngine->damageEffect( false ); // El efecto desaparece con el tiempo
				}
			}
			
		data.health = message.target;
		break;
		}
	case Message::SHIELD_CHANGED:
		data.shield = message.target;
		break;
	case Message::JADE_CHANGED:
		data.jade = message.value;
		break;
	case Message::EXPERIENCE_CHANGED:
		data.exp = message.value;
		break;
	case Message::LEVEL_CHANGED:
		data.level = message.value;
		break;
	case Message::NEXT_EXPERIENCE_CHANGED:
		data.nextExp = message.value;
		break;
	case Message::NEXT_WAVE:
		data.waves = message.value;		
		break;
	}
}
