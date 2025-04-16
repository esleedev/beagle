#pragma once

#include "common_types.h"

namespace esl
{
	class SoundEffectChannelFlags
	{
	public:
		static const int Channel0 = 1;
		static const int Channel1 = 2;
		static const int Channel2 = 4;
		static const int Channel3 = 8;
		static const int Channel4 = 16;
		static const int Channel5 = 32;
		static const int Channel6 = 64;
		static const int Channel7 = 128;
	};

	struct SoundEffectChannelState
	{
		esl::uint lastPlayedTime;
	};

	struct SoundEffect;

	class Audio
	{
		const int SoundEffectChannelFlags[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };
	public:
		esl::SoundEffectChannelState soundEffectChannelStates[8];

		void PlaySoundEffect(esl::SoundEffect const SoundEffect, int ChannelFlags);
	};
}