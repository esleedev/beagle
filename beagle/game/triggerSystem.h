#pragma once

#include "../game.h"

namespace game_systems
{
	class TriggerSystem : public System
	{
	public:
		int player0ObjectIndex;
		void Update(float DeltaTime, Game* Game);
	};
}