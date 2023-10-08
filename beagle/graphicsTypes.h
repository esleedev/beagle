#pragma once

#include <gl/glew.h>
#include "vector.h"

struct Vertex
{
    Vector position;
    Vector2D uv;
};

struct Mesh
{
    GLuint vao, vbo, ibo;
};

struct Shader
{
    GLuint shaderProgram;
    GLint objectMatrixUniform;
    GLint projectionMatrixUniform;
    GLint viewMatrixUniform;
    GLint textureUniform;
};

struct Material
{
	GLuint texture;
	int shader;
};

struct AnimationClip
{
	short frameStart;
	short frameCount;
	float speed;
};

struct Sprite
{
	float time;
	AnimationClip clip;
	AnimationClip queuedClip;
	Vector2D frameUVSize;

	void SetClip(AnimationClip Clip, bool ShouldResetTimeIfNewClip = true);
	void QueueClip(AnimationClip Clip);

	Rectangle GetUVs();
};

class SpriteMesh
{
public:
	Sprite sprite;
	Vertex vertices[4];
	GLuint vbo;

	void UpdateVertices()
	{
		vertices[0].position = { 0 - 0.5, 0 - 0.5, 0 - 0.5 };
		vertices[1].position = { 1 - 0.5, 0 - 0.5, 0 - 0.5 };
		vertices[2].position = { 0 - 0.5, 1 - 0.5, 0 - 0.5 };
		vertices[3].position = { 1 - 0.5, 1 - 0.5, 0 - 0.5 };

		Rectangle uvs = sprite.GetUVs();
		vertices[0].uv = { uvs.x, uvs.y + uvs.height };
		vertices[1].uv = { uvs.x + uvs.width, uvs.y + uvs.height };
		vertices[2].uv = { uvs.x, uvs.y };
		vertices[3].uv = { uvs.x + uvs.width, uvs.y };
	}
};

struct Line2D
{
	Vector2D pointA, pointB;
	float width;
};

struct Line
{
	Vector pointA, pointB;
	float width;
};