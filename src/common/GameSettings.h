#include <iostream>
#pragma once
class GameSettings
{
public:
	GameSettings(void);
	~GameSettings(void);

	void loadSettings();
	void saveSettings();
	void nextResolution();
	void backResolution();
	int getWidth();
	int getHeight();
	int getColor();
	bool getVolumen();
	bool getSync();
	bool getAliasing();
	bool getFullScreen();
	bool usePhysics();
	bool useAliasing();
	bool isIrrlicht();
	std::wstringstream getResolution();
	void apply();
	void reset();
	void changeVSync();
	void changeFullScreen();
	void changeIrrlicht();
	void changePhysics();
	void changeAliasing();
	void changeVolume();
private:
	int width;
	int height;
	int color;
	bool fullscreen;
	bool sync;
	bool volumen;
	bool tfullscreen;
	bool tsync;
	bool tvolumen;
	int widthList[7];
	int heightList[7];
	int resolution;
	bool irrlicht;
	bool tirrlicht;
	bool physics;
	bool tphysics;
	bool aliasing;
	bool taliasing;
};

