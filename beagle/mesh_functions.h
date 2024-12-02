#pragma once

#include "resource_types.h"
#include "shape_types.h"

namespace esl
{
	esl::Mesh GenerateQuadMesh(glm::vec2 Size, glm::vec2 Origin);
	esl::LineMesh GenerateLineMesh(esl::Line Line);
	void DeleteMeshes(std::vector<esl::Mesh> Meshes);
	void LoadOBJ(std::string FilePath, std::vector<glm::vec3>& Vertices, std::vector<esl::uint>& Indices);
	void LoadOBJWithPositionDataOnly(std::string FilePath, std::vector<esl::Vertex>& Vertices, std::vector<esl::uint>& Indices);
	void LoadOBJ(std::string FilePath, std::vector<esl::Vertex>& Vertices, std::vector<esl::uint>& Indices);
	esl::Mesh LoadOBJAsMesh(std::string FilePath, esl::VertexDescription VertexDescription = esl::VertexDescription::HasAll);
	void ReloadMeshIfItHasFile(esl::Mesh& Mesh);
}