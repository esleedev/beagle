#include "game.h"
#include <iostream>

Game::Game()
{
	shaderProgram = CreateShaderProgram("shaders/vertexShader.txt", "shaders/fragmentShader.txt");
	positionAttribute = glGetAttribLocation(shaderProgram, "inPosition");

    float z = 0;
    GLfloat vertexData[] =
    {
        0, 0, z,
        1, 0, z,
        0, 1, z,
        1, 1, z
    };

    GLuint indexData[] = { 0, 1, 2, 2, 1, 3 };
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
    
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(positionAttribute);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
}

Game::~Game()
{
	DeleteShaderProgram(shaderProgram);
}

void Game::Update(float DeltaTime)
{

}

void Game::Render()
{
	glUseProgram(shaderProgram);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}