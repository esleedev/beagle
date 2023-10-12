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

    Mesh playerMesh = GenerateQuad({ 1.0, 1.0 });
    Game->meshes.push_back(playerMesh);

    GLuint playerTexture = LoadTexture("textures/spriteSheet.png");
    Game->materials.push_back({ playerTexture });

    Transform playerTransform = {};
    playerTransform.position = { 0.0f, 0.0f, 0.0f };
    playerTransform.shouldUpdateMatrix = true;

    SpriteMesh* spriteMesh = new SpriteMesh();
    spriteMesh->sprite.frameUVSize = { 0.5f, 0.5f };
    spriteMesh->vbo = playerMesh.vbo;
    spriteMesh->size = { 1.0, 1.0 };
    spriteMesh->origin = { 0.5, 1.0 };
    spriteMesh->sprite.SetClip({ 0, 1, 1.0f }, true);
    Game->objects.push_back(new Object{ 0, 0, playerTransform, spriteMesh });
    Game->spriteMeshes.push_back(spriteMesh);

    Game->systems.push_back(new game_systems::PlayerSystem());

    DynamicMesh* dynamicMesh;
    int meshIndex;
    Game->PushBackMesh(GenerateEmptyMesh(), meshIndex);
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