#include "ComponentAutoDestroy.h"
#include "GameObject.h"



ComponentAutoDestroy::ComponentAutoDestroy( float autoDestroyTime ) : autoDestroyTime(autoDestroyTime)
{
	clock.restart();
}

void ComponentAutoDestroy::update()
{
	if(clock.getElapsedTime() > autoDestroyTime)
	{
		parent->kill();
	}
}
