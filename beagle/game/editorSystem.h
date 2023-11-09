#pragma once

#include "../game.h"

namespace game_systems
{
	enum EditorTools
	{
		Line = 0,
		Piece = 1
	};

	class EditorSystem : public System
	{
		bool wasInitialized = false;
		void InitializeAssetsAndObjects(Game* Game);
	public:
		std::shared_ptr<Object> cursorObject;
		EditorTools tool;
		void Update(float DeltaTime, Game* Game);
	};
}
