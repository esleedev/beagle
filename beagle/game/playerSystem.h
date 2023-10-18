#pragma once

#include "../game.h"

namespace game_systems
{
	class PlayerSystem : public System
	{
	public:
		Uint16 objectIndex;
		Sint16 deviceIndex;
		Vector2D velocity;
		void Update(float DeltaTime, Game* Game);
	};
}