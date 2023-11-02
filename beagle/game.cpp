#include "game.h"
#include "vector.h"
#include "mesh.h"
#include "shader.h"
#include <vector>
#include <iostream>

Game::Game()
{
    input = new Input();

    projectionMatrix.SetIdentity();
    cameraTransform = {};
    cameraTransform.matrix.SetIdentity();

    aspectRatio = 4.0f / 3.0f;
    verticalFieldOfViewInDegrees = 30.0f;
    near = 0.5f;
    far = 100.0f;

    OnGameStart(this);
}

Game::~Game()
{
    OnGameEnd(this);

    for (int mesh = 0; mesh < meshes.size(); mesh++)
    {
        glDeleteVertexArrays(1, &meshes[mesh]->vao);
        glDeleteBuffers(1, &meshes[mesh]->vbo);
        glDeleteBuffers(1, &meshes[mesh]->ibo);

        delete meshes[mesh];
    }

    for (int mesh = 0; mesh < dynamicMeshes.size(); mesh++)
    {
        delete dynamicMeshes[mesh];
    }

    for (int mesh = 0; mesh < spriteMeshes.size(); mesh++)
    {
        delete spriteMeshes[mesh];
    }

    for (int system = 0; system < systems.size(); system++)
    {
        delete systems[system];
    }

    for (int object = 0; object < objects.size(); object++)
    {
        delete objects[object];
    }

    delete input;
}

void Game::Update(float DeltaTime)
{
    //for (Uint8 device = 0; device < Input::MaximumGamepadCount; device++)
    //{
    //    std::cout << (int)device << ": left stick (" << input->gamepadInputs[device].leftStick.x << ", " <<
    //        input->gamepadInputs[device].leftStick.y << ") right stick (" <<
    //        input->gamepadInputs[device].rightStick.x << ", " <<
    //        input->gamepadInputs[device].rightStick.y << ")" << std::endl;
    //}

    for (int system = 0; system < systems.size(); system++)
    {
        systems[system]->Update(DeltaTime, this);
    }

    for (int object = 0; object < objects.size(); object++)
    {
        if (objects[object]->transform.shouldUpdateMatrix)
        {
            objects[object]->transform.UpdateMatrix();
            objects[object]->transform.shouldUpdateMatrix = false;
        }
    }

    for (int mesh = 0; mesh < spriteMeshes.size(); mesh++)
    {
        // update sprite animation
        SpriteMesh* spriteMesh = spriteMeshes[mesh];
        spriteMesh->sprite.time += DeltaTime * spriteMesh->sprite.clip.speed;
        if (spriteMesh->sprite.time >= spriteMesh->sprite.clip.frameCount)
        {
            spriteMesh->sprite.clip = spriteMesh->sprite.queuedClip;
            spriteMesh->sprite.time = 0;
        }

        // update sprite mesh
        spriteMesh->UpdateVertices();
        glBindBuffer(GL_ARRAY_BUFFER, spriteMesh->vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), spriteMesh->vertices, GL_STATIC_DRAW);
    }

    for (int mesh = 0; mesh < dynamicMeshes.size(); mesh++)
    {
        // update dynamic mesh vbos
        DynamicMesh* dynamicMesh = dynamicMeshes[mesh];

        bool isEmpty = (dynamicMesh->vertices.size() == 0);
        glBindBuffer(GL_ARRAY_BUFFER, dynamicMesh->mesh->vbo);
        glBufferData(GL_ARRAY_BUFFER, dynamicMesh->vertices.size() * sizeof(Vertex), (isEmpty) ? nullptr : &dynamicMesh->vertices[0], GL_STATIC_DRAW);

        dynamicMesh->mesh->indexCount = dynamicMesh->indices.size();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dynamicMesh->mesh->ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, dynamicMesh->mesh->indexCount * sizeof(GLuint), (isEmpty) ? nullptr : &dynamicMesh->indices[0], GL_STATIC_DRAW);
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
    int object = 0;
    for (int shader = 0; shader < shaders.size(); shader++)
    {
        glUseProgram(shaders[shader].shaderProgram);
        glUniformMatrix4fv(shaders[shader].projectionMatrixUniform, 1, GL_TRUE, &projectionMatrix.matrix[0][0]);
        glUniformMatrix4fv(shaders[shader].viewMatrixUniform, 1, GL_TRUE, &cameraTransform.matrix.matrix[0][0]);

        // objects assumed to be sorted by order of shaders
        while (object < objects.size() && materials[objects[object]->material].shader == shader)
        {
            glBindTexture(GL_TEXTURE_2D, materials[objects[object]->material].texture);
            glBindVertexArray(meshes[objects[object]->mesh]->vao);
            glUniformMatrix4fv(shaders[shader].objectMatrixUniform, 1, GL_TRUE, &objects[object]->transform.matrix.matrix[0][0]);
            glUniform1i(materials[objects[object]->material].texture, 0);
            glDrawElements(GL_TRIANGLES, meshes[objects[object]->mesh]->indexCount, GL_UNSIGNED_INT, NULL);
            object++;
        }
    }
}

SpriteMesh* Game::AddNewSpriteMesh(Vector2D Size, Vector2D Origin, Vector2D FrameUVSize)
{
    Mesh* mesh = GenerateNewQuad();
    meshes.push_back(mesh);

    SpriteMesh* spriteMesh = new SpriteMesh();
    spriteMesh->vbo = mesh->vbo;
    spriteMesh->size = Size;
    spriteMesh->origin = Origin;
    spriteMesh->sprite.frameUVSize = FrameUVSize;
    spriteMesh->sprite.SetClip({ 0, 1, 1.0f }, true);
    spriteMeshes.push_back(spriteMesh);

    return spriteMesh;
}

void Game::AddNewShader(std::string VertexShaderFilePath, std::string FragmentShaderFilePath, int& Index)
{
    GLuint program = CreateShaderProgram(VertexShaderFilePath, FragmentShaderFilePath);
    Shader shader = GetShaderWithLocations(program);
    shaders.push_back(shader);
    Index = shaders.size() - 1;
}

void Game::AddMesh(Mesh* Mesh, int& Index)
{
    meshes.push_back(Mesh);
    Index = meshes.size() - 1;
}

Object::Object(short Mesh, short Material)
{
    mesh = Mesh;
    material = Material;
    transform = {};
    transform.shouldUpdateMatrix = true;
    spriteMesh = nullptr;
}

Object::Object(short Mesh, short Material, Transform Transform, SpriteMesh* SpriteMesh)
{
    mesh = Mesh;
    material = Material;
    transform = Transform;
    transform.shouldUpdateMatrix = true;
    spriteMesh = SpriteMesh;
}