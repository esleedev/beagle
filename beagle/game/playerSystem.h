#pragma once

#include "../game.h"

namespace game_systems
{
	class PlayerSystem : public System
	{
	public:
		void Update(float DeltaTime, Game* Game);
	};
}