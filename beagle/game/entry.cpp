#include "../game.h"
#include "../texture.h"
#include "../shader.h"
#include "../mesh.h"

// define globals
#include "globals.h"
std::vector<Line2D> game_globals::lines;

#include "playerSystem.h"

void OnGameStart(Game* Game)
{
    GLuint shaderProgram = CreateShaderProgram("shaders/vertexShader.txt", "shaders/fragmentShader.txt");
    Shader shader = GetShaderWithLocations(shaderProgram);
    Game->shaders.push_back(shader);

    GLuint playerTexture = LoadTexture("textures/spriteSheet.png");
    Game->materials.push_back({ playerTexture });

    Transform playerTransform = {};
    playerTransform.position = { 0.0f, 0.0f, 0.0f };
    playerTransform.shouldUpdateMatrix = true;

    for (Sint16 player = 0; player < 2; player++)
    {
        SpriteMesh* spriteMesh = Game->AddNewSpriteMesh({ 1.0, 1.0 }, { 0.5, 1.0 }, { 0.5f, 0.5f });
        Game->objects.push_back(new Object{ player, 0, playerTransform, spriteMesh });

        game_systems::PlayerSystem* playerSystem = Game->AddNewSystem<game_systems::PlayerSystem>();
        playerSystem->objectIndex = player;
        playerSystem->deviceIndex = (Sint16)(player - 1);
    }

    DynamicMesh* dynamicMesh;
    int meshIndex;
    Game->AddMesh(GenerateEmptyMesh(), meshIndex);
    Game->objects.push_back(new Object(meshIndex, 0));

    dynamicMesh = new DynamicMesh(&Game->meshes[meshIndex]);
    Game->dynamicMeshes.push_back(dynamicMesh);

    game_globals::lines.push_back(Line2D{ { 2.75f, -1 }, { 2.75f, 2 }, 0.025f });
    game_globals::lines.push_back(Line2D{ { 2, -2 }, { 2.75f, -1 }, 0.025f });
    game_globals::lines.push_back(Line2D{ { -2, -1 }, { 2, -2 }, 0.025f });
    game_globals::lines.push_back(Line2D{ { -4, -1.25f }, { -2, -1 }, 0.025f });

    for (int line = 0; line < game_globals::lines.size(); line++)
        AddLineToMesh(game_globals::lines[line], dynamicMesh);
}

void OnGameEnd(Game* Game)
{

}