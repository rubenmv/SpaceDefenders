#pragma once

#include "Entity.h"

#include "Vector2d.h"
#include "Vector3d.h"
#include "Matrix.h"

class GraphicsEngineTAG;

class EntityTransform: public Entity
{
public:
	EntityTransform();
	virtual ~EntityTransform();
	
	// Devuelve la rotacion
	Vector3d getTranslation()
	{
		return translation;
	}

	// Devuelve la rotacion
	Vector3d getRotation()
	{
		return rotation;
	}

	// Devuelve el escalado
	Vector3d getScale()
	{
		return scale;
	}

	// Setters
	// Establece la posicion actual en 3D
	void setTranslation(Vector3d values)
	{
		if(translation != values)
		{
			translation = values;
			dirty = true;
		}
	}

	// Establece la posicion actual en 2D
	void setTranslation(Vector2d values)
	{
		if(translation.x != values.x || translation.z != values.y)
		{
			translation.x = values.x;
			translation.z = values.y;
			dirty = true;
		}
	}

	// Establece la rotacion actual
	void setRotation(Vector3d values)
	{
		if(rotation != values)
		{
			rotation = values;
			dirty = true;
		}
	}

	// Establece el escalado
	void setScale(Vector3d values)
	{
		if(scale != values)
		{
			scale = values;
			dirty = true;
		}
	}

	void setVisible(bool visible)
	{
		this->visible = visible;
	}

	bool isVisible()
	{
		return visible;
	}

	void beginDraw();
	void endDraw();

private:
	GraphicsEngineTAG* graphicsEngine;

	// Matriz con las transformaciones acumuladas
	Matrix<float> transformMatrix;
	// Se utiliza para generar la matriz
	Matrix<float> auxMatrix;
	// Posicion, rotacion y escalado que se va a aplicar en el dibujado
	Vector3d translation;
	Vector3d rotation;
	Vector3d scale;

	//Indica si la matriz de transformacion es obsoleta y toca volver a generarla
	//Solo se hara si se ha modicado en ese frame la posicion,rotacion o el escalado
	bool dirty;
	bool visible;
	// Operaciones sobre la matriz de transformacion
	// Agrega una translacion 3D
	void addTranslation(Vector3d position);
	// Agrega una rotacion
	void addRotation(float angle, char axis);
	// Agrega una rotacion a partir de un vector3d
	void addRotation(Vector3d rotation);
	void addRotationY(float angle);
	// Agrega un escalado a cada eje independientemente
	void addScale(Vector3d factor);
	
	// Multiplica la matriz de transformacion a la de openGL
	void addOpenGLMatrix();
};

