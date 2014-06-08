#pragma once

#include "AudioManager.h"
#include "ResourceSoundBuffer.h"

#include "SFML/Audio.hpp"

class NodeSound
{
public:
	// Para mayor rapidez, se le pasa el AudioManager activo actualmente
	NodeSound(const char* soundPath, AudioManager* audioManager)
	{
		sound = new sf::Sound();
		buffer = audioManager->getSoundBuffer(soundPath);
		buffer->linkSound(sound);
	}

	virtual ~NodeSound()
	{
		stop();
		delete sound;
		sound = NULL;
		buffer = NULL;
	}

	void play()
	{
		sound->play();
	}

	void pause()
	{
		sound->pause();
	}

	void stop()
	{
		sound->stop();
	}

	void setLoop(bool loop)
	{
		sound->setLoop(loop);
	}

	//The value ranges from 0 (mute) to 100 (full volume).
	//The default value is 100, which means that you can't make a sound louder than its initial volume. 
	void setVolume(float level)
	{
		volume = level;
		sound->setVolume(level);
	}

	//The pitch is a factor that changes the perceived frequency of the sound:
	//greater than 1 makes the sound more acute, less than 1 makes the sound more grave,
	//and 1 leaves it unchanged. Changing the pitch has a side effect: it also impacts the playing speed.
	void setPitch(float level)
	{
		sound->setPitch(level);
	}

	void setMute(bool mute)
	{
		if (mute)
		{
			sound->setVolume(0);
		}
		else
		{
			sound->setVolume(volume);
		}
	}

private:
	sf::Sound* sound;
	ResourceSoundBuffer* buffer;
	float volume; // Para cuando se hace mute guardamos el valor anterior
};
