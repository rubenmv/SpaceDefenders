#include "ComponentDebugPathfinding.h"
#include <iostream>
#include "GameManager.h"
#include "Math.h"
#include <sstream>
#include "EventManager.h"
#include "DebugTools.h"

ComponentDebugPathfinding::ComponentDebugPathfinding(void)
{
	debugTools = GameManager::getInstance()->getDebugTools();
	eventManager = GameManager::getInstance()->getEventManager();
}

ComponentDebugPathfinding::~ComponentDebugPathfinding(void)
{
	debugTools = NULL;
	eventManager = NULL;
}

void ComponentDebugPathfinding::update()
{
	// Realiza todos los pasos del pathfinding
	if(eventManager->isKeyDown(KEY::KEY_F2))
	{
		debugTools->allSteps = !debugTools->allSteps;

		debugTools->textSteps = "Modo paso a paso: ";
		debugTools->allSteps ? debugTools->textSteps += "INACTIVO\nPulsa F2 para activar" : debugTools->textSteps += "ACTIVO\nPulsa ESPACIO para ver el siguiente paso.";
	}
	
	// Realiza un paso del pathfinding si allSteps esta desactivado
	if(eventManager->isKeyDown(KEY::KEY_SPACE))
	{
		debugTools->nextStep = true;
	}

	debugTools->updateTextPathfinding();
}

void ComponentDebugPathfinding::onMessage(const Message& message)
{

}

