#include <vector>

#include <gl/glew.h>
#include <glm/gtx/vector_angle.hpp>

#include "../mesh_functions.h"
#include "../resource_functions.h"
#include "../collision_functions.h"
#include "../debug_functions.h"
#include "../shape_types.h"

#include "game_globals.h"
#include "player_system.h"

void game::PlayerSystem::Start(std::shared_ptr<esl::Resources> Resources)
{
	// set up camera
	Resources->camera.position = { 0, 0.5f, 10.0f };
	Resources->camera.yaw = 0;
}

void game::PlayerSystem::Update(float DeltaTime, esl::Input* const Input, std::shared_ptr<esl::Resources> Resources)
{
	float radius = 0.35f;
	float speed = 1.75f;
	float turnSpeed = 80.0f;

	// handle camera turning
	if (Input->keyboard.isKeyPressed[SDL_SCANCODE_LEFT])
		Resources->camera.yaw += DeltaTime * turnSpeed;
	else if (Input->keyboard.isKeyPressed[SDL_SCANCODE_RIGHT])
		Resources->camera.yaw -= DeltaTime * turnSpeed;

	// get movement input
	glm::vec3 moveInput = glm::vec3(0, 0, 0);
	if (Input->keyboard.isKeyPressed[SDL_SCANCODE_W])
		moveInput.z = -1;
	else if (Input->keyboard.isKeyPressed[SDL_SCANCODE_S])
		moveInput.z = 1;
	if (Input->keyboard.isKeyPressed[SDL_SCANCODE_A])
		moveInput.x = -1;
	else if (Input->keyboard.isKeyPressed[SDL_SCANCODE_D])
		moveInput.x = 1;

	// calc movement
	glm::vec2 position = { Resources->camera.position.x, Resources->camera.position.z };
	glm::quat rotationAroundYaw = glm::angleAxis(glm::radians(Resources->camera.yaw), glm::vec3(0, 1, 0));
	if (moveInput.z != 0 || moveInput.x != 0)
	{
		moveInput = glm::normalize(moveInput);
		glm::vec3 moveDirection = rotationAroundYaw * moveInput;
		velocity = glm::normalize(glm::vec2(moveDirection.x, moveDirection.z)) * DeltaTime * speed;
	}
	else
	{
		velocity *= glm::pow(0.004f, DeltaTime);
		if (glm::length2(velocity) < 0.006f * 0.006f)
		{
			velocity = glm::vec2(0);
		}
	}

	// check for collision
	std::vector<esl::SweepHitWithLine> hits;
	if (esl::DoesCircleSweepIntersectWithLines(position, velocity, radius, game_globals::walls, hits))
	{
		position = esl::GetPositionAfterCircleSweep(position, velocity, radius, game_globals::walls, hits[0], 4);
	}
	else
	{
		position += velocity;
	}

	// set camera to our new position. it's in first-person
	Resources->camera.position.x = position.x;
	Resources->camera.position.z = position.y;

	// create ray
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 forward = rotationAroundYaw * glm::vec3(0, 0, -1);
	glm::vec3 right = rotationAroundYaw * glm::vec3(1, 0, 0);
	glm::vec3 rayStart = Resources->camera.position - up * 0.05f + right * 0.15f + forward * 0.125f;
	glm::vec3 rayDirection = forward + up * 0.05f - right * 0.05f;

	// shoot ray against the wall mesh and draw the line
	esl::RayHit hit;
	if (esl::DoesRayIntersectWithMesh(rayStart, rayDirection, Resources->meshes[game_globals::wallsMesh], hit))
	{
		esl_debug::DrawLine(Resources, esl::Line{ rayStart, hit.hitPoint });
	}
}
