#pragma once

#include "Resource.h"

#include <SFML/Audio.hpp>
#include <iostream>

class ResourceSoundBuffer : public Resource
{
public:
	ResourceSoundBuffer(const char* filePath)
	{
		fileName = filePath;

		if (!soundBuffer.loadFromFile(filePath))
		{
			std::cout << "Hubo un problema cargando el sonido: " << filePath << std::endl;
		}
	}

	virtual~ResourceSoundBuffer()
	{
	}

	void linkSound(sf::Sound* sound)
	{
		sound->setBuffer(this->soundBuffer);
	}

private:
	sf::SoundBuffer soundBuffer;
};
