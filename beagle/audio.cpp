#include <SDL_mixer.h>

#include "resource_types.h"
#include "audio.h"

void esl::Audio::PlaySoundEffect(esl::SoundEffect const SoundEffect, int ChannelFlags)
{
	int foundChannel = -1;

	// find a channel that's not playing
	for (int channel = 0; channel < 8; channel++)
	{
		if ((ChannelFlags & SoundEffectChannelFlags[channel]) == SoundEffectChannelFlags[channel] && Mix_Playing(channel) == 0)
		{
			foundChannel = channel;
			break;
		}
	}

	// couldn't find a free channel so find a playing channel that was the oldest played
	if (foundChannel == -1)
	{
		int oldestPlayedTimeChannel = -1;
		esl::uint oldestPlayedTime = SDL_GetTicks();
		for (int channel = 0; channel < 8; channel++)
		{
			if ((ChannelFlags & SoundEffectChannelFlags[channel]) == SoundEffectChannelFlags[channel] && Mix_Playing(channel) == 1 && soundEffectChannelStates[channel].lastPlayedTime <= oldestPlayedTime)
			{
				oldestPlayedTime = soundEffectChannelStates[channel].lastPlayedTime;
				foundChannel = channel;
			}
		}
		if (foundChannel >= 0)
			Mix_HaltChannel(foundChannel);
	}

	if (foundChannel >= 0)
	{
		soundEffectChannelStates[foundChannel].lastPlayedTime = SDL_GetTicks();
		Mix_PlayChannel(foundChannel, SoundEffect.audioChunk, 0);
	}
}
