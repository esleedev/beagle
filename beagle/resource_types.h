#pragma once

#include <SDL_ttf.h>

#include <vector>
#include <memory>
#include <string>
#include <filesystem>

#include "common_types.h"
#include "vertex.h"
#include "transform.h"
#include "camera.h"
#include "input.h"

namespace esl
{
	static const int PositionAttribute = 0;
	static const int UVAttribute = 1;
	static const int ColorAttribute = 2;

	static const short TransparentRenderOrder = 1024;

	struct FileInformation
	{
		std::string path;
		std::filesystem::file_time_type lastWriteTime;
	};

	enum VertexDescription : esl::ubyte
	{
		HasAll = 0,
		HasPosition
	};

	struct Mesh
	{
		esl::uint vao, vbo, ibo;
		std::vector<esl::Vertex> vertices;
		std::vector<esl::uint> indices;
		std::shared_ptr<esl::FileInformation> fileInformation;
		esl::VertexDescription vertexDescription = VertexDescription::HasAll;
	};

	struct Shader
	{
		esl::uint program;
		int objectMatrixUniform;
		int projectionMatrixUniform;
		int viewMatrixUniform;
		int textureUniform;
		int diffuseColorUniform;
	};

	struct Texture
	{
		esl::uint name;
		std::shared_ptr<esl::FileInformation> fileInformation;
	};

	struct RenderTarget
	{
		esl::uint frameBufferName;
		esl::uint depthBufferName;
		esl::uint textureName;
		esl::Mesh mesh;
		short shader;
		glm::vec4 diffuseColor; // shader setting
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

	enum class HorizontalTextAlignment : char
	{
		Left = 0,
		Right = 1,
		Middle = 2
	};

	enum class VerticalTextAlignment : char
	{
		Bottom = 0,
		Top = 1,
		Middle = 2
	};

	class Object
	{
	public:
		short mesh;
		std::shared_ptr<esl::Material> material;
		esl::Transform transform;
		glm::vec4 diffuseColor;
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

	class Event
	{
	public:
		virtual void OnEvent(esl::System* System, esl::ushort ID) = 0;
	};

	class Resources
	{
	public:
		esl::Camera camera;
		esl::RenderTarget renderTarget;
		std::vector<TTF_Font*> fonts;
		std::vector<esl::Mesh> meshes;
		std::vector<esl::Shader> shaders;
		std::vector<esl::Texture> textures;
		std::vector<std::shared_ptr<esl::Material>> materials;
		std::vector<std::shared_ptr<esl::Sprite>> sprites;
		std::vector<std::shared_ptr<esl::Object>> objects;
		std::vector<std::shared_ptr<esl::System>> systems;
		std::vector<std::shared_ptr<esl::System>> queuedSystems;
		std::vector<std::shared_ptr<esl::Event>> events;
	};
}