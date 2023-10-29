#include "world.h"
#include <fstream>
#include "../stringParser.h"

void World::LoadWorld(std::string FileName)
{
    lines.clear();
    goToTriggers.clear();
    spawnPoints.clear();

    std::ifstream file;
    file.open(FileName);
    if (file.is_open())
    {
        std::string line;
        std::string sectionName;
        int sectionCount = 0;
        while (std::getline(file, line))
        {
            std::vector<std::string> values;

            if (sectionCount <= 0)
            {
                FindValuesInLine(line, values, ' ');
                sectionName = values[0];
                sectionCount = std::stoi(values[1]);
            }
            else
            {
                FindValuesInLine(line, values, ',');
                if (sectionName == "lines" && values.size() >= 4)
                {
                    Line2D line = {};
                    line.pointA = { std::stof(values[0]), std::stof(values[1]) };
                    line.pointB = { std::stof(values[2]), std::stof(values[3]) };
                    line.width = 0.075f;
                    lines.push_back(line);
                }
                else if (sectionName == "go_to" && values.size() >= 6)
                {
                    RectangleTrigger trigger;
                    trigger.area.x = std::stof(values[0]);
                    trigger.area.y = std::stof(values[1]);
                    trigger.area.width = std::stof(values[2]);
                    trigger.area.height = std::stof(values[3]);
                    trigger.value = values[4];
                    trigger.id = values[5];

                    goToTriggers.push_back(trigger);
                }
                else if (sectionName == "spawn_points" && values.size() >= 4)
                {
                    SpawnPoint spawnPoint;
                    spawnPoint.eventId = values[0];
                    spawnPoint.playerIndex = std::stoi(values[1]);
                    spawnPoint.position = { std::stof(values[2]), std::stof(values[3]) };

                    spawnPoints.push_back(spawnPoint);
                }

                sectionCount--;
            }
        }
        file.close();
    }
}
