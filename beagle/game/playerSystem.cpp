#include "playerSystem.h"
#include "globals.h"
#include "../collisionTests.h"
#include <iostream>

void game_systems::PlayerSystem::Update(float DeltaTime, Game* Game)
{
    Vector2D playerPoint = object->transform.position.GetXY();

    // move
    velocity.x *= powf(0.05f, DeltaTime);

    float walkSpeed = 13.5f;
    float moveInputX = 0;
    bool isJumpInputted = false;

    if (deviceIndex < 0)
    {
        if (Game->input->isKeyPressed[SDL_SCANCODE_RIGHT])
            moveInputX = 1;
        else if (Game->input->isKeyPressed[SDL_SCANCODE_LEFT])
            moveInputX = -1;

        isJumpInputted = Game->input->IsKeyJustPressed(SDL_SCANCODE_D);
    }
    else
    {
        moveInputX = Game->input->gamepadInputs[deviceIndex].leftStick.x;
        isJumpInputted = Game->input->IsButtonJustPressed(deviceIndex, 0);
    }

    if (moveInputX > 0.5f)
    {
        velocity.x += DeltaTime * walkSpeed;
        isMirrored = false;
    }
    else if (moveInputX < -0.5f)
    {
        velocity.x -= DeltaTime * walkSpeed;
        isMirrored = true;
    }

    velocity.x = Clamp(velocity.x, -1.5f, 1.5f);

    // fall
    velocity.y -= DeltaTime * 2.9;
    if (velocity.y < -5.7) velocity.y = -5.7;

    playerPoint.x += velocity.x * DeltaTime;
    playerPoint.y += velocity.y * DeltaTime;

    std::vector<Line2D> wallsToCheck;
    bool isOnGround = false;
    for (int line = 0; line < game_globals::world->lines.size(); line++)
    {
        float time;
        Line2D thisLine = game_globals::world->lines[line];

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
        if (time >= 0 && time <= 1)
        {
            bool canLand = false;
            if (velocity.y >= -0.125f)
            {
                canLand = playerPoint.y + 0.0025f <= pointInLine.y && playerPoint.y + 0.25f >= pointInLine.y;
            }
            else
            {
                canLand = playerPoint.y + 0.125f >= pointInLine.y && playerPoint.y - 0.125f <= pointInLine.y;
            }
            if (canLand)
            {
                velocity.y = 0;
                playerPoint.y = pointInLine.y;
                isOnGround = true;
            }
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
                    playerPoint.x -= (playerRectangle.x + playerRectangle.width) - hits.hitPoints[CardinalDirections::Top].x;
                    velocity.x = 0;
                }
                else if (hitPointX > playerRectangle.x)
                {
                    playerPoint.x += hits.hitPoints[CardinalDirections::Top].x - playerRectangle.x;
                    velocity.x = 0;
                }
            }
        }
    }

    if (isOnGround && isJumpInputted)
    {
        // queue jump vel
        velocity.y = 2.18f;
    }

    // update transform
    object->transform.scale.Set(isMirrored ? -1 : 1, 1, 1);
    object->transform.position.Set(playerPoint);
    object->transform.shouldUpdateMatrix = true;

    // check go to triggers
    for (int trigger = 0; trigger < game_globals::world->goToTriggers.size(); trigger++)
    {
        if (IsPointInRectangle(playerPoint, game_globals::world->goToTriggers[trigger].area))
        {
            if (Game->input->IsKeyJustPressed(SDL_SCANCODE_F))
            {
                TriggerEvent triggerEvent;
                triggerEvent.id = game_globals::world->goToTriggers[trigger].id;
                triggerEvent.value = game_globals::world->goToTriggers[trigger].value;
                game_globals::goToEvent = triggerEvent;
            }
        }
    }
}