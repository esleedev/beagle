#pragma once

#include "shaders.h"
#include "vector.h"
#include "matrix.h"
#include "transform.h"
#include <vector>

struct Mesh
{
	GLuint vao, vbo, ibo;
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
