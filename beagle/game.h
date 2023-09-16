#pragma once

#include "shaders.h"

class Game
{
public:
	GLuint shaderProgram;
	GLint positionAttribute;

	GLuint vbo, ibo, vao;

	Game();
	~Game();

	void Update(float DeltaTime);
	void Render();
};