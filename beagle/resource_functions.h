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

	// return -1 if texture with file path doesn't exist
	short GetTexture
	(
		std::shared_ptr<esl::Resources> Resources,
		const char* FilePath
	);

	void ReloadTexture
	(
		esl::Texture& Texture
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

	short AddFont
	(
		std::shared_ptr<esl::Resources> Resources,
		std::string FilePath, int Size
	);

	std::shared_ptr<esl::Object> AddTextObject
	(
		std::shared_ptr<esl::Resources> Resources,
		short Shader, short Font,
		std::string Text,
		glm::vec4 DiffuseColor = glm::vec4(1),
		short RenderOrder = -1,
		esl::HorizontalTextAlignment HorizontalTextAlignment = esl::HorizontalTextAlignment::Left,
		esl::VerticalTextAlignment VerticalTextAlignment = esl::VerticalTextAlignment::Bottom
	);

	std::shared_ptr<esl::Object> AddObject
	(
		std::shared_ptr<esl::Resources> Resources,
		short Mesh,
		std::shared_ptr<esl::Material> Material,
		esl::Transform Transform,
		glm::vec4 DiffuseColor = glm::vec4(1)
	);

	template <typename SystemType> std::shared_ptr<SystemType> AddSystem
	(
		std::shared_ptr<esl::Resources> Resources, bool ShouldStartImmediately = true
	)
	{
		std::shared_ptr<SystemType> system = std::make_shared<SystemType>();

		std::shared_ptr<esl::System> castedSystem = std::static_pointer_cast<esl::System>(system);
		if (ShouldStartImmediately)
		{
			castedSystem->Start(Resources);
			Resources->systems.push_back(castedSystem);
		}
		else
		{
			Resources->queuedSystems.push_back(castedSystem);
		}

		return system;
	}

	template <typename EventType> std::shared_ptr<EventType> AddEvent
	(
		std::shared_ptr<esl::Resources> Resources
	)
	{
		std::shared_ptr<EventType> event = std::make_shared<EventType>();

		std::shared_ptr<esl::Event> castedEvent = std::static_pointer_cast<esl::Event>(event);

		Resources->events.push_back(castedEvent);
		return event;
	}

	void CallEvents(std::shared_ptr<esl::Resources> Resources, esl::System* System, esl::ushort Id);
}