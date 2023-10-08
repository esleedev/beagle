#include "game.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"
#include "pointTests.h"

class PlayerSystem : public System
{
public:
    Line2D line;
    PlayerSystem(Game* Game)
    {
        line.pointA = { -2, 0 };
        line.pointB = { 2, -2 };
        line.width = 0.25;
        Game->meshes.push_back(GenerateLine(line));
        Transform noTransform = {};
        noTransform.position = { 0, 0, 0 };
        noTransform.shouldUpdateMatrix = true;
        Game->objects.push_back(new Object{ 1, 0, noTransform, nullptr });
    }

    void Update(float DeltaTime, Game* Game)
    {
        Game->cameraTransform.position.z = -10;

        Vector2D playerPoint = Game->objects[0]->transform.position.GetXY();

        // move
        if (Game->input->isKeyPressed[SDL_SCANCODE_RIGHT])
        {
            playerPoint.x += DeltaTime;
        }
        else if (Game->input->isKeyPressed[SDL_SCANCODE_LEFT])
        {
            playerPoint.x -= DeltaTime;
        }

        playerPoint.y -= DeltaTime; // fall

        Vector2D pointInLine = GetPointClosestToPointInLine(playerPoint, {-2, 0}, {2, -2});
        if (playerPoint.y <= pointInLine.y)
        {
            playerPoint.y = pointInLine.y;
        }

        Game->objects[0]->transform.position.Set(playerPoint);
        Game->objects[0]->transform.shouldUpdateMatrix = true;
    }
};

void OnGameStart(Game* Game)
{
    GLuint shaderProgram = CreateShaderProgram("shaders/vertexShader.txt", "shaders/fragmentShader.txt");
    Shader shader = GetShaderWithLocations(shaderProgram);
    Game->shaders.push_back(shader);

    Mesh playerMesh = GenerateQuad({ 1.0, 1.0 }, { 0.5, 1.0 });
    Game->meshes.push_back(playerMesh);

    GLuint playerTexture = LoadTexture("textures/spriteSheet.png");
    Game->materials.push_back({ playerTexture });

    Transform playerTransform = {};
    playerTransform.position = { 0.0f, 0.0f, 0.0f };
    playerTransform.shouldUpdateMatrix = true;

    SpriteMesh* spriteMesh = new SpriteMesh();
    spriteMesh->sprite.frameUVSize = { 0.5f, 0.5f };
    spriteMesh->vbo = playerMesh.vbo;
    spriteMesh->sprite.SetClip({ 0, 1, 1.0f }, true);
    Game->objects.push_back(new Object{ 0, 0, playerTransform, spriteMesh });
    Game->spriteMeshes.push_back(spriteMesh);

    Game->systems.push_back(new PlayerSystem(Game));
}

void OnGameEnd(Game* Game)
{

}