
#pragma once

#include "Vector2d.h"
#include "Vector3d.h"

class NodeText
{
public:
	NodeText() {}
	virtual ~NodeText() {}

	virtual void setPosition(Vector2d position) = 0;

	virtual void setText(const wchar_t* text) = 0;
};
