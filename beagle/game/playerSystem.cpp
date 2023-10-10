#include "playerSystem.h"
#include "globals.h"
#include "../pointTests.h"
#include <iostream>

void game_systems::PlayerSystem::Update(float DeltaTime, Game* Game)
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

    for (int line = 0; line < game_globals::lines.size(); line++)
    {
        float time;
        Line2D thisLine = game_globals::lines[line];
        Vector2D pointInLine = GetPointClosestToPointInLine(playerPoint, thisLine.pointA, thisLine.pointB, time);
        if (time >= 0 && time <= 1 && playerPoint.y <= pointInLine.y)
        {
            playerPoint.y = pointInLine.y;
        }
    }

    Game->objects[0]->transform.position.Set(playerPoint);
    Game->objects[0]->transform.shouldUpdateMatrix = true;
}