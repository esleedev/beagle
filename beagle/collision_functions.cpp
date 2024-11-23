#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>
#include <algorithm>
#include <iostream>

#include "collision_functions.h"

bool esl::DoesRayIntersectWithSpheres
(
    glm::vec3 RayOrigin, glm::vec3 RayDirection,
    std::vector<esl::Sphere> Spheres,
    glm::vec3& HitPoint, float& HitDistance, int& HitSphereIndex
)
{
    HitPoint = RayOrigin;
    HitDistance = std::numeric_limits<float>::max();
    HitSphereIndex = -1;

    for (int sphere = 0; sphere < Spheres.size(); sphere++)
    {
        glm::vec3 toSpherePosition = Spheres[sphere].position - RayOrigin;

        // This dot product lets us know if the sphere is in the direction of the ray
        float dotProduct = glm::dot(toSpherePosition, RayDirection);
        if (dotProduct < 0.0f) continue;

        // We want to get the distance between the ray origin and the outer sphere
        // glm::dot(x, x) is same as squared length
        float squaredDistanceToSphereEdge = glm::dot(toSpherePosition, toSpherePosition) - Spheres[sphere].radius * Spheres[sphere].radius;

        // The discriminant is part of the quadratic equation which is used to find hits
        // If it is zero the ray grazed the edge of the sphere
        // If greater than zero, the ray has entered and exited the sphere
        float discriminant = dotProduct * dotProduct - squaredDistanceToSphereEdge;
        if (discriminant < 0.0f)
            continue;

        float hitDistance = dotProduct - glm::sqrt(discriminant);
        // Negative means ray started from inside the sphere
        if (hitDistance < 0.0f)
            continue;

        if (hitDistance < HitDistance)
        {
            HitPoint = RayOrigin + RayDirection * hitDistance;
            HitDistance = hitDistance;
            HitSphereIndex = sphere;
        }
    }

    return (HitSphereIndex > -1);
}

// Returns true if there are any hits. Hits are sorted based on hit distance, closest first
bool esl::DoesCircleSweepIntersectWithLines
(
    glm::vec2 Origin, glm::vec2 Velocity, float Radius,
    std::vector<esl::Line> Lines,
    std::vector<esl::SweepHitWithLine>& Hits
)
{
    Hits.clear();

    glm::vec2 sweepEndPoint = Origin + Velocity;
    glm::vec2 sweepDirection = glm::normalize(Velocity);
    float sweepDistance = glm::length(Velocity);

    for (int line = 0; line < Lines.size(); line++)
    {
        glm::vec2 linePointA = { Lines[line].pointA.x, Lines[line].pointA.z };
        glm::vec2 linePointB = { Lines[line].pointB.x, Lines[line].pointB.z };
        glm::vec2 aToB = (linePointB - linePointA);
        float lineLength = glm::distance(linePointA, linePointB);

        // find nearest point on the static line to the circle sweep's origin
        float time = (((Origin.x - linePointA.x) * aToB.x) + ((Origin.y - linePointA.y) * aToB.y)) / (lineLength * lineLength);
        glm::vec2 closestToOrigin = linePointA + (time * aToB);

        // test line with sweep
        float lineSweepDeterminant = aToB.y * -Velocity.x - Velocity.y * -aToB.x;
        // lines are parallel, exit early
        if (lineSweepDeterminant == 0.0f)
            continue;

        bool canCollide = false;

        // find point of intersection between static line and sweep line
        glm::vec2 lineSweepHitPoint;
        lineSweepHitPoint.x = (-Velocity.x * (aToB.y * linePointA.x + -aToB.x * linePointA.y) - -aToB.x * (Velocity.y * Origin.x + -Velocity.x * Origin.y)) / lineSweepDeterminant;
        lineSweepHitPoint.y = (aToB.y * (Velocity.y * Origin.x + -Velocity.x * Origin.y) - Velocity.y * (aToB.y * linePointA.x + -aToB.x * linePointA.y)) / lineSweepDeterminant;
        if
        (
            lineSweepHitPoint.x + 0.0001f >= glm::min(linePointA.x, linePointB.x) &&
            lineSweepHitPoint.x - 0.0001f <= glm::max(linePointA.x, linePointB.x) &&
            lineSweepHitPoint.x + 0.0001f >= glm::min(Origin.x, sweepEndPoint.x) &&
            lineSweepHitPoint.x - 0.0001f <= glm::max(Origin.x, sweepEndPoint.x) &&
            lineSweepHitPoint.y + 0.0001f >= glm::min(linePointA.y, linePointB.y) &&
            lineSweepHitPoint.y - 0.0001f <= glm::max(linePointA.y, linePointB.y) &&
            lineSweepHitPoint.y + 0.0001f >= glm::min(Origin.y, sweepEndPoint.y) &&
            lineSweepHitPoint.y - 0.0001f <= glm::max(Origin.y, sweepEndPoint.y)
        )
        {
            canCollide = true;
        }

        // find nearest point on static line to the sweep end point
        time = (((sweepEndPoint.x - linePointA.x) * aToB.x) + ((sweepEndPoint.y - linePointA.y) * aToB.y)) / (lineLength * lineLength);
        glm::vec2 lineSweepEndHitPoint = linePointA + (time * aToB);
        if (glm::distance(lineSweepEndHitPoint, sweepEndPoint) <= Radius + 0.001f && time >= 0.0f - 0.01f && time <= 1.0f + 0.001f)
        {
            canCollide = true;
        }

        // find nearest point on sweep line to static point A
        time = (((linePointA.x - Origin.x) * Velocity.x) + ((linePointA.y - Origin.y) * Velocity.y)) / (sweepDistance * sweepDistance);
        glm::vec2 pointAHitPoint = Origin + (time * Velocity);
        if (time >= -0.0001f && time <= 1.0f + Radius / sweepDistance + 0.005f && glm::distance(pointAHitPoint, linePointA) <= Radius + 0.0025f)
        {
            canCollide = true;
        }

        // find nearest point on sweep line to static point B
        time = (((linePointB.x - Origin.x) * Velocity.x) + ((linePointB.y - Origin.y) * Velocity.y)) / (sweepDistance * sweepDistance);
        glm::vec2 pointBHitPoint = Origin + (time * Velocity);
        if (time >= -0.0001f && time <= 1.0f + Radius / sweepDistance + 0.005f && glm::distance(pointBHitPoint, linePointB) <= Radius + 0.0025f)
        {
            canCollide = true;
        }

        if (!canCollide) continue;

        // new origin after sweep
        float distanceToClosestToOrigin = glm::distance(Origin, closestToOrigin);
        if (distanceToClosestToOrigin == 0.0f)
            continue;
        glm::vec2 newOrigin = lineSweepHitPoint - sweepDirection * (Radius * (glm::distance(Origin, lineSweepHitPoint) / distanceToClosestToOrigin));

        // find where the nearest point is on the static line to the new origin
        time = (((newOrigin.x - linePointA.x) * aToB.x) + ((newOrigin.y - linePointA.y) * aToB.y)) / (lineLength * lineLength);

        if (time >= 0.0f && time <= 1.0f)
        {
            // nearest point to new origin is on the static line
            esl::SweepHitWithLine hit;
            hit.hitPoint = linePointA + (time * aToB);
            hit.hitDistance = glm::distance(hit.hitPoint, Origin);
            hit.newOrigin = newOrigin;
            hit.lineNormal = hit.hitNormal = glm::normalize(glm::vec2(-aToB.y, aToB.x));
            hit.isHitAnEndPoint = false;
            Hits.push_back(hit);
        }
        else
        {
            // nearest point to new origin is on an end point
            glm::vec2 endPointHitPoint = (time <= 0.0f) ? pointAHitPoint : pointBHitPoint;
            glm::vec2 endPoint = (time <= 0.0f) ? linePointA : linePointB;
            float distanceToEndPoint = glm::distance(endPointHitPoint, endPoint);
            float distanceToMoveBackFromHitPoint = glm::sqrt(Radius * Radius - distanceToEndPoint * distanceToEndPoint);
            if (std::isnan(distanceToMoveBackFromHitPoint))
                distanceToMoveBackFromHitPoint = 0.0f;
            newOrigin = endPointHitPoint - sweepDirection * distanceToMoveBackFromHitPoint;

            esl::SweepHitWithLine hit;
            hit.hitPoint = endPoint;
            hit.hitDistance = glm::distance(hit.hitPoint, Origin);
            hit.newOrigin = newOrigin;
            hit.lineNormal = glm::normalize(glm::vec2(-aToB.y, aToB.x));
            hit.hitNormal = glm::normalize(hit.newOrigin - hit.hitPoint);
            hit.isHitAnEndPoint = true;
            Hits.push_back(hit);
        }
    }

    // sort hits based on hit distance
    std::sort
    (
        Hits.begin(), Hits.end(),
        [](esl::SweepHitWithLine const& A, esl::SweepHitWithLine const& B) { return A.hitDistance < B.hitDistance; }
    );

    return Hits.size() > 0;
}

glm::vec2 esl::GetPositionAfterCircleSweep
(
    glm::vec2 Origin, glm::vec2 Velocity, float Radius,
    std::vector<esl::Line> Lines,
    esl::SweepHitWithLine Hit,
    int IterationCount
)
{
    const float Epsilon = 0.000003f;

    if (Hit.hitDistance >= Radius + Epsilon + Epsilon * 0.2f)
        return Origin + Velocity;

    glm::vec2 direction = glm::normalize(Velocity);

    glm::vec2 toNewOrigin = Hit.newOrigin - Origin;
    float projectedDistanceAlongDirection = glm::dot(toNewOrigin, Velocity) - Epsilon;
    glm::vec2 position = Origin + direction * projectedDistanceAlongDirection;

    if (IterationCount <= 1)
        return position;

    float distanceRemaining = glm::max(0.0f, glm::length(Velocity) - projectedDistanceAlongDirection);

    glm::vec2 toTarget = Origin + Velocity - position;
    float distanceToTarget = glm::length(toTarget);
    float projectedDistanceAlongNormal = glm::dot(toTarget, -Hit.hitNormal);
    distanceToTarget -= projectedDistanceAlongNormal;
    if (distanceToTarget <= Epsilon || distanceRemaining <= Epsilon)
        return position;

    distanceRemaining = glm::min(distanceRemaining, distanceToTarget);
    toTarget -= projectedDistanceAlongNormal * Hit.hitNormal;
    direction = glm::normalize(toTarget);

    Velocity = direction * distanceRemaining;
    Velocity -= Hit.hitNormal * glm::dot(Velocity, Hit.hitNormal);

    std::vector<esl::SweepHitWithLine> hits;
    if (esl::DoesCircleSweepIntersectWithLines(position, Velocity, Radius, Lines, hits))
    {
        if (glm::distance2(Hit.lineNormal, hits[0].lineNormal) > Epsilon && hits[0].isHitAnEndPoint == false)
            return esl::GetPositionAfterCircleSweep(position, Velocity, Radius, Lines, hits[0], IterationCount - 1);
        else
            return position + Velocity;
    }
    else
    {
        return position + Velocity;
    }
}
