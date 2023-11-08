#include "../game.h"
#include "../texture.h"
#include "../shader.h"
#include "../mesh.h"
#include "../stringParser.h"
#include <fstream>
#include <iostream>

// define globals
#include "globals.h"
game_globals::AppMode game_globals::appMode = game_globals::AppMode::Game;
World* game_globals::world;
TriggerEvent game_globals::goToEvent = {};
DynamicMesh* game_globals::dynamicLinesMesh;
DynamicMesh* game_globals::dynamicPiecesMesh;
const Uint16 game_globals::MaximumPlayerCount = 2;

#include "editorSystem.h"
#include "playerSystem.h"
#include "cameraSystem.h"
#include "triggerSystem.h"

void OnGameStart(Game* Game)
{
    int shaderIndex, checkerboardShaderIndex;
    Game->AddNewShader("shaders/vertexShader.txt", "shaders/fragmentShader.txt", shaderIndex);
    Game->AddNewShader("shaders/checkerboardVertexShader.txt", "shaders/fragmentShader.txt", checkerboardShaderIndex);

    GLuint playerTexture = LoadTexture("textures/spriteSheet.png");
    short playerMaterial;
    Game->AddMaterial(playerTexture, shaderIndex, playerMaterial);

    GLuint checkerboardTexture = LoadTexture("textures/checkerboard.png");
    short checkerboardMaterial;
    Game->AddMaterial(checkerboardTexture, checkerboardShaderIndex, checkerboardMaterial);

    Transform playerTransform = Transform();

    for (Sint16 player = 0; player < game_globals::MaximumPlayerCount; player++)
    {
        SpriteMesh* spriteMesh = Game->AddNewSpriteMesh({ 1.0, 1.0 }, { 0.5, 1.0 }, { 0.5f, 0.5f });
        Game->objects.push_back(new Object(player, playerMaterial, playerTransform));

        game_systems::PlayerSystem* playerSystem = Game->AddNewSystem<game_systems::PlayerSystem>();
        playerSystem->objectIndex = player;
        playerSystem->deviceIndex = (Sint16)(player - 1);
    }

    game_systems::TriggerSystem* triggerSystem = Game->AddNewSystem<game_systems::TriggerSystem>();
    triggerSystem->player0ObjectIndex = 0;

    auto cameraSystem = Game->AddNewSystem<game_systems::CameraSystem>();
    cameraSystem->player0ObjectIndex = 0;

    game_globals::LoadWorld("game/testWorld.txt");

    int piecesMeshIndex;
    Game->AddMesh(GenerateEmptyMeshObject(), piecesMeshIndex);
    Game->objects.push_back(new Object(piecesMeshIndex, playerMaterial));

    game_globals::dynamicPiecesMesh = new DynamicMesh(piecesMeshIndex);
    Game->dynamicMeshes.push_back(game_globals::dynamicPiecesMesh);

    int linesMeshIndex;
    Game->AddMesh(GenerateEmptyMeshObject(), linesMeshIndex);
    Game->objects.push_back(new Object(linesMeshIndex, checkerboardMaterial));

    game_globals::dynamicLinesMesh = new DynamicMesh(linesMeshIndex);
    Game->dynamicMeshes.push_back(game_globals::dynamicLinesMesh);

    game_globals::UpdateWorldMeshes();

    Game->AddNewSystem<game_systems::EditorSystem>();
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

    game_globals::dynamicPiecesMesh->vertices.clear();
    game_globals::dynamicPiecesMesh->indices.clear();

    for (int piece = 0; piece < game_globals::world->pieces.size(); piece++)
        AddPieceToMesh(game_globals::world->pieces[piece], game_globals::dynamicPiecesMesh);
}

void game_globals::LoadWorld(std::string FileName)
{
    if (game_globals::world == nullptr)
        game_globals::world = new World();

    game_globals::world->LoadWorld(FileName);
}