#pragma once

#include "../game.h"

namespace game_systems
{
	class CameraSystem : public System
	{
	public:
		Uint16 player0ObjectIndex;
		void Update(float DeltaTime, Game* Game);
	};
}
