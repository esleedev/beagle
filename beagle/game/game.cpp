#include "../game.h"
#include "../resource_functions.h"
#include "../mesh_functions.h"

void esl_main::OnGameStart(std::shared_ptr<esl::Resources> Resources)
{
	short mesh = esl::AddMesh(Resources, esl::GenerateQuadMesh(glm::vec2(1.0f, 1.0f), glm::vec2(0.5f, 0.5f)));
	short shader = esl::AddShader(Resources, "shaders/vertexShader.txt", "shaders/fragmentShader.txt");
	short texture = esl::AddTexture(Resources, "textures/checkerboard.png");
	std::shared_ptr<esl::Material> material = esl::AddMaterial(Resources, texture, shader);
	std::shared_ptr<esl::Object> object = esl::AddObject(Resources, mesh, material, esl::Transform());
	object->transform.matrix = glm::mat4x4(1);
}