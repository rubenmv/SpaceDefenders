#pragma once

#include <SFML/Audio.hpp>
#include "ResourceSoundBuffer.h"

class NodeSound;

namespace Resources
{
	static const char* AUDIO_MUSIC_INTRO = "../assets/audio/music/00_nice_music.ogg";
	static const char* AUDIO_MUSIC_MENUS = "../assets/audio/music/01_Project_System_12_-_Breath_of_Hope.ogg";
	static const char* AUDIO_MUSIC_GAME_01 = "../assets/audio/music/02_Musician_Toy_-_Inside_the_light.ogg";
}

class AudioManager
{
public:
	AudioManager();
	~AudioManager();
	
	ResourceSoundBuffer* getSoundBuffer( const char* filePath );

	// Cambia la musica segun el estado
	void onStateChange(int state);

	void initBuffers();

	void mute(bool mute);

	bool isMute;

private:
	ResourceSoundBuffer* loadSoundBuffer( const char* filePath );

	std::vector<ResourceSoundBuffer*> bufferList;
	// Ultimo buffer pedido
	ResourceSoundBuffer* lastBuffer;

	// 0 = menus, 1+ = resto de musica de partida
	std::vector< NodeSound* > musicList;
	NodeSound* currentMusic;
	int lastState; // Para saber cuando reiniciar la musica
};
