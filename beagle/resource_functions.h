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
		short Texture, short Shader, short RenderOrder = 0,
		std::string Name = ""
	);

	std::shared_ptr<esl::Material> GetMaterial(std::shared_ptr<esl::Resources> Resources, std::string Name);

	esl::ushort GetHashFromName(std::string Name);

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
		std::string FilePath, short Shader, int Size
	);

	std::shared_ptr<esl::Text> AddText
	(
		std::shared_ptr<esl::Resources> Resources,
		short Mesh, short Font,
		std::string String,
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
			Resources->queuedSystems.insert(Resources->queuedSystems.begin(), castedSystem);
		}

		return system;
	}

	template <typename EventType> std::shared_ptr<EventType> AddEvent
	(
		std::shared_ptr<esl::Resources> Resources, esl::ushort ID
	)
	{
		std::shared_ptr<EventType> event = std::make_shared<EventType>();

		std::shared_ptr<esl::Event> castedEvent = std::static_pointer_cast<esl::Event>(event);
		castedEvent->resources = Resources;
		castedEvent->id = ID;

		Resources->events.push_back(castedEvent);
		return event;
	}

	// c++11 feature tip: use shared_from_this() if calling from a system if you need the current system as a shared_ptr
	void CallEvents(std::shared_ptr<esl::Resources> Resources, std::shared_ptr<esl::System> System, esl::ushort ID, float WaitTime = 0);
}