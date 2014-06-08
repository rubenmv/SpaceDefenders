#include "EntityTransform.h"

#include "Math.h"

#include "SceneManager.h"
#include "GameManager.h"
#include "GraphicsEngineTAG.h"

// Primero glew y luego OpenGL
#include <glew/glew.h>
#include <SFML/OpenGL.hpp>


EntityTransform::EntityTransform()
{
	//No hace falta hacerlo, ya lo hace automaticamente
	//transformMatrix = Matrix<float>();
	//auxMatrix = Matrix<float>();

	// Cargamos la identidad
	transformMatrix.setIdentity();

	setScale(1);
	setRotation(Vector3d(0,0,0));
	setTranslation(Vector3d(0,0,0));
	setTranslation(Vector2d(0,0));

	//La primera vez tiene que generar la matriz de transformacion
	dirty = true;
	visible = true;
	// Recogemos el graphics engine
	graphicsEngine = (GraphicsEngineTAG*)GameManager::getInstance()->getGraphicsEngine();
}

EntityTransform::~EntityTransform()
{
	graphicsEngine = NULL;
}

void EntityTransform::beginDraw()
{
	SceneManager::PassMode passMode = graphicsEngine->getSceneManager()->passMode;

	//Si se ha modificado algo, toca generarla de nuevo
	if(dirty)
	{
		// Aplicamos la identidad primero ya que la traslacion indica directamente
		// la posicion final, no un desplazamiento
		transformMatrix.setIdentity();
		addTranslation(translation);
		addRotation(rotation);
		addScale(scale);
	}
	
		
	// Solo apilamos en el dibujado de mallas, etc
	if (passMode == SceneManager::PassMode::OBJECTS)
	{
		// Apilar matriz actual
		glPushMatrix();
	}
	// Camaras, aplicamos la inversa
	else if (passMode == SceneManager::PassMode::CAMERAS)
	{
		if(dirty)
		{
			transformMatrix = transformMatrix.inverse();
		}
	}

	// La posicion de las luces se aplican en la entidad de la hoja
	if (passMode != SceneManager::PassMode::LIGHTS)
	{
		// Para camaras y resto de objetos
		// Multiplicar matriz actual por la de transformacion
		addOpenGLMatrix();
	}

	dirty = false;
}

void EntityTransform::endDraw()
{
	SceneManager::PassMode passMode = graphicsEngine->getSceneManager()->passMode;
	if (passMode == SceneManager::PassMode::OBJECTS)
	{
		// Desapilar matriz
		glPopMatrix();
	}
}

// Agrega una translacion 3D
void EntityTransform::addTranslation(Vector3d position)
{
	//auxMatrix.setIdentity();

	transformMatrix(1,4) = -position.x;
	transformMatrix(2,4) = position.y;
	transformMatrix(3,4) = position.z;

	//transformMatrix = transformMatrix * auxMatrix;
}

// Agrega una rotacion
void EntityTransform::addRotation(float angle, char axis)
{
	//10% de mejora de rendimiento
	if(angle == 0)
	{
		return;
	}

	angle *= Math::DEGTORAD; // Radianes

	float cosAngle = Math::cos(angle);
	float sinAngle = Math::sin(angle);

	switch (axis)
	{
	case 'x':
		auxMatrix.setIdentity();

		auxMatrix(2,2) = cosAngle;
		auxMatrix(2,3) = sinAngle * -1;
		auxMatrix(3,2) = sinAngle;
		auxMatrix(3,3) = cosAngle;
		transformMatrix = transformMatrix * auxMatrix;

		break;
// 	case 'y':
// 		transformMatrix(1,1) = cosAngle;
// 		transformMatrix(3,1) = sinAngle * -1;
// 		transformMatrix(1,3) = sinAngle;
// 		transformMatrix(3,3) = cosAngle;
// 		break;
	case 'z':
		auxMatrix.setIdentity();
		auxMatrix(1,1) = cosAngle;
		auxMatrix(1,2) = sinAngle * -1;
		auxMatrix(2,1) = sinAngle;
		auxMatrix(2,2) = cosAngle;
		transformMatrix = transformMatrix * auxMatrix;

		break;
	}

}

// Agrega una rotacion a partir de un vector3d
void EntityTransform::addRotation(Vector3d rotation)
{
	// Se realizan 3 rotaciones
	addRotationY(-rotation.y); //Es distinta por optimizacion
	addRotation(rotation.x, 'x');
	addRotation(rotation.z, 'z');
}

// Agrega un escalado a cada eje independientemente
void EntityTransform::addScale(Vector3d factor)
{
	//10% de mejora de rendimiento
	if(factor.x == 1 && factor.y == 1 && factor.z == 1)
	{
		return;
	}
	auxMatrix.setIdentity();

	auxMatrix(1,1) = factor.x;
	auxMatrix(2,2) = factor.y;
	auxMatrix(3,3) = factor.z;

	transformMatrix = transformMatrix * auxMatrix;
}

void EntityTransform::addOpenGLMatrix()
{
	// Convierte la matriz a formato GLfLoat
	GLfloat matrix[16];
	int num_celda = 0;
	for(int i = 1; i <= 4; i++)
	{
		for(int j = 1; j <= 4; j++)
		{
			matrix[num_celda] = transformMatrix(j,i);
			num_celda++;
		}
	}

	//glMultTransposeMatrixf(matrix); // casca :(
	glMultMatrixf(matrix);
}

void EntityTransform::addRotationY( float angle )
{
	//10% de mejora de rendimiento
	if(angle == 0)
	{
		return;
	}

	angle *= Math::DEGTORAD; // Radianes

	float cosAngle = Math::cos(angle);
	float sinAngle = Math::sin(angle);
	transformMatrix(1,1) = cosAngle;
	transformMatrix(3,1) = sinAngle * -1;
	transformMatrix(1,3) = sinAngle;
	transformMatrix(3,3) = cosAngle;
}
