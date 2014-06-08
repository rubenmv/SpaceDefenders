#pragma once
#include "Component.h"
#include "Vector2d.h"

class GameManager;
class GraphicsEngine;
class EventManager;

/*	Clase de testeo para cambiar la posicion del objeto Target
	directamente sin utilizar ningun tipo de movimiento
*/
class ComponentTargetPosition :	public Component
{
public:
	ComponentTargetPosition(void);
	virtual ~ComponentTargetPosition(void);

	virtual void update();
	virtual void onMessage(const Message& message);
private:
	void changePosition(Vector2d newTarget);
};

