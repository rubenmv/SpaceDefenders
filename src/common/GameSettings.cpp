#include "GameSettings.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"
#include "rapidjson/stringbuffer.h"
#include <fstream>
#include <sstream>  

GameSettings::GameSettings(void)
{
	// Opciones por defecto
	width = 1024;
	height = 768;
	color = 16;
	tfullscreen = fullscreen = false;
	tsync = sync = true;
	tvolumen = volumen = true;
	resolution = 0;
	irrlicht = true;
	physics = tphysics = true;
	aliasing = taliasing = true;

	widthList[0] = 1024;
	widthList[1] = 1280;
	widthList[2] = 1360;
	widthList[3] = 1366;
	widthList[4] = 1600;
	widthList[5] = 1680;
	widthList[6] = 1920;
	
	heightList[0] = 768;
	heightList[1] = 720;
	heightList[2] = 1024;
	heightList[3] = 768;
	heightList[4] = 900;
	heightList[5] = 1050;
	heightList[6] = 1080;
}

GameSettings::~GameSettings(void)
{

}

void GameSettings::loadSettings()
{
	std::stringstream data;
	std::ifstream file;
	file.open("../data/settings.json", std::ios::binary);
	data << file.rdbuf();
	file.close();
	rapidjson::Document document;

	try
	{
		if(document.Parse<0>(data.str().c_str()).HasParseError())
			throw std::invalid_argument("json parse error");
	}catch(std::invalid_argument a)
	{
		std::cerr<<"Error en el json, carga de valores por defecto\n";
		width = 1280;
		height = 720;
		color = 16;
		tsync = sync = true;
		tvolumen = volumen = true;
		tfullscreen = fullscreen = false;
		tirrlicht = irrlicht = true;

		return;
	}

	try
	{
	if(document.HasMember("width"))
	{
		if(document["width"].IsInt())
		{
			width = document["width"].GetInt();
		}
	}
	else
	{
		width = 1280;
	}
	
	if(width < 1280)
	{
		width = 1024;
		resolution = 0;
	}

	else if(width < 1360)
	{
		width = 1280;
		resolution = 1;
	}

	else if(width < 1366)
	{
		width = 1360;
		resolution = 2;
	}

	else if(width < 1600)
	{
		width = 1366;
		resolution = 3;
	}

	else if(width < 1680)
	{
		width = 1600;
		resolution = 4;
	}

	else if(width < 1920)
	{
		width = 1680;
		resolution = 5;
	}

	else if(width >= 1920)
	{
		width = 1920;
		resolution = 6;
	}

	if(document.HasMember("height"))
	{
		if(document["height"].IsInt())
			height = document["height"].GetInt();
	}
	else
	{
		height = 720;
	}
	if(height != heightList[resolution])
	{
		height = heightList[resolution];
	}

	if(document.HasMember("color"))
	{
		if(document["color"].IsInt())
			color = document["color"].GetInt();
	}
	else
	{
		color = 16;
	}

	if(document.HasMember("fullscreen"))
	{
		if(document["fullscreen"].IsBool())
			tfullscreen = fullscreen = document["fullscreen"].GetBool();
	}
	else
	{
		tfullscreen = fullscreen = false;
	}

	if(document.HasMember("sync"))
	{
		if(document["sync"].IsBool())
			tsync = sync = document["sync"].GetBool();
	}
	else
	{
		tsync = sync = true;
	}

	if(document.HasMember("physics"))
	{
		if(document["physics"].IsBool())
			tphysics = physics = document["physics"].GetBool();
	}
	else
	{
		physics = tphysics = true;
	}

	if(document.HasMember("aliasing"))
	{
		if(document["aliasing"].IsBool())
			taliasing = aliasing = document["aliasing"].GetBool();
	}
	else
	{
		aliasing = aliasing = true;
	}

	if(document.HasMember("irrlicht"))
	{
		if(document["irrlicht"].IsBool())
			tirrlicht = irrlicht = document["irrlicht"].GetBool();
	}
	else
	{
		tirrlicht = irrlicht = true;
	}

	if(document.HasMember("music"))
	{
		if(document["music"].IsBool())
			tvolumen = volumen = document["music"].GetBool();
	}
	else
	{
		tirrlicht = irrlicht = true;
	}

	}catch(std::invalid_argument a)
	{
		std::cerr<<"ERERERERERERERE\n";
	}
}

void GameSettings::saveSettings()
{
	std::ofstream file;
	file.open("../data/settings.json", std::ios::binary);

	std::stringstream test;

	test << "{\"width\":" << width 
		 << ",\"height\":" << height
		 << ",\"color\":" << color 
		 << ",\"fullscreen\":";

	// Fullscreen
	if(fullscreen)
	{
		test<<"true";
	}
	else
	{
		test<<"false";
	}
	test<<",\"sync\":";
	// VSync
	if(sync)
	{
		test<<"true";
	}
	else
	{
		test<<"false";
	}
	test<<",\"physics\":";
	// Physics
	if(physics)
	{
		test<<"true";
	}
	else
	{
		test<<"false";
	}
	test<<",\"aliasing\":";
	// Aliasing
	if(aliasing)
	{
		test<<"true";
	}
	else
	{
		test<<"false";
	}
	test<<",\"irrlicht\":";
	// Irrlicht
	if(irrlicht)
	{
		test<<"true";
	}
	else
	{
		test<<"false";
	}
	test<<",\"music\":";
	// Music
	if(volumen)
	{
		test<<"true}";
	}
	else
	{
		test<<"false}";
	}

	file.write( test.str().c_str(), test.str().size() );

	file.close();
}

void GameSettings::nextResolution()
{
	resolution++;
	if(resolution > 6)
	{
		resolution = 0;
	}
}


void GameSettings::backResolution()
{
	resolution--;
	if(resolution < 0)
	{
		resolution = 6;
	}
}


void GameSettings::changeFullScreen()
{
	tfullscreen = !tfullscreen;
}

void GameSettings::changeVSync()
{
	tsync = !tsync;
}

void GameSettings::changeIrrlicht()
{
	tirrlicht = !tirrlicht;
}

void GameSettings::changePhysics()
{
	tphysics = !tphysics;
}

void GameSettings::changeAliasing()
{
	taliasing = !taliasing;
}

void GameSettings::changeVolume()
{
	tvolumen = !tvolumen;
}

std::wstringstream GameSettings::getResolution()
{
	std::wstringstream cad;
	cad << widthList[resolution];
	cad << "x";
	cad << heightList[resolution];

	return cad;
}

int GameSettings::getWidth()
{
	return width;
}

int GameSettings::getHeight()
{
	return height;
}

int GameSettings::getColor()
{
	return color;
}

bool GameSettings::getVolumen()
{
	return volumen;
}

bool GameSettings::getFullScreen()
{
	return fullscreen;
}

bool GameSettings::getSync()
{
	return sync;
}

bool GameSettings::getAliasing()
{
	return aliasing;
}

bool GameSettings::usePhysics()
{
	return physics;
}

bool GameSettings::useAliasing()
{
	return aliasing;
}


bool GameSettings::isIrrlicht()
{
	return irrlicht;
}

void GameSettings::apply()
{
	fullscreen = tfullscreen;
	sync = tsync;
	irrlicht = tirrlicht;
	volumen = tvolumen;
	width = widthList[resolution];
	height = heightList[resolution];
	physics = tphysics;
	aliasing = taliasing;
}

void GameSettings::reset()
{
	tfullscreen = fullscreen;
	tsync = sync;
	tvolumen = volumen;
	tirrlicht = irrlicht;
	tphysics = physics;
	taliasing = aliasing;

	if(width < 1280)
	{
		width = 1024;
		resolution = 0;
	}

	else if(width < 1360)
	{
		width = 1280;
		resolution = 1;
	}

	else if(width < 1366)
	{
		width = 1360;
		resolution = 2;
	}

	else if(width < 1600)
	{
		width = 1366;
		resolution = 3;
	}

	else if(width < 1680)
	{
		width = 1600;
		resolution = 4;
	}

	else if(width < 1920)
	{
		width = 1680;
		resolution = 5;
	}

	else if(width >= 1920)
	{
		width = 1920;
		resolution = 6;
	}
}