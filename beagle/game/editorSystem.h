#pragma once

#include "../game.h"

namespace game_systems
{
	class EditorSystem : public System
	{
		bool wasInitialized = false;
		void InitializeAssetsAndObjects(Game* Game);
	public:
		int indicatorObjectIndex;
		void Update(float DeltaTime, Game* Game);
	};
}
