#pragma once
#include "Vector2d.h"
#include <vector>


class Path
{
public:
	Path(Vector2d start, Vector2d end);
	~Path(void);

	void addNextPath(Path* path)
	{
		pathList.push_back(path);
		
	}

	Vector2d getStart()
	{
		return start;
	}

	Vector2d getEnd()
	{
		return end;
	}

	std::vector<Path*> getPathList()
	{
		return pathList;
	}
	void Path::update();

	int getNumberOfEnemies()
	{

		return numberOfEnemies;
	}

	void addEnemy();
	void deleteEnemy();

	void addStruct();
	void deleteStruct();

	bool getPreference()
	{
		return preference;
	}

	void setPreference(bool preference)
	{
		this->preference = preference;
	}
	int getNumberOfStructs()
	{
		return numberOfStructs;
	}
private:
	int numberOfStructs;
	bool preference; //indica si tiene preferencia el camino;
	int numberOfEnemies;	
	Vector2d start;
	Vector2d end;
	std::vector<Path*> pathList;

	
};

