#include <glm/geometric.hpp>
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
