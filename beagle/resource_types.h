#pragma once

#include <vector>
#include <memory>

#include "common_types.h"
#include "vertex.h"
#include "transform.h"
#include "camera.h"
#include "input.h"

namespace esl
{
	static const int PositionAttribute = 0;
	static const int UVAttribute = 1;

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
	};

	struct Texture
	{
		esl::uint name;
	};

	struct Material
	{
		short texture;
		short shader;

		Material(short Texture, short Shader) : texture(Texture), shader(Shader) {}
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
		AnimationClip clip;
		AnimationClip queuedClip;
		bool shouldUpdateMesh;

		Sprite(short Mesh, glm::vec2 FrameUVSize, AnimationClip Clip, AnimationClip QueuedClip) : time(0), mesh(Mesh), frameUVSize(FrameUVSize), clip(Clip), queuedClip(QueuedClip), shouldUpdateMesh(true) {}
	};

	class Object
	{
	public:
		short mesh;
		short material;
		esl::Transform transform;
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
		std::vector<esl::Material> materials;
		std::vector<std::shared_ptr<esl::Sprite>> sprites;
		std::vector<std::shared_ptr<esl::Object>> objects;
		std::vector<std::shared_ptr<esl::System>> systems;
	};
}