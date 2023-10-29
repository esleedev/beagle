#include "../game.h"
#include "../texture.h"
#include "../shader.h"
#include "../mesh.h"
#include "../stringParser.h"
#include <fstream>
#include <iostream>

// define globals
#include "globals.h"
World* game_globals::world;
TriggerEvent game_globals::goToEvent = {};
DynamicMesh* game_globals::dynamicLinesMesh;
const Uint16 game_globals::MaximumPlayerCount = 2;

#include "playerSystem.h"
#include "cameraSystem.h"
#include "triggerSystem.h"

void OnGameStart(Game* Game)
{
    int shaderIndex;
    Game->AddNewShader("shaders/vertexShader.txt", "shaders/fragmentShader.txt", shaderIndex);

    GLuint playerTexture = LoadTexture("textures/spriteSheet.png");
    Game->materials.push_back({ playerTexture, shaderIndex });

    Transform playerTransform = Transform();

    for (Sint16 player = 0; player < game_globals::MaximumPlayerCount; player++)
    {
        SpriteMesh* spriteMesh = Game->AddNewSpriteMesh({ 1.0, 1.0 }, { 0.5, 1.0 }, { 0.5f, 0.5f });
        Game->objects.push_back(new Object{ player, 0, playerTransform, spriteMesh });

        game_systems::PlayerSystem* playerSystem = Game->AddNewSystem<game_systems::PlayerSystem>();
        playerSystem->objectIndex = player;
        playerSystem->deviceIndex = (Sint16)(player - 1);
    }

    game_systems::TriggerSystem* triggerSystem = Game->AddNewSystem<game_systems::TriggerSystem>();
    triggerSystem->player0ObjectIndex = 0;

    auto cameraSystem = Game->AddNewSystem<game_systems::CameraSystem>();
    cameraSystem->player0ObjectIndex = 0;

    game_globals::LoadWorld("game/testWorld.txt");

    int linesMeshIndex;
    Game->AddMesh(GenerateEmptyMesh(), linesMeshIndex);
    Game->objects.push_back(new Object(linesMeshIndex, 0));

    game_globals::dynamicLinesMesh = new DynamicMesh(&Game->meshes[linesMeshIndex]);
    Game->dynamicMeshes.push_back(game_globals::dynamicLinesMesh);

    game_globals::UpdateWorldMeshes();
}

void OnGameEnd(Game* Game)
{

}

void game_globals::UpdateWorldMeshes()
{
    game_globals::dynamicLinesMesh->vertices.clear();
    game_globals::dynamicLinesMesh->indices.clear();

    for (int line = 0; line < game_globals::world->lines.size(); line++)
        AddLineToMesh(game_globals::world->lines[line], game_globals::dynamicLinesMesh);
}

void game_globals::LoadWorld(std::string FileName)
{
    if (game_globals::world == nullptr)
        game_globals::world = new World();

    game_globals::world->LoadWorld(FileName);
}