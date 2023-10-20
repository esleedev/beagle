#include "cameraSystem.h"
#include "globals.h"
#include <float.h>

void game_systems::CameraSystem::Update(float DeltaTime, Game* Game)
{
    Vector2D minimum = { FLT_MAX, FLT_MAX };
    Vector2D maximum = { FLT_MIN, FLT_MIN };
    for (int player = 0; player < game_globals::MaximumPlayerCount; player++)
    {
        int objectIndex = player + player0ObjectIndex;
        Vector playerPosition = Game->objects[objectIndex]->transform.position;
        if (playerPosition.x < minimum.x)
            minimum.x = playerPosition.x;
        if (playerPosition.y < minimum.y)
            minimum.y = playerPosition.y;
        if (playerPosition.x > maximum.x)
            maximum.x = playerPosition.x;
        if (playerPosition.y > maximum.y)
            maximum.y = playerPosition.y;
    }
    Vector2D maximumToMinimum = maximum - minimum;
    Vector2D center = minimum + maximumToMinimum * 0.5f;

    // update camera
    Game->cameraTransform.position.Set(center);
    Game->cameraTransform.position.z = -10;

    float multiplayerDistance = maximumToMinimum.GetMagnitude();

    if (multiplayerDistance > 2.0f)
        Game->cameraTransform.position.z -= fmin(6.0f, (multiplayerDistance - 2.0f));

    Game->cameraTransform.shouldUpdateMatrix = true;
}
