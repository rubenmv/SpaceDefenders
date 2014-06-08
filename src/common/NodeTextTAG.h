#pragma once

#include "NodeText.h"
#include "Entity.h"
#include "GUIText.h"

#include "GameManager.h"
#include "GraphicsEngine.h"
#include "GUIManager.h"

class NodeTextTAG: public NodeText
{
public:
	// Crea el texto y lo asocia al GUIManager
	NodeTextTAG( GUIText* nodeText, GUIManager* manager )
	{
		this->nodeText = nodeText;
		guiManager = manager;
	}

	virtual ~NodeTextTAG()
	{
		guiManager->removeChildFromId(nodeText->getId());
		nodeText = NULL;
	}

	void setPosition(Vector2d position)
	{
		nodeText->setPosition(position.x , position.y);
	}
	void setText(const wchar_t* text)
	{
		nodeText->setString(text);
	}

private:
	GUIText* nodeText;
	GUIManager* guiManager; // Para eliminar el nodo de texto
};
