#include "SceneNode.h"
#include "Entity.h"
// Debug
#include "SceneManager.h"
#include <iostream>

SceneNode::SceneNode(void)
{
	visible = true;
	//children.clear();
	//SceneManager::nodeCount++;
	//std::cout << "Nodos: " << SceneManager::nodeCount << std::endl;
}

// A este destructor solo se llama en el caso en que este nodo no tenga hijos
// por lo que hay que ocuparse de los hijos antes mediante clear
SceneNode::~SceneNode(void)
{
	if (parent != NULL)
	{
		// Le indica al padre que lo borre de la lista
		parent->removeChild(this);
		parent = NULL;
	}

	// Elimina la entidad a la que apunta
	if (entity != NULL)
	{
		delete entity;
		entity = NULL;
	}

	//SceneManager::nodeCount--;
	//std::cout << "Nodos: " << SceneManager::nodeCount << std::endl;
}

void SceneNode::addChild(SceneNode* child)
{
	children.push_back(child);
}

void SceneNode::removeChild(SceneNode* child)
{
	std::vector<SceneNode*>::iterator iterator = children.begin();

	while( iterator != children.end() )
	{
		SceneNode* node = *iterator;
		if(node == child)
		{
			children.erase(iterator);
			break;
		}
		else
		{
			++iterator;
		}
	}
	
	if ( children.empty() && parent != NULL ) // si es el root (parent es NULL), no queremos que ese se borre
	{
		delete this;
	}
}

// Dibuja la entidad asociada
void SceneNode::draw()
{
	if (entity != NULL && visible)
	{
		entity->beginDraw();
		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->draw();
		}
		entity->endDraw();
	}
}

void SceneNode::clear()
{
	// Si no tiene mas hijos, se elimina
	if ( children.empty() && parent != NULL ) // si es el root (parent es NULL) y no queremos que ese se borre
	{
		delete this;
	}
	else // Si tiene hijos, llama al clear de cada uno
	{
		while (!children.empty())
		{
			children.back()->clear();
		}
	}
}
