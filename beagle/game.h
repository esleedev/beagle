#pragma once

#include "shaders.h"
#include "vector.h"
#include "matrix.h"

struct CameraObject
{
	Vector position;
	float yaw;
};

class Game
{
public:
	GLuint shaderProgram;

	GLint positionAttribute;
	GLint projectionMatrixLocation, viewMatrixLocation;

	GLuint vao, vbo, ibo;

	Matrix projectionMatrix, viewMatrix;
	// projection settings
	float aspectRatio, verticalFieldOfViewInDegrees, near, far;

	CameraObject cameraObject;

	Game();
	~Game();

	void Update(float DeltaTime);
	void Render();
};
