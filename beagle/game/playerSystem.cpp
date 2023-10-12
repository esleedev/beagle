#include "playerSystem.h"
#include "globals.h"
#include "../collisionTests.h"
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

    // fall
    velocity.y -= DeltaTime * 2.9;
    if (velocity.y < -5.7) velocity.y = -5.7;

    playerPoint.x += velocity.x * DeltaTime;
    playerPoint.y += velocity.y * DeltaTime;

    std::vector<Line2D> wallsToCheck;
    bool isOnGround = false;
    for (int line = 0; line < game_globals::lines.size(); line++)
    {
        float time;
        Line2D thisLine = game_globals::lines[line];

        // check if the line is ground
        Vector2D bToA = thisLine.pointA - thisLine.pointB;
        float angle = std::atan2f(bToA.y, -bToA.x);  // rotate bToA clockwise to get normal, then get the angle
        angle *= 180.0f / 3.141592653f;
        if (abs(angle) > 60.0f) // line is too sloped to stand on :(
        {
            // might be a good wall candidate though!
            wallsToCheck.push_back(thisLine);
            continue;
        }

        Vector2D pointInLine = GetPointClosestToPointInLine(playerPoint, thisLine.pointA, thisLine.pointB, time);
        if (time >= 0 && time <= 1 && playerPoint.y - 0.008f + velocity.y * DeltaTime * 2.0f <= pointInLine.y)
        {
            velocity.y = 0;
            playerPoint.y = pointInLine.y;
            isOnGround = true;
        }
    }

    // wall collision
    Rectangle playerRectangle = { playerPoint.x - 0.375f, playerPoint.y, 0.75f, 1.0f };
    for (int wall = 0; wall < wallsToCheck.size(); wall++)
    {
        Line2D thisWall = wallsToCheck[wall];
        LineHitRectangle hits;
        if (IsLineIntersectingRectangle(thisWall.pointA, thisWall.pointB, playerRectangle, hits))
        {
            if (hits.hasHitSide[CardinalDirections::Top])
            {
                float hitPointX = hits.hitPoints[CardinalDirections::Top].x;
                if (hitPointX < playerRectangle.x + playerRectangle.width)
                {
                    std::cout << playerRectangle.x + playerRectangle.width << " " << hits.hitPoints[CardinalDirections::Top].x << std::endl;
                    playerPoint.x -= (playerRectangle.x + playerRectangle.width) - hits.hitPoints[CardinalDirections::Top].x;
                    velocity.x = 0;
                }
                else if (hitPointX > playerRectangle.x)
                {
                    std::cout << playerRectangle.x << " " << hits.hitPoints[CardinalDirections::Top].x << std::endl;
                    playerPoint.x += hits.hitPoints[CardinalDirections::Top].x - playerRectangle.x;
                    velocity.x = 0;
                }
            }
        }
    }

    if (isOnGround && Game->input->IsKeyJustPressed(SDL_SCANCODE_D))
    {
        // queue jump vel
        velocity.y = 2.18f;
    }

    // update position
    Game->objects[0]->transform.position.Set(playerPoint);
    Game->objects[0]->transform.shouldUpdateMatrix = true;
}