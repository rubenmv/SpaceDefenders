#pragma once
#include "Component.h"

class EventManager;
class GameObject;

class ComponentInput : public Component
{
public:
	ComponentInput(float speed);
	virtual ~ComponentInput(void);

	virtual void update();
	virtual void onMessage(const Message& message);

	void handleDeath();
	void repairShip();
private:
	EventManager* eventManager;
	float speed;
	bool shooting;
	bool down;
	bool debugMode;

	void handleShootingInput();
	void applyWallForces();
	void handleSkillsInput();
	void handleMovementInput();
	void handleDebugMode();
	void startShooting();
	void stopShooting();

};

