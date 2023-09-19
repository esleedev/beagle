#include "game.h"
#include "vector.h"
#include <iostream>

Game::Game()
{
	shaderProgram = CreateShaderProgram("shaders/vertexShader.txt", "shaders/fragmentShader.txt");
	positionAttribute = glGetAttribLocation(shaderProgram, "inPosition");
    projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");

    float z = 2;
    GLfloat vertexData[] =
    {
        0 + 0.5, 0 - 0.5, z,
        1 + 0.5, 0 - 0.5, z,
        0 + 0.5, 1 - 0.5, z,
        1 + 0.5, 1 - 0.5, z,
        0 + 0.5, 0 - 0.5, z + 1,
        0 + 0.5, 1 - 0.5, z + 1,
        1 + 0.5, 1 - 0.5, z + 1,
        1 + 0.5, 0 - 0.5, z + 1,

    };

    GLuint indexData[] = { 0, 2, 1, 1, 2, 3, 4, 5, 0, 0, 5, 2, 2, 5, 6, 2, 6, 3, 1, 3, 6, 1, 6, 7, 6, 4, 7, 6, 5, 4 };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
    
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(positionAttribute);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 30 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

    projectionMatrix.SetIdentity();
    viewMatrix.SetIdentity();

    aspectRatio = 4.0f / 3.0f;
    verticalFieldOfViewInDegrees = 30.0f;
    near = 0.5f;
    far = 100.0f;

    cameraObject = {};
}

Game::~Game()
{
	DeleteShaderProgram(shaderProgram);
}

void Game::Update(float DeltaTime)
{
    //viewMatrix.Move(Vector{ DeltaTime * 0.25f, DeltaTime * -0.1f, 0 });

    cameraObject.yaw += DeltaTime * 0.59f;
    cameraObject.position.x -= 0.3 * DeltaTime;

    Matrix translationMatrix, rotationMatrix;
    translationMatrix.SetTranslation(cameraObject.position);
    rotationMatrix.SetRotation(sin(cameraObject.yaw) * 35.0f);
    viewMatrix.SetIdentity();
    viewMatrix = rotationMatrix * translationMatrix;

    // update projection matrix
    float projectionAngleHalved = tan(verticalFieldOfViewInDegrees * 0.5f * 0.0174532925199f);
    projectionMatrix.SetIdentity();
    projectionMatrix.matrix[0][0] = 1.0f / (projectionAngleHalved * aspectRatio);
    projectionMatrix.matrix[1][1] = 1.0f / projectionAngleHalved;
    projectionMatrix.matrix[2][2] = (-far - near) / (near - far);
    projectionMatrix.matrix[2][3] = (2.0f * far * near) / (near - far);
    projectionMatrix.matrix[3][2] = 1.0f;
    projectionMatrix.matrix[3][3] = 0.0f;
}

void Game::Render()
{
	glUseProgram(shaderProgram);
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_TRUE, &projectionMatrix.matrix[0][0]);
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_TRUE, &viewMatrix.matrix[0][0]);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, NULL);
}