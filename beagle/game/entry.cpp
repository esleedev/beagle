#include "../game.h"
#include "../texture.h"
#include "../shader.h"
#include "../mesh.h"
#include "../stringParser.h"
#include <fstream>
#include <iostream>

// define globals
#include "globals.h"
std::vector<Line2D> game_globals::lines;
const Uint16 game_globals::MaximumPlayerCount = 2;

#include "playerSystem.h"
#include "cameraSystem.h"

void LoadWorld(std::string FileName);

void OnGameStart(Game* Game)
{
    int shaderIndex;
    Game->AddNewShader("shaders/vertexShader.txt", "shaders/fragmentShader.txt", shaderIndex);

    GLuint playerTexture = LoadTexture("textures/spriteSheet.png");
    Game->materials.push_back({ playerTexture, shaderIndex });

    Transform playerTransform = {};
    playerTransform.position = { 0.0f, 0.0f, 0.0f };
    playerTransform.shouldUpdateMatrix = true;

    for (Sint16 player = 0; player < game_globals::MaximumPlayerCount; player++)
    {
        SpriteMesh* spriteMesh = Game->AddNewSpriteMesh({ 1.0, 1.0 }, { 0.5, 1.0 }, { 0.5f, 0.5f });
        Game->objects.push_back(new Object{ player, 0, playerTransform, spriteMesh });

        game_systems::PlayerSystem* playerSystem = Game->AddNewSystem<game_systems::PlayerSystem>();
        playerSystem->objectIndex = player;
        playerSystem->deviceIndex = (Sint16)(player - 1);
    }

    auto cameraSystem = Game->AddNewSystem<game_systems::CameraSystem>();
    cameraSystem->player0ObjectIndex = 0;

    LoadWorld("game/testWorld.txt");

    DynamicMesh* dynamicMesh;
    int meshIndex;
    Game->AddMesh(GenerateEmptyMesh(), meshIndex);
    Game->objects.push_back(new Object(meshIndex, 0));

    dynamicMesh = new DynamicMesh(&Game->meshes[meshIndex]);
    Game->dynamicMeshes.push_back(dynamicMesh);

    for (int line = 0; line < game_globals::lines.size(); line++)
        AddLineToMesh(game_globals::lines[line], dynamicMesh);

}

void OnGameEnd(Game* Game)
{

}

void LoadWorld(std::string FileName)
{
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
                    game_globals::lines.push_back(line);
                }
                sectionCount--;
            }
        }
        file.close();
    }
}