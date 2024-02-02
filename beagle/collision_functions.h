#pragma once

#include <vector>

#include "shape_types.h"

namespace esl
{
    bool DoesRayIntersectWithSpheres
    (
        glm::vec3 RayOrigin, glm::vec3 RayDirection,
        std::vector<esl::Sphere> Spheres,
        glm::vec3& HitPoint, float& HitDistance, int& HitSphereIndex
    );
}