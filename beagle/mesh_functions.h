#pragma once

#include "resource_types.h"

namespace esl
{
	esl::Mesh GenerateQuadMesh(glm::vec2 Size, glm::vec2 Origin);
	void DeleteMeshes(std::vector<esl::Mesh> Meshes);
	void LoadOBJ(std::string FilePath, std::vector<esl::Vertex>& Vertices, std::vector<esl::uint>& Indices);
	esl::Mesh LoadOBJAsMesh(std::string FilePath);
}