#pragma once

#include "Vector2d.h"

/*	Clase nodo para el pathfinding de tipo A*
*/
class NodeAS
{
public:
	NodeAS();
	NodeAS(int id, NodeAS* parent, Vector2d position);
	virtual ~NodeAS();

	int id;
	NodeAS* parent;
	int g; // Coste desde origen a casilla candidata
	int h; // Coste estimado desde origen a destino
	int f; // Coste total G + H
	Vector2d center; // Coordenada central del nodo

	int getId()
	{
		return id;
	}
	int getG()
	{
		return g;
	}
	int getH()
	{
		return h;
	}
	int getF()
	{
		return f;
	}
	Vector2d getCenter()
	{
		return center;
	}
	NodeAS* getParent()
	{
		return parent;
	}

	void setId(int id)
	{
		this->id = id;
	}
	void setParent(NodeAS* parent)
	{
		this->parent = parent;
	}
	void setCenter(Vector2d center)
	{
		this->center = center;
	}
	void setG(int g)
	{
		this->g = g;
	}
	void setH(int h)
	{
		this->h = h;
	}
	void setF()
	{
		f = g + h;
	}

	// Convierte las coordenadas al cuarto cuadrante (x, -y)
	void normalizeCenter();
};


