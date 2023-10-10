#include "playerSystem.h"
#include "globals.h"
#include "../pointTests.h"
#include <iostream>

void game_systems::PlayerSystem::Update(float DeltaTime, Game* Game)
{
    Game->cameraTransform.position.z = -10;

    Vector2D playerPoint = Game->objects[0]->transform.position.GetXY();

    // move
    velocity.x *= powf(0.05f, DeltaTime);

    float walkSpeed = 13.5f;
    if (Game->input->isKeyPressed[SDL_SCANCODE_RIGHT])
    {
        velocity.x += DeltaTime * walkSpeed;
    }
    else if (Game->input->isKeyPressed[SDL_SCANCODE_LEFT])
    {
        velocity.x -= DeltaTime * walkSpeed;
    }
    velocity.x = Clamp(velocity.x, -1.5f, 1.5f);

    playerPoint.x += velocity.x * DeltaTime;

    // fall
    velocity.y -= DeltaTime * 2.9;
    if (velocity.y < -5.7) velocity.y = -5.7;

    bool isOnGround = false;
    for (int line = 0; line < game_globals::lines.size(); line++)
    {
        float time;
        Line2D thisLine = game_globals::lines[line];
        Vector2D pointInLine = GetPointClosestToPointInLine(playerPoint, thisLine.pointA, thisLine.pointB, time);
        if (time >= 0 && time <= 1 && playerPoint.y + velocity.y * DeltaTime - 0.025f <= pointInLine.y)
        {
            velocity.y = 0;
            playerPoint.y = pointInLine.y;
            isOnGround = true;
        }
    }

    if (isOnGround && Game->input->IsKeyJustPressed(SDL_SCANCODE_D))
    {
        velocity.y = 2.18f;
    }

    playerPoint.y += velocity.y * DeltaTime;

    Game->objects[0]->transform.position.Set(playerPoint);
    Game->objects[0]->transform.shouldUpdateMatrix = true;
}