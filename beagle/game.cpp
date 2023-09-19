#include "game.h"
#include "vector.h"
#include <iostream>

Game::Game()
{
	shaderProgram = CreateShaderProgram("shaders/vertexShader.txt", "shaders/fragmentShader.txt");
	positionAttribute = glGetAttribLocation(shaderProgram, "inPosition");
    objectMatrixUniform = glGetUniformLocation(shaderProgram, "objectMatrix");
    projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");

    float z = 0;
    GLfloat vertexData[] =
    {
        0 - 0.5, 0 - 0.5, z - 0.5,
        1 - 0.5, 0 - 0.5, z - 0.5,
        0 - 0.5, 1 - 0.5, z - 0.5,
        1 - 0.5, 1 - 0.5, z - 0.5,
        0 - 0.5, 0 - 0.5, z + 0.5,
        0 - 0.5, 1 - 0.5, z + 0.5,
        1 - 0.5, 1 - 0.5, z + 0.5,
        1 - 0.5, 0 - 0.5, z + 0.5,
    };

    GLuint indexData[] = { 0, 2, 1, 1, 2, 3, 4, 5, 0, 0, 5, 2, 2, 5, 6, 2, 6, 3, 1, 3, 6, 1, 6, 7, 6, 4, 7, 6, 5, 4 };

    Mesh cube;
    glGenVertexArrays(1, &cube.vao);
    glBindVertexArray(cube.vao);

    glGenBuffers(1, &cube.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cube.vbo);
    glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
    
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(positionAttribute);

    glGenBuffers(1, &cube.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 30 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
    meshes.push_back(cube);

    Transform cubeTransform = {};
    for (int x = 0; x < 8; x++)
    {
        cubeTransform.position = { x * 1.25f, cos(x * 1.9f) * 0.4f, 5.0f + sin(x * 0.5f) + x % 2 };
        cubeTransform.UpdateMatrix();
        objects.push_back(new Object{ 0, cubeTransform });
    }

    projectionMatrix.SetIdentity();
    cameraTransform = {};
    cameraTransform.matrix.SetIdentity();

    aspectRatio = 4.0f / 3.0f;
    verticalFieldOfViewInDegrees = 30.0f;
    near = 0.5f;
    far = 100.0f;
}

Game::~Game()
{
	DeleteShaderProgram(shaderProgram);

    for (int object = 0; object < objects.size(); object++)
    {
        delete objects[object];
    }
}

void Game::Update(float DeltaTime)
{
    //viewMatrix.Move(Vector{ DeltaTime * 0.25f, DeltaTime * -0.1f, 0 });

    cameraTransform.yaw += DeltaTime * 0.59f;
    cameraTransform.position.x -= 0.3 * DeltaTime;

    Matrix translationMatrix, rotationMatrix;
    translationMatrix.SetTranslation(cameraTransform.position);
    rotationMatrix.SetRotation(sin(cameraTransform.yaw) * 35.0f);
    cameraTransform.matrix.SetIdentity();
    cameraTransform.matrix = rotationMatrix * translationMatrix;

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
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_TRUE, &cameraTransform.matrix.matrix[0][0]);

    for (int object = 0; object < objects.size(); object++)
    {
        glBindVertexArray(meshes[objects[object]->mesh].vao);
        glUniformMatrix4fv(objectMatrixUniform, 1, GL_TRUE, &objects[object]->transform.matrix.matrix[0][0]);
        glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, NULL);
    }

}