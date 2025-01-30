#include <gl/glew.h>

#include "../game.h"
#include "../resource_functions.h"
#include "../mesh_functions.h"

#include "game_globals.h"
#include "player_system.h"

namespace game_globals
{
	std::vector<esl::Line> walls;
	short wallsMesh;
}

void esl_main::OnGameStart(std::shared_ptr<esl::Resources> Resources)
{
	// add player system :)
	esl::AddSystem<game::PlayerSystem>(Resources);

	// load font
	short textShader = esl::AddShader(Resources, "shaders/vertexShader.txt", "shaders/textFragmentShader.txt");
	short font = esl::AddFont(Resources, "fonts/jost-semibold.ttf", textShader, 32);
	// add text
	short textMesh = esl::AddEmptyMesh(Resources);
	glm::vec4 textColor = glm::vec4(1);
	std::shared_ptr<esl::Object> textObject = esl::AddObject(Resources, textMesh, Resources->fonts[font]->material, esl::Transform(), textColor);
	textObject->transform.position = { 0.0f, 0.25f, 6.0f };
	textObject->transform.scale = { 0.75f, 0.75f, 1 };
	std::shared_ptr<esl::Text> text = esl::AddText
	(
		Resources,
		textMesh, font,
		"It was a dark and stormy night",
		esl::HorizontalTextAlignment::Left, esl::VerticalTextAlignment::Top
	);

	// add walls
	esl::Line line;
	line = { glm::vec3(-3.75f, 0, 8), glm::vec3(-3.5f, 0, 6) };
	game_globals::walls.push_back(line);
	line = { glm::vec3(-3.5f, 0, 6), glm::vec3(-1, 0, 3) };
	game_globals::walls.push_back(line);
	line = { glm::vec3(-1, 0, 3), glm::vec3(1, 0, 3) };
	game_globals::walls.push_back(line);
	line = { glm::vec3(1, 0, 3), glm::vec3(3, 0, 2) };
	game_globals::walls.push_back(line);
	line = { glm::vec3(3, 0, 2), glm::vec3(3, 0, 6) };
	game_globals::walls.push_back(line);
	line = { glm::vec3(3, 0, 6), glm::vec3(5, 0, 6.5f) };
	game_globals::walls.push_back(line);
	line = { glm::vec3(5, 0, 6.5f), glm::vec3(4, 0, 10.5f) };
	game_globals::walls.push_back(line);
	line = { glm::vec3(4, 0, 10.5f), glm::vec3(-3, 0, 10.5f) };
	game_globals::walls.push_back(line);
	line = { glm::vec3(-3, 0, 10.5f), glm::vec3(-3.5f, 0, 6) };
	game_globals::walls.push_back(line);

	// wall mesh
	game_globals::wallsMesh = esl::AddMesh(Resources, esl::GenerateEmptyMesh());

	// build mesh out of the lines
	for (int wall = 0; wall < game_globals::walls.size(); wall++)
	{
		esl::Vertex vertices[] =
		{
			esl::Vertex{ game_globals::walls[wall].pointA, glm::vec2{ 0, 0 } },
			esl::Vertex{ game_globals::walls[wall].pointA + glm::vec3{ 0, 1, 0 }, glm::vec2{ 0, 1 } },
			esl::Vertex{ game_globals::walls[wall].pointB + glm::vec3{ 0, 1, 0 }, glm::vec2{ 1, 1 } },
			esl::Vertex{ game_globals::walls[wall].pointB, glm::vec2{ 1, 0 } }
		};
		esl::uint indices[] =
		{
			wall * 4, wall * 4 + 1, wall * 4 + 2,
			wall * 4, wall * 4 + 2, wall * 4 + 3
		};
		Resources->meshes[game_globals::wallsMesh].indices.insert(Resources->meshes[game_globals::wallsMesh].indices.end(), indices, indices + 6);
		Resources->meshes[game_globals::wallsMesh].vertices.insert(Resources->meshes[game_globals::wallsMesh].vertices.end(), vertices, vertices + 4);
	}

	// update vao
	esl::Mesh mesh = Resources->meshes[game_globals::wallsMesh];
	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(esl::Vertex), &mesh.vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(esl::uint), &mesh.indices[0], GL_DYNAMIC_DRAW);

	// place the walls in the scene :)
	short shaderIndex = esl::AddShader(Resources, "shaders/vertexShader.txt", "shaders/fragmentShader.txt");
	short textureIndex = esl::AddTexture(Resources, "textures/checkerboard.png");
	std::shared_ptr<esl::Material> material = esl::AddMaterial(Resources, textureIndex, shaderIndex);
	std::shared_ptr<esl::Object> object = esl::AddObject(Resources, game_globals::wallsMesh, material, esl::Transform());
	object->transform.position = glm::vec3(0, 0, 0);
}