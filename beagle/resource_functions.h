#pragma once

#include <string>
#include <memory>

#include "resource_types.h"
#include "common_types.h"

namespace esl
{
	// load and add a shader to resources. return a shader id for materials
	short AddShader
	(
		std::shared_ptr<esl::Resources> Resources,
		std::string VertexShaderFilePath, std::string FragmentShaderFilePath
	);

	short AddTexture
	(
		std::shared_ptr<esl::Resources> Resources,
		const char* FilePath
	);

	short AddMaterial
	(
		std::shared_ptr<esl::Resources> Resources,
		short Texture, short Shader
	);

	short AddMesh
	(
		std::shared_ptr<esl::Resources> Resources,
		esl::Mesh Mesh
	);

	std::shared_ptr<esl::Object> AddObject
	(
		std::shared_ptr<esl::Resources> Resources,
		short Mesh,
		short Material,
		esl::Transform Transform
	);

	template <typename SystemType> std::shared_ptr<SystemType> AddNewSystem
	(
		std::shared_ptr<esl::Resources> Resources
	)
	{
		std::shared_ptr<SystemType> system = std::make_shared<SystemType>();
		Resources->systems.push_back(system);
		return system;
	}
}