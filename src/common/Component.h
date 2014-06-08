#pragma once

#include "Message.h"

class GameObject;

class Component
{
public:
	Component(){};
	virtual ~Component(){};

	//Se llama al inicializar el gameObject, justo despues de inicializar todos los componentes, ejemplo en componentHealth
	virtual void awake(){};

	virtual void setParent(GameObject* parent)
	{
		this->parent = parent;
	}

	virtual void update() = 0;

	virtual void onMessage(const Message& message) = 0;
protected:
	GameObject* parent;
};
