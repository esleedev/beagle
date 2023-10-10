#pragma once

#include "../game.h"

namespace game_systems
{
	class PlayerSystem : public System
	{
	public:
		Vector2D velocity;
		void Update(float DeltaTime, Game* Game);
	};
}