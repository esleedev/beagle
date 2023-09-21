#pragma once

#include "shaders.h"
#include "vector.h"
#include "matrix.h"
#include "transform.h"
#include <vector>

struct Vertex
{
	Vector position;
	Vector2D uv;
};

struct Mesh
{
	GLuint vao, vbo, ibo;
};

struct Material
{
	GLuint texture;
};

class Object
{
public:
	short mesh;
	short material;
	Transform transform;
};

class Game
{
public:
	GLuint shaderProgram;

	GLint positionAttribute;
	GLint uvAttribute;
	GLint objectMatrixUniform;
	GLint projectionMatrixUniform, viewMatrixUniform;
	GLint textureUniform;

	Matrix projectionMatrix;
	Transform cameraTransform;

	std::vector<Mesh> meshes;
	std::vector<Material> materials;
	std::vector<Object*> objects;

	// projection settings
	float aspectRatio, verticalFieldOfViewInDegrees, near, far;

	Game();
	~Game();

	void Update(float DeltaTime);
	void Render();
};
