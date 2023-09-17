#include "game.h"
#include "vector.h"
#include <iostream>

Game::Game()
{
	shaderProgram = CreateShaderProgram("shaders/vertexShader.txt", "shaders/fragmentShader.txt");
	positionAttribute = glGetAttribLocation(shaderProgram, "inPosition");
    projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");

    float z = 0;
    GLfloat vertexData[] =
    {
        0, 0, z,
        1, 0, z,
        0, 1, z
    };

    GLuint indexData[] = { 0, 1, 2 };
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
    
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(positionAttribute);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

    projectionMatrix.SetIdentity();
    viewMatrix.SetIdentity();
}

Game::~Game()
{
	DeleteShaderProgram(shaderProgram);
}

void Game::Update(float DeltaTime)
{
    viewMatrix.Move(Vector{ DeltaTime * 0.25f, 0, 0 });
}

void Game::Render()
{
	glUseProgram(shaderProgram);
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_TRUE, &projectionMatrix.matrix[0][0]);
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_TRUE, &viewMatrix.matrix[0][0]);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
}