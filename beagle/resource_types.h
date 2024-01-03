#pragma once

#include <vector>
#include <memory>
#include <string>

#include "common_types.h"
#include "vertex.h"
#include "transform.h"
#include "camera.h"
#include "input.h"

namespace esl
{
	static const int PositionAttribute = 0;
	static const int UVAttribute = 1;

	static const short TransparentRenderOrder = 1024;

	struct Mesh
	{
		esl::uint vao, vbo, ibo;
		std::vector<esl::Vertex> vertices;
		std::vector<esl::uint> indices;
	};

	struct Shader
	{
		esl::uint program;
		int objectMatrixUniform;
		int projectionMatrixUniform;
		int viewMatrixUniform;
		int textureUniform;
		int colorUniform;
	};

	struct Texture
	{
		esl::uint name;
	};

	class Material
	{
	public:
		short texture;
		short shader;
		short renderOrder;

		Material(short Texture, short Shader, short RenderOrder) : texture(Texture), shader(Shader), renderOrder(RenderOrder) {}
	};

	struct AnimationClip
	{
		short frameStart;
		short frameCount;
		float speed;

		AnimationClip(short FrameStart, short FrameCount, float Speed) : frameStart(FrameStart), frameCount(FrameCount), speed(Speed) {}
	};

	class Sprite
	{
	public:
		float time;
		short mesh;
		glm::vec2 frameUVSize;
		esl::AnimationClip clip;
		esl::AnimationClip queuedClip;
		bool shouldUpdateMesh;

		Sprite(short Mesh, glm::vec2 FrameUVSize, esl::AnimationClip Clip, esl::AnimationClip QueuedClip) : time(0), mesh(Mesh), frameUVSize(FrameUVSize), clip(Clip), queuedClip(QueuedClip), shouldUpdateMesh(true) {}
	};

	enum class TextAlignment : char
	{
		Left = 0,
		Right = 1
	};

	struct Text
	{
		short sharedMesh;
		glm::vec2 position;
		std::string text;
		esl::TextAlignment alignment;
		float letterSpacing;
		float size;
	};

	class Object
	{
	public:
		short mesh;
		std::shared_ptr<esl::Material> material;
		esl::Transform transform;
		glm::vec4 color;
	};

	class Resources;

	class System
	{
	public:
		virtual void Start(std::shared_ptr<esl::Resources> Resources) {};
		virtual void Update
		(
			float DeltaTime,
			std::unique_ptr<esl::Input>& const Input,
			std::shared_ptr<esl::Resources> Resources
		) = 0;
	};

	class Resources
	{
	public:
		esl::Camera camera;
		std::vector<esl::Mesh> meshes;
		std::vector<esl::Shader> shaders;
		std::vector<esl::Texture> textures;
		std::vector<std::shared_ptr<esl::Material>> materials;
		std::vector<std::shared_ptr<esl::Sprite>> sprites;
		std::vector<std::shared_ptr<esl::Text>> texts;
		std::vector<std::shared_ptr<esl::Object>> objects;
		std::vector<std::shared_ptr<esl::System>> systems;
	};
}