#include "triggerSystem.h"
#include "globals.h"
#include "../collisionTests.h"
#include <iostream>
#include <iomanip>

void game_systems::TriggerSystem::Update(float DeltaTime, Game* Game)
{
	if (game_globals::goToEvent.id.size() > 0)
	{
		std::cout << "Go to trigger event:" << std::endl;
		std::cout << std::left << std::setw(12) << "  id: " << game_globals::goToEvent.id << std::endl;
		std::cout << std::left << std::setw(12) << "  value: " << game_globals::goToEvent.value << std::endl;

		game_globals::LoadWorld(game_globals::goToEvent.value);
		game_globals::UpdateWorldMeshes();

		for (int point = 0; point < game_globals::world->spawnPoints.size(); point++)
		{
			if (game_globals::world->spawnPoints[point].eventId == game_globals::goToEvent.id)
			{
				Game->objects[player0ObjectIndex + game_globals::world->spawnPoints[point].playerIndex]->transform.SetPosition(game_globals::world->spawnPoints[point].position);
				std::cout << "Spawn player " << game_globals::world->spawnPoints[point].playerIndex << " at ";
				std::cout << game_globals::world->spawnPoints[point].position.x << ", ";
				std::cout << game_globals::world->spawnPoints[point].position.y << std::endl;
			}
		}

		game_globals::goToEvent = {};
	}
}
