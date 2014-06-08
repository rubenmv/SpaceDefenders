#pragma once

#include <vector>

class Entity;

class SceneNode
{
public:
	SceneNode(void);
	~SceneNode(void);
	
	// --------------------------------------------
	void setEntity(Entity* entity)
	{
		this->entity = entity;
	}

	Entity* getEntity()
	{
		return this->entity;
	}

	void setParent(SceneNode* parent)
	{
		this->parent = parent;
	}

	SceneNode* getParent()
	{
		return this->parent;
	}

	std::vector<SceneNode*> getChildren()
	{
		return this->children;
	}

	// --------------------------------------------
	void addChild(SceneNode* child);
	void removeChild(SceneNode* child);

	void clear();

	// Dibula la entidad asociada y llama al draw de los hijos
	void draw();

	void setVisible(bool visible)
	{
		this->visible = visible;
	}

	bool isVisible()
	{
		return visible;
	}

private:
	Entity* entity;
	SceneNode* parent;
	std::vector<SceneNode*> children;
	bool visible;
};

