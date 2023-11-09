#pragma once

#include "../game.h"

namespace game_systems
{
	class PlayerSystem : public System
	{
	public:
		std::shared_ptr<Object> object;
		short deviceIndex;
		Vector2D velocity;
		bool isMirrored;

		void Update(float DeltaTime, Game* Game);
	};
}