#pragma once

#include <vector>
#include <memory>

#include "common_types.h"
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
		esl::ushort indexCount;
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
		std::vector<Mesh> meshes;
		std::vector<Shader> shaders;
		std::vector<Texture> textures;
		std::vector<Material> materials;
		std::vector<std::shared_ptr<Object>> objects;
		std::vector<std::shared_ptr<System>> systems;
	};
}