#include <vector>
#include <iostream>

#include <gl/glew.h>
#include <glm/gtx/vector_angle.hpp>

#include "../mesh_functions.h"
#include "../resource_functions.h"
#include "../collision_functions.h"

#include "game_globals.h"
#include "player_system.h"

void game::PlayerSystem::Start(std::shared_ptr<esl::Resources> Resources)
{
	// set up camera
	Resources->camera.position = { 0, 0.5f, 10.0f };
	Resources->camera.yaw = 0;
}

void game::PlayerSystem::Update(float DeltaTime, std::unique_ptr<esl::Input>& const Input, std::shared_ptr<esl::Resources> Resources)
{
	float radius = 0.35f;
	float speed = 1.75f;

	glm::vec3 moveInput = glm::vec3(0, 0, 0);

	if (Input->keyboard.isKeyPressed[SDL_SCANCODE_W])
		moveInput.z = -1;
	else if (Input->keyboard.isKeyPressed[SDL_SCANCODE_S])
		moveInput.z = 1;

	if (Input->keyboard.isKeyPressed[SDL_SCANCODE_A])
		moveInput.x = -1;
	else if (Input->keyboard.isKeyPressed[SDL_SCANCODE_D])
		moveInput.x = 1;

	float turnSpeed = 80.0f;
	if (Input->keyboard.isKeyPressed[SDL_SCANCODE_LEFT])
		Resources->camera.yaw += DeltaTime * turnSpeed;
	else if (Input->keyboard.isKeyPressed[SDL_SCANCODE_RIGHT])
		Resources->camera.yaw -= DeltaTime * turnSpeed;

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

	std::vector<esl::SweepHitWithLine> hits;
	if (esl::DoesCircleSweepIntersectWithLines(position, velocity, radius, game_globals::walls, hits))
	{
		position = esl::GetPositionAfterCircleSweep(position, velocity, radius, game_globals::walls, hits[0], 4);
	}
	else
	{
		position += velocity;
	}

	Resources->camera.position.x = position.x;
	Resources->camera.position.z = position.y;
}