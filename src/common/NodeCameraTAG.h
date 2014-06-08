#pragma once
#include "Vector3d.h"
#include "NodeCamera.h"
#include "Entity.h"
#include "Math.h"

class GameObject;
class NodeCameraTAG: public NodeCamera
{
public:
	NodeCameraTAG() {}
	virtual~NodeCameraTAG() {}

	void setPosition(Vector3d position)
	{
		this->position = position;
		EntityTransform* transform = (EntityTransform*)node->getParent()->getEntity();
		transform->setTranslation(position);
	}

	void setTargetPosition(Vector3d targetPosition)
	{
		Vector3d direction =  this->position - targetPosition;
		Vector3d angle = direction.getSphericalCoordinateAngles();

		angle.x -= 90;
		angle.z = 0;

// 		angle.x = Math::atan2(direction.y, direction.z);
// 		angle.y = Math::atan2(direction.z * Math::cos(angle.x), direction.z);
// 		angle.z = Math::atan2(Math::cos(angle.x), Math::sin(angle.x) * Math::sin(angle.y));

		EntityTransform* transform = (EntityTransform*)node->getParent()->getEntity();
		transform->setRotation(angle);
	}

	void setNode(SceneNode* node)
	{
		this->node = node;
	}

private:
	SceneNode* node;
	Vector3d position;
};
