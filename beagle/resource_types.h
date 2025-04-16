#pragma once

#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <vector>
#include <memory>
#include <string>
#include <filesystem>

#include "common_types.h"
#include "vertex.h"
#include "transform.h"
#include "camera.h"
#include "input.h"
#include "audio.h"

namespace esl
{
	// default rendering attributes
	static const int PositionAttribute = 0;
	static const int UVAttribute = 1;
	static const int ColorAttribute = 2;

	// line rendering attributes
	static const int NextPositionAttribute = 1;
	static const int PreviousPositionAttribute = 2;

	static const short TransparentRenderOrder = 1024;

	static const esl::ushort HashIndexedMaterialsSize = 256;

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

	struct Line;

	struct LineMesh
	{
		esl::uint vao, vbo, ibo;
		std::vector<esl::Line> lines;
		std::vector<esl::LineVertex> vertices;
		std::vector<esl::uint> indices;
		glm::vec4 diffuseColor;
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
		std::string name;

		Material(short Texture, short Shader, short RenderOrder) : texture(Texture), shader(Shader), renderOrder(RenderOrder), name("") {}
	};

	struct AnimationClip
	{
		short frameStart;
		short frameCount;
		float speed;

		bool operator==(esl::AnimationClip OtherClip) { return OtherClip.frameStart == frameStart && OtherClip.frameCount == frameCount && OtherClip.speed == speed; }
		bool operator!=(esl::AnimationClip OtherClip) { return OtherClip.frameStart != frameStart || OtherClip.frameCount != frameCount || OtherClip.speed != speed; }
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

	class Text
	{
	public:
		short mesh;
		short font;
		std::string string;
		esl::HorizontalTextAlignment horizontalTextAlignment;
		esl::VerticalTextAlignment verticalTextAlignment;
		glm::vec2 areaSize;
		bool shouldUpdateMesh;
	};

	class Resources;

	class System : public std::enable_shared_from_this<esl::System>
	{
	public:
		virtual void Start(std::shared_ptr<esl::Resources> Resources) {};
		virtual void Update
		(
			float DeltaTime,
			esl::Input* const Input,
			esl::Audio* const Audio,
			std::shared_ptr<esl::Resources> Resources
		) = 0;
	};

	class Event
	{
	public:
		std::shared_ptr<esl::Resources> resources;
		esl::ushort id;
		virtual void OnEvent(std::shared_ptr<esl::System> System) = 0;
	};

	struct InternalEventTimer
	{
		std::shared_ptr<esl::System> system;
		esl::ushort id;
		float time;
	};

	struct Glyph
	{
		glm::vec2 uv;
		glm::vec2 uvSize;
	};

	struct Font
	{
		TTF_Font* font;
		std::shared_ptr<esl::Material> material;
		std::vector<esl::Glyph> glyphs;
	};

	struct SoundEffect
	{
		Mix_Chunk* audioChunk;
	};

	class Resources
	{
	public:
		esl::Camera camera;
		esl::RenderTarget renderTarget;
		std::vector<esl::Mesh> meshes;
		std::vector<esl::Shader> shaders;
		std::vector<esl::Texture> textures;
		std::vector<esl::SoundEffect> soundEffects;
		std::vector<std::shared_ptr<esl::Material>> materials;
		std::vector<std::shared_ptr<esl::Sprite>> sprites;
		std::vector<std::shared_ptr<esl::Font>> fonts;
		std::vector<std::shared_ptr<esl::Text>> texts;
		std::vector<std::shared_ptr<esl::Object>> objects;
		std::vector<std::shared_ptr<esl::System>> systems;
		std::vector<std::shared_ptr<esl::System>> queuedSystems;
		std::vector<std::shared_ptr<esl::Event>> events;
		std::vector<esl::InternalEventTimer> internalEventTimers;
		std::vector<esl::LineMesh> internalLineMeshes;
		std::shared_ptr<esl::Material> internalLineMaterial;
		std::shared_ptr<esl::Material> internalHashIndexedMaterials[esl::HashIndexedMaterialsSize];
	};
}