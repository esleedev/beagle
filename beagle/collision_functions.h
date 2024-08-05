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

	struct SweepHitWithLine
	{
		glm::vec2 newOrigin;
		glm::vec2 hitPoint;
		glm::vec2 hitNormal;
		glm::vec2 lineNormal;
		float hitDistance;
		bool isHitAnEndPoint;
	};

	bool DoesCircleSweepIntersectWithLines
	(
		glm::vec2 Origin, glm::vec2 Velocity, float Radius,
		std::vector<esl::Line> Lines,
		std::vector<esl::SweepHitWithLine>& Hits
	);

	glm::vec2 GetPositionAfterCircleSweep
	(
		glm::vec2 Origin, glm::vec2 Velocity, float Radius,
		std::vector<esl::Line> Lines,
		esl::SweepHitWithLine Hit,
		int IterationCount = 1
	);
}