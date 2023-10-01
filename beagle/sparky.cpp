#include "game.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"

class TestSystem : public System
{
public:
    void Update(float DeltaTime, Game* Game)
    {
        if (Game->input->isKeyPressed[SDL_SCANCODE_D])
        {
            Game->cameraTransform.position.x += 0.5 * DeltaTime;
        }
        else if (Game->input->isKeyPressed[SDL_SCANCODE_A])
        {
            Game->cameraTransform.position.x -= 0.5 * DeltaTime;
        }

        if (Game->input->isKeyPressed[SDL_SCANCODE_LEFT])
        {
            Game->cameraTransform.yaw -= 30.0f * DeltaTime;
        }
        else if (Game->input->isKeyPressed[SDL_SCANCODE_RIGHT])
        {
            Game->cameraTransform.yaw += 30.0f * DeltaTime;
        }

        if (Game->input->IsKeyJustPressed(SDL_SCANCODE_R))
        {
            Game->spriteMeshes[0]->sprite.QueueClip({ 0, 16, 8.0f });
        }
    }
};

void OnGameStart(Game* Game)
{
    GLuint shaderProgram = CreateShaderProgram("shaders/vertexShader.txt", "shaders/fragmentShader.txt");
    Shader shader = GetShaderWithLocations(shaderProgram);
    Game->shaders.push_back(shader);

    Mesh backgroundQuad, animatedQuad;
    backgroundQuad = GenerateQuad({ 1.0, 1.0 }, { 0.5, 0.5 });
    Game->meshes.push_back(backgroundQuad);
    animatedQuad = GenerateQuad({ 1.0, 1.0 }, { 0.5, 0.5 });
    Game->meshes.push_back(animatedQuad);

    GLuint backgroundTexture = LoadTexture("textures/spriteSheet.png");
    Game->materials.push_back({ backgroundTexture });
    GLuint animatedTexture = LoadTexture("textures/spritesheet2.png");
    Game->materials.push_back({ animatedTexture });

    Transform cubeTransform = {};
    for (int x = 0; x < 8; x++)
    {
        cubeTransform.position = { x * 1.25f, (float)cos(x * 1.9f) * 0.4f, 5.0f + (float)sin(x * 0.5f) + x % 2 };
        cubeTransform.shouldUpdateMatrix = true;
        Game->objects.push_back(new Object{ 0, 0, cubeTransform, nullptr });
    }

    cubeTransform.position = { 2.0f, 0.0f, 5.0f };
    cubeTransform.shouldUpdateMatrix = true;
    SpriteMesh* spriteMesh = new SpriteMesh();
    spriteMesh->sprite.frameUVSize = { 0.125f, 0.125f };
    spriteMesh->vbo = animatedQuad.vbo;
    spriteMesh->sprite.SetClip({ 40, 4, 12.0f }, true);
    Game->objects.push_back(new Object{ 1, 1, cubeTransform, spriteMesh });
    Game->spriteMeshes.push_back(spriteMesh);

    Game->systems.push_back(new TestSystem());
}

void OnGameEnd(Game* Game)
{

}