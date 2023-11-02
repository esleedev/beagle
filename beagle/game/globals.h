#pragma once

#include "world.h"

namespace game_globals
{
	enum AppMode
	{
		Game = 0,
		WorldEditor = 1
	};

	extern AppMode appMode;

	extern World* world;

	extern TriggerEvent goToEvent;
	extern DynamicMesh* dynamicLinesMesh;
	extern DynamicMesh* dynamicPiecesMesh;
	extern const Uint16 MaximumPlayerCount;

	extern void LoadWorld(std::string FileName);
	extern void UpdateWorldMeshes();
}