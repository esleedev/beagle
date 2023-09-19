#pragma once

#include "shaders.h"
#include "vector.h"
#include "matrix.h"
#include <vector>

struct Mesh
{
	GLuint vao, vbo, ibo;
};

struct Transform
{
	Vector position;
	float yaw;
	Matrix matrix;
};

class Object
{
public:
	short mesh;
	Transform transform;
};

class Game
{
public:
	GLuint shaderProgram;

	GLint positionAttribute;
	GLint objectMatrixUniform;
	GLint projectionMatrixLocation, viewMatrixLocation;

	Matrix projectionMatrix;
	Transform cameraTransform;

	std::vector<Mesh> meshes;
	std::vector<Object*> objects;

	// projection settings
	float aspectRatio, verticalFieldOfViewInDegrees, near, far;

	Game();
	~Game();

	void Update(float DeltaTime);
	void Render();
};
