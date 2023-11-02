#pragma once

#include "../game.h"

namespace game_systems
{
	class EditorSystem : public System
	{
	public:
		void InitializeObjects(Game* Game);
		void Update(float DeltaTime, Game* Game);
	};
}
