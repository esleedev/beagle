#include "game.h"
#include "vector.h"
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
            glBindVertexArray(meshes[objects[object]->mesh].vao);
            glUniformMatrix4fv(shaders[shader].objectMatrixUniform, 1, GL_TRUE, &objects[object]->transform.matrix.matrix[0][0]);
            glUniform1i(materials[objects[object]->material].texture, 0);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
            object++;
        }
    }
}