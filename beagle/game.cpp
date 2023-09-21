#include "game.h"
#include "vector.h"
#include "texture.h"
#include <iostream>

Game::Game()
{
	shaderProgram = CreateShaderProgram("shaders/vertexShader.txt", "shaders/fragmentShader.txt");
	positionAttribute = glGetAttribLocation(shaderProgram, "inPosition");
    uvAttribute = glGetAttribLocation(shaderProgram, "inUV");
    objectMatrixUniform = glGetUniformLocation(shaderProgram, "objectMatrix");
    projectionMatrixUniform = glGetUniformLocation(shaderProgram, "projectionMatrix");
    viewMatrixUniform = glGetUniformLocation(shaderProgram, "viewMatrix");
    textureUniform = glGetUniformLocation(shaderProgram, "texture");

    float z = 0;

    Vertex vertices[] =
    {
        Vertex{ { 0 - 0.5, 0 - 0.5, 0 - 0.5 }, { 0, 1 } },
        Vertex{ { 1 - 0.5, 0 - 0.5, 0 - 0.5 }, { 1, 1 } },
        Vertex{ { 0 - 0.5, 1 - 0.5, 0 - 0.5 }, { 0, 0 } },
        Vertex{ { 1 - 0.5, 1 - 0.5, 0 - 0.5 }, { 1, 0 } }
    };

    GLuint indexData[] = { 0, 2, 1, 1, 2, 3 };

    Mesh cube;
    glGenVertexArrays(1, &cube.vao);
    glBindVertexArray(cube.vao);

    glGenBuffers(1, &cube.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cube.vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(positionAttribute);
    glEnableVertexAttribArray(uvAttribute);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);

    glGenBuffers(1, &cube.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
    meshes.push_back(cube);

    GLuint texture = LoadTexture("textures/spriteSheet.png");
    Material material = { texture };
    materials.push_back(material);

    Transform cubeTransform = {};
    for (int x = 0; x < 8; x++)
    {
        cubeTransform.position = { x * 1.25f, cos(x * 1.9f) * 0.4f, 5.0f + sin(x * 0.5f) + x % 2 };
        cubeTransform.shouldUpdateMatrix = true;
        objects.push_back(new Object{ 0, 0, cubeTransform });
    }

    projectionMatrix.SetIdentity();
    cameraTransform = {};
    cameraTransform.matrix.SetIdentity();

    aspectRatio = 4.0f / 3.0f;
    verticalFieldOfViewInDegrees = 30.0f;
    near = 0.5f;
    far = 100.0f;

    input = new Input();
}

Game::~Game()
{
	DeleteShaderProgram(shaderProgram);

    for (int object = 0; object < objects.size(); object++)
    {
        delete objects[object];
    }

    delete input;
}

void Game::Update(float DeltaTime)
{
    if (input->isKeyPressed[SDL_SCANCODE_D])
    {
        cameraTransform.position.x += 0.5 * DeltaTime;
    }
    else if (input->isKeyPressed[SDL_SCANCODE_A])
    {
        cameraTransform.position.x -= 0.5 * DeltaTime;
    }

    if (input->isKeyPressed[SDL_SCANCODE_LEFT])
    {
        cameraTransform.yaw -= 30.0f * DeltaTime;
    }
    else if (input->isKeyPressed[SDL_SCANCODE_RIGHT])
    {
        cameraTransform.yaw += 30.0f * DeltaTime;
    }

    for (int object = 0; object < objects.size(); object++)
    {
        if (objects[object]->transform.shouldUpdateMatrix)
        {
            objects[object]->transform.UpdateMatrix();
            objects[object]->transform.shouldUpdateMatrix = false;
        }
    }

    cameraTransform.matrix.SetTranslationAndRotation(-cameraTransform.position, cameraTransform.yaw);

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
    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_TRUE, &projectionMatrix.matrix[0][0]);
    glUniformMatrix4fv(viewMatrixUniform, 1, GL_TRUE, &cameraTransform.matrix.matrix[0][0]);

    for (int object = 0; object < objects.size(); object++)
    {
        glBindVertexArray(meshes[objects[object]->mesh].vao);
        glUniformMatrix4fv(objectMatrixUniform, 1, GL_TRUE, &objects[object]->transform.matrix.matrix[0][0]);
        glUniform1i(materials[objects[object]->material].texture, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    }

}