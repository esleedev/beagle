#pragma once

#include "shaders.h"
#include "matrix.h"

class Game
{
public:
	GLuint shaderProgram;

	GLint positionAttribute;
	GLint projectionMatrixLocation, viewMatrixLocation;

	GLuint vbo, ibo, vao;

	Matrix projectionMatrix, viewMatrix;
	// projection settings
	float aspectRatio, verticalFieldOfViewInDegrees, near, far;

	Game();
	~Game();

	void Update(float DeltaTime);
	void Render();
};
