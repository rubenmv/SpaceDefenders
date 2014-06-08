#pragma once

#include "irrlicht\Irrlicht.h"
#include "Vector3d.h"
#include "NodeCamera.h"

class GameObject;

class NodeCameraIrrlicht: public NodeCamera
{
public:
	NodeCameraIrrlicht(irr::scene::ICameraSceneNode* irrlichtNode):irrlichtNode(irrlichtNode){}
	virtual ~NodeCameraIrrlicht()
	{	
		irrlichtNode->remove();
	}
	virtual irr::scene::ICameraSceneNode* getIrrlichtNode()
	{
		return(irrlichtNode);
	}

	virtual void setPosition(Vector3d position)
	{
		irrlichtNode->setPosition(position);
	}

	virtual void setTargetPosition(Vector3d targetPosition)
	{
		irrlichtNode->setTarget(targetPosition);
	}

private:

	irr::scene::ICameraSceneNode* irrlichtNode;
};

