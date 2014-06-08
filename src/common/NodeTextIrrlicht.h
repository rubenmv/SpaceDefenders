#pragma once

#include "irrlicht/irrlicht.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "NodeText.h"


class NodeTextIrrlicht: public NodeText
{
public:
	NodeTextIrrlicht(irr::gui::IGUIStaticText* irrlichtNode):irrlichtNode(irrlichtNode){};
	virtual ~NodeTextIrrlicht()
	{
		irrlichtNode->remove();
		//delete irrlichtNode;
		irrlichtNode=NULL;
	}

	irr::gui::IGUIStaticText* getIrrlichtNode()
	{
		return(irrlichtNode);
	}

	virtual void setPosition(Vector2d position)
	{
		irr::core::vector2di positionIrr(position.x, position.y);
		irrlichtNode->setRelativePosition(positionIrr);
		
	}

	virtual void setText(const wchar_t* text)
	{
		irrlichtNode->setText(text);
	}
		
private:

	irr::gui::IGUIStaticText* irrlichtNode;
};

