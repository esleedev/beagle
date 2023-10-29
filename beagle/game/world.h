#pragma once

#include "../graphicsTypes.h"
#include "triggers.h"

struct SpawnPoint;

class World
{
public:
	std::vector<Line2D> lines;
	std::vector<RectangleTrigger> goToTriggers;
	std::vector<SpawnPoint> spawnPoints;

	void LoadWorld(std::string FileName);
};

struct SpawnPoint
{
	std::string eventId;
	uint8_t playerIndex;
	Vector2D position;
};