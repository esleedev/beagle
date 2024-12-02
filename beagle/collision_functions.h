#pragma once

#include <vector>

namespace esl
{
	struct Mesh;
	struct Sphere;
	struct Line;

    bool DoesRayIntersectWithSpheres
    (
        glm::vec3 RayOrigin, glm::vec3 RayDirection,
        std::vector<esl::Sphere> Spheres,
        glm::vec3& HitPoint, float& HitDistance, int& HitSphereIndex
    );

	struct RayHit
	{
		glm::vec3 hitPoint;
		glm::vec3 hitNormal;
		float hitDistance;
	};

	bool DoesRayIntersectWithMesh
	(
		glm::vec3 RayOrigin, glm::vec3 RayDirection,
		const esl::Mesh& Mesh, esl::RayHit& Hit
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