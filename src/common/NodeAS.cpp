#include <iostream>
#include "NodeAS.h"

NodeAS::NodeAS()
{
	id = 0;
	parent = 0;
	g = 0;
	h = 0;
	f = g + h;
	center = Vector2d(0, 0);
}

NodeAS::NodeAS(int id, NodeAS* parent, Vector2d position)
{
	this->id = id;
	this->parent = parent;
	this->center = position;

	if (this->parent == NULL)
	{
		this->g = 0;
	}
	else
	{
		this->g = this->parent->g + 1; // Aqui sumar distancia entre centros
	}
	
	this->h = 0; // Se calcula despues
	this->f = g + h; // Se ha de recalcular despues
	
}

NodeAS::~NodeAS()
{
}

// Convierte las coordenadas al cuarto cuadrante
void NodeAS::normalizeCenter()
{
	if(center.y > 0)
	{
		center.y *= -1;
	}
}


