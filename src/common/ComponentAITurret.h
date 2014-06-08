#pragma once
#include "Component.h"
#include <list>

class ComponentAITurret : public Component
{
public:
	ComponentAITurret(void);
	~ComponentAITurret(void);

	void update();
	void onMessage(const Message& message);
	void awake(GameObject* gameObject);

private:
	void addObject(GameObject* gameObject);
	void deleteObject(GameObject* targetObject);
	void selectAim();
	bool watching;
	//Devuelve true si target1 tiene mayor pref que target 2
	//bool selectPref(GameObject* target1,GameObject* target2);
	std::list<GameObject*> onVisionList;	
	void messageStopAIM();
};

