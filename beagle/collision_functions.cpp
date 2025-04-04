#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <algorithm>

#include "collision_functions.h"
#include "resource_types.h"
#include "shape_types.h"

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

bool esl::DoesRayIntersectWithMesh(glm::vec3 RayOrigin, glm::vec3 RayDirection, const esl::Mesh& Mesh, esl::RayHit& Hit)
{
    const float Epsilon = 0.000001f;

    Hit.hitPoint = Hit.hitNormal = glm::vec3(0, 0, 0);
    Hit.hitDistance = std::numeric_limits<float>::max();

    int triangleCount = Mesh.indices.size() / 3;
    for (int triangle = 0; triangle < triangleCount; triangle++)
    {
        glm::vec3 vertex0 = Mesh.vertices[Mesh.indices[triangle * 3]].position;
        glm::vec3 vertex1 = Mesh.vertices[Mesh.indices[triangle * 3 + 1]].position;
        glm::vec3 vertex2 = Mesh.vertices[Mesh.indices[triangle * 3 + 2]].position;

        glm::vec3 edge0To1 = vertex1 - vertex0;
        glm::vec3 edge0To2 = vertex2 - vertex0;

        glm::vec3 rayCrossEdge0To2 = glm::cross(RayDirection, edge0To2);
        float determinant = glm::dot(edge0To1, rayCrossEdge0To2);

        if (determinant > -Epsilon && determinant < Epsilon)
            continue;

        glm::vec3 toOrigin = RayOrigin - vertex0;
        float u = (1.0f / determinant) * glm::dot(toOrigin, rayCrossEdge0To2);

        if (u >= 0.0f && u <= 1.0f)
        {
            glm::vec3 toOriginCrossEdge0To1 = glm::cross(toOrigin, edge0To1);
            float v = (1.0f / determinant) * glm::dot(RayDirection, toOriginCrossEdge0To1);

            if (v >= 0.0f && u + v <= 1.0f)
            {
                float time = (1.0f / determinant) * glm::dot(edge0To2, toOriginCrossEdge0To1);
                if (time > Epsilon && time < Hit.hitDistance)
                {
                    Hit.hitPoint = RayOrigin + RayDirection * time;
                    Hit.hitNormal = glm::normalize(glm::cross(glm::normalize(edge0To2), glm::normalize(edge0To1)));
                    Hit.hitDistance = time;
                }
            }
        }
    }

    return Hit.hitDistance < std::numeric_limits<float>::max();
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

    const float Epsilon = 0.000008f;
    const float SmallerEpsilon = 0.000002f;

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

        // distance from line to origin
        float distanceToClosestToOrigin = glm::distance(Origin, closestToOrigin);
        if (distanceToClosestToOrigin == 0.0f)
            continue;

        // test line with sweep
        float lineSweepDeterminant = aToB.y * -Velocity.x - Velocity.y * -aToB.x;
        // lines are parallel, or sweep is happening from the other side. exit early
        if (lineSweepDeterminant >= 0.0f)
            continue;

        bool canCollide = false;
        bool canCollideWithEndPoint = false;

        // find point of intersection between static line and sweep line
        glm::vec2 lineSweepHitPoint;
        lineSweepHitPoint.x = (-Velocity.x * (aToB.y * linePointA.x + -aToB.x * linePointA.y) - -aToB.x * (Velocity.y * Origin.x + -Velocity.x * Origin.y)) / lineSweepDeterminant;
        lineSweepHitPoint.y = (aToB.y * (Velocity.y * Origin.x + -Velocity.x * Origin.y) - Velocity.y * (aToB.y * linePointA.x + -aToB.x * linePointA.y)) / lineSweepDeterminant;
        if
        (
            lineSweepHitPoint.x + SmallerEpsilon >= glm::min(linePointA.x, linePointB.x) &&
            lineSweepHitPoint.x - SmallerEpsilon <= glm::max(linePointA.x, linePointB.x) &&
            lineSweepHitPoint.x + SmallerEpsilon >= glm::min(Origin.x, sweepEndPoint.x) &&
            lineSweepHitPoint.x - SmallerEpsilon <= glm::max(Origin.x, sweepEndPoint.x) &&
            lineSweepHitPoint.y + SmallerEpsilon >= glm::min(linePointA.y, linePointB.y) &&
            lineSweepHitPoint.y - SmallerEpsilon <= glm::max(linePointA.y, linePointB.y) &&
            lineSweepHitPoint.y + SmallerEpsilon >= glm::min(Origin.y, sweepEndPoint.y) &&
            lineSweepHitPoint.y - SmallerEpsilon <= glm::max(Origin.y, sweepEndPoint.y)
        )
        {
            canCollide = true;
        }

        // find nearest point on static line to the sweep end point
        time = (((sweepEndPoint.x - linePointA.x) * aToB.x) + ((sweepEndPoint.y - linePointA.y) * aToB.y)) / (lineLength * lineLength);
        glm::vec2 lineSweepEndHitPoint = linePointA + (time * aToB);
        if (glm::distance(lineSweepEndHitPoint, sweepEndPoint) <= Radius + SmallerEpsilon && time >= 0.0f - Epsilon / lineLength && time <= 1.0f + Epsilon / lineLength)
        {
            canCollide = true;
        }

        // find nearest point on sweep line to static point A
        time = (((linePointA.x - Origin.x) * Velocity.x) + ((linePointA.y - Origin.y) * Velocity.y)) / (sweepDistance * sweepDistance);
        glm::vec2 pointAHitPoint = Origin + (time * Velocity);
        if (time >= (-Radius - Epsilon) / sweepDistance && time <= 1.0f + (Radius + Epsilon) / sweepDistance && glm::distance(pointAHitPoint, linePointA) <= (Radius + Epsilon))
        {
            canCollideWithEndPoint = true;
        }

        // find nearest point on sweep line to static point B
        time = (((linePointB.x - Origin.x) * Velocity.x) + ((linePointB.y - Origin.y) * Velocity.y)) / (sweepDistance * sweepDistance);
        glm::vec2 pointBHitPoint = Origin + (time * Velocity);
        if (time >= (-Radius - Epsilon) / sweepDistance && time <= 1.0f + (Radius + Epsilon) / sweepDistance && glm::distance(pointBHitPoint, linePointB) <= (Radius + Epsilon))
        {
            canCollideWithEndPoint = true;
        }

        if (!canCollide && !canCollideWithEndPoint) continue;

        // new origin after sweep
        glm::vec2 newOrigin = lineSweepHitPoint - sweepDirection * (Radius * (glm::distance(Origin, lineSweepHitPoint) / distanceToClosestToOrigin));

        // find where the nearest point is on the static line to the new origin
        time = (((newOrigin.x - linePointA.x) * aToB.x) + ((newOrigin.y - linePointA.y) * aToB.y)) / (lineLength * lineLength);

        if (time > 0.0f && time < 1.0f)
        {
            // nearest point to new origin is on the static line
            esl::SweepHitWithLine hit;
            hit.lineIndex = line;
            hit.hitPoint = linePointA + (time * aToB);
            hit.hitDistance = glm::distance(hit.hitPoint, Origin);
            hit.lineNormal = hit.hitNormal = glm::normalize(glm::vec2(-aToB.y, aToB.x));
            hit.newOrigin = newOrigin + hit.hitNormal * Epsilon;
            hit.isHitAnEndPoint = false;
            Hits.push_back(hit);
        }
        else if (canCollideWithEndPoint)
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
            hit.lineIndex = line;
            hit.hitPoint = endPoint;
            hit.hitDistance = glm::distance(hit.hitPoint, Origin);
            hit.lineNormal = glm::normalize(glm::vec2(-aToB.y, aToB.x));
            hit.hitNormal = glm::normalize(newOrigin - endPoint);
            hit.newOrigin = newOrigin + hit.hitNormal * Epsilon;
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
    const float Epsilon = 0.000012f;
    const float SmallerEpsilon = 0.000001f;

    float distance = glm::length(Velocity);

    if (distance == 0.0f)
    {
        return Origin;
    }

    glm::vec2 direction = Velocity / distance;

    glm::vec2 toNewOrigin = Hit.newOrigin - Origin;
    float projectedDistanceAlongDirection = glm::dot(toNewOrigin, Velocity) - SmallerEpsilon;
    glm::vec2 position = Origin + direction * projectedDistanceAlongDirection;
    if (IterationCount <= 0)
        return position;

    float distanceRemaining = glm::max(0.0f, glm::length(Velocity) - projectedDistanceAlongDirection);
    if (distanceRemaining <= 0.0f)
        return position;

    glm::vec2 toTarget = Origin + Velocity - position;
    float distanceToTarget = glm::length(toTarget);
    float projectedDistanceAlongNormal = glm::dot(toTarget, -Hit.hitNormal);
    distanceToTarget -= projectedDistanceAlongNormal;
    distanceRemaining = glm::min(distanceRemaining, distanceToTarget);

    toTarget -= projectedDistanceAlongNormal * Hit.hitNormal;
    direction = glm::normalize(toTarget);

    Velocity = direction * distanceRemaining;
    Velocity -= Hit.hitNormal * glm::dot(Velocity, Hit.hitNormal);

    distance = glm::length(Velocity);
    if (distance == 0.0f) return position;
    direction = Velocity / distance;

    std::vector<esl::SweepHitWithLine> hits;
    if (esl::DoesCircleSweepIntersectWithLines(position, Velocity + direction * Epsilon, Radius - SmallerEpsilon, Lines, hits))
    {
        esl::SweepHitWithLine hit = hits[0];
        if (Hit.lineIndex != hit.lineIndex || (hit.isHitAnEndPoint != Hit.isHitAnEndPoint))
            return esl::GetPositionAfterCircleSweep(position, Velocity, Radius, Lines, hit, IterationCount - 1);
        else
            return position + Velocity;
    }
    else
    {
        return position + Velocity;
    }
}
