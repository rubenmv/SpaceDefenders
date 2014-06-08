#pragma once
#include "irrlicht\Irrlicht.h"
#include "Vector3d.h"

class GameObject;
class NodeCamera
{
public:
	NodeCamera(){};
	virtual ~NodeCamera(){};

	virtual void setPosition(Vector3d position) = 0;
	virtual void setTargetPosition(Vector3d targetPosition) = 0;
};
