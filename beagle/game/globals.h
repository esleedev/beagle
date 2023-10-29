#pragma once

#include "world.h"

namespace game_globals
{
	extern World* world;

	extern TriggerEvent goToEvent;
	extern DynamicMesh* dynamicLinesMesh;
	extern const Uint16 MaximumPlayerCount;

	extern void LoadWorld(std::string FileName);
	extern void UpdateWorldMeshes();

	enum TriggerTypes
	{
		GoToWorld = 0
	};
}