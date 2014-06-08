#include "ComponentSound.h"

#include "NodeSound.h"
#include "GameManager.h"
#include "AudioManager.h"

ComponentSound::ComponentSound(float rate, const char* soundPath)
{
	this->rate = rate;
	playing = false;

	AudioManager* audioManager = GameManager::getInstance()->getAudioManager();

	if (audioManager != NULL)
	{
		sound = new NodeSound(soundPath, audioManager);
		sound->setVolume(20);
		sound->setPitch(0.2);
	}
}

ComponentSound::~ComponentSound()
{
	delete sound;
	sound = NULL;
}

void ComponentSound::update()
{
	if (playing)
	{
		if(delay.getElapsedTime() > rate && playing)
		{
			sound->play();
			delay.restart();
		}
	}
}

void ComponentSound::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::START_SHOOTING:
		playing = true;
		break;
	case Message::STOP_SHOOTING:
		playing = false;
		break;
	}
}