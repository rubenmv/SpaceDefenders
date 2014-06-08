#include "AudioManager.h"

#include "State.h"
#include "NodeSound.h"

#include "GameManager.h"
#include "GameSettings.h"

AudioManager::AudioManager()
{
	lastBuffer = NULL;

	lastState = 0;

	isMute = false;
}

AudioManager::~AudioManager()
{
	lastBuffer = NULL;

	while (!bufferList.empty())
	{
		delete bufferList.back();
		bufferList.pop_back();
	}
}

ResourceSoundBuffer* AudioManager::getSoundBuffer( const char* filePath )
{
	//ResourceSoundBuffer* buffer = (ResourceSoundBuffer*)getResource(filePath);

	bool found = false;
	ResourceSoundBuffer* buffer = NULL;

	// Primero comprobamos si el recurso acaba de ser accedido anteriormente
	if (lastBuffer != NULL)
	{
		if ( strcmp( lastBuffer->fileName.c_str(), filePath ) == 0 )
		{
			found = true;
			buffer = lastBuffer;
		}
	}
	// Si el recurso que se pide es diferente al ultimo que se pidio lo buscamos en el vector
	if (!found && !bufferList.empty())
	{
		for (size_t i = 0; i < bufferList.size() && !found; i++)
		{
			if( strcmp( bufferList[i]->fileName.c_str(), filePath ) == 0 )
			{
				found = true;
				buffer = bufferList[i];
				lastBuffer = buffer;
			}
		}
	}

	if(buffer == NULL)
	{
		buffer = new ResourceSoundBuffer(filePath);
		bufferList.push_back(buffer);
	}

	return buffer;
}

void AudioManager::onStateChange( int state )
{
	// Canciones durante el juego
	if (state >= State::StateName::IN_GAME)
	{
		currentMusic->stop();
		currentMusic = musicList[2];
		currentMusic->play();
	}
	/*
	// Musica de carga
	else if ( state == State::StateName::INTRO )
	{
		currentMusic = musicList[0];
		currentMusic->play();
	}
	*/
	// Musica de menus
	else
	{
		if ( lastState >= State::StateName::IN_GAME || lastState == State::StateName::INTRO  )
		{
			currentMusic->stop();
			currentMusic = musicList[1];
			currentMusic->play();
		}
	}

	lastState = state;
}

void AudioManager::initBuffers()
{
	bool music = GameManager::getInstance()->getGameSettings()->getVolumen();
	
	// Intro aqui en constructor para que se escuche durante la intro
	NodeSound* sound = new NodeSound(Resources::AUDIO_MUSIC_INTRO, this);
	musicList.push_back( sound );
	musicList[0]->setVolume(10.f);
	// Como se carga en el intro, la empezamos a reproducir aqui
	currentMusic = musicList[0];
	if (music)
	{
		currentMusic->play();
	}
	// Menus
	sound = new NodeSound(Resources::AUDIO_MUSIC_MENUS, this);
	musicList.push_back( sound );
	musicList[1]->setVolume(20.f);
	musicList[1]->setLoop(true);
	// Tracks de partida
	// Lo suyo es que estas no usen loop y vaya cambiando de pistas
	sound = new NodeSound(Resources::AUDIO_MUSIC_GAME_01, this);
	musicList.push_back( sound );
	musicList[2]->setVolume(20.f);
	musicList[2]->setLoop(true);

	mute( !music );
}

void AudioManager::mute( bool mute )
{
	isMute = mute;
	for (size_t i = 0; i < musicList.size(); i++)
	{
		musicList[i]->setMute(mute);
	}
}
