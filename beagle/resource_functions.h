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

	std::shared_ptr<esl::Material> AddMaterial
	(
		std::shared_ptr<esl::Resources> Resources,
		short Texture, short Shader, short RenderOrder = 0
	);

	short AddMesh
	(
		std::shared_ptr<esl::Resources> Resources,
		esl::Mesh Mesh
	);

	std::shared_ptr<esl::Sprite> AddSprite
	(
		std::shared_ptr<esl::Resources> Resources,
		short Mesh,
		glm::vec2 FrameUVSize,
		esl::AnimationClip InitialClip = esl::AnimationClip(0, 1, 0)
	);

	std::shared_ptr<esl::Object> AddObject
	(
		std::shared_ptr<esl::Resources> Resources,
		short Mesh,
		std::shared_ptr<esl::Material> Material,
		esl::Transform Transform,
		glm::vec4 Color = glm::vec4(1)
	);

	template <typename SystemType> std::shared_ptr<SystemType> AddSystem
	(
		std::shared_ptr<esl::Resources> Resources
	)
	{
		std::shared_ptr<SystemType> system = std::make_shared<SystemType>();

		std::shared_ptr<esl::System> castedSystem = std::static_pointer_cast<esl::System>(system);
		castedSystem->Start(Resources);

		Resources->systems.push_back(castedSystem);
		return system;
	}
}