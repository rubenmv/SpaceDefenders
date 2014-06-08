#pragma once
#include "Component.h"
#include "Vector2d.h"
#include <list>

struct MovemementSnapshot
{
	Vector2d position;
	float rotation;
	unsigned int time;
	bool death;

	MovemementSnapshot(Vector2d position = Vector2d(), float rotation = 0, unsigned int time= 0): position(position), rotation(rotation), time(time), death(false){}
	MovemementSnapshot(unsigned int time): position(Vector2d()), rotation(0), time(time), death(true){}
};

//Interpola la posicion de los dos ultimos paquetes recibidos.
class ComponentMovementInterpolation : public Component
{
public:
	ComponentMovementInterpolation();
	~ComponentMovementInterpolation(){};

	void update();
	void onMessage(const Message& message);
	void onNewSnapshot( Vector2d newPosition, float newRotation, unsigned int snapshotTime );
	void onDeathSnapshot( unsigned int time );
private:
	static const int INTERPOLATION_RATIO = 2;
	static const int MAX_HISTORY_LENGTH = 6;

	std::list<MovemementSnapshot> snapshotHistory;

};


