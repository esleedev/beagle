#include <gl/glew.h>
#include <iostream>
#include <algorithm>

#include "text_system.h"

void esl::TextSystem::UpdateTexts(std::shared_ptr<esl::Resources> Resources)
{
	// sort texts by order of shared meshes
	std::sort
	(
		Resources->texts.begin(), Resources->texts.end(),
		[](std::shared_ptr<esl::Text>& const A, std::shared_ptr<esl::Text>& const B) { return A->sharedMesh < B->sharedMesh; }
	);

	short lastSharedMesh = -1;
	std::vector<short> usedMeshes;
	for (int text = 0; text < Resources->texts.size(); text++)
	{
		short sharedMesh = Resources->texts[text]->sharedMesh;
		if (sharedMesh != lastSharedMesh)
		{
			// first of its kind, so we can safely clear the mesh
			Resources->meshes[sharedMesh].vertices.clear();
			Resources->meshes[sharedMesh].indices.clear();
			// record it too for later
			usedMeshes.push_back(sharedMesh);
		}

		int characterCount = Resources->texts[text]->text.size();
		glm::vec2 characterSize = glm::vec2(Resources->texts[text]->size / Resources->camera.aspectRatio, Resources->texts[text]->size);
		glm::vec2 characterUVSize = glm::vec2(1.0f / 16.0f); // temp uv size
		float letterSpacing = Resources->texts[text]->letterSpacing;
		float totalSizeInLine = 0.0f;

		// Adjust the txt origin for text alignment
		esl::HorizontalTextAlignment horizontalAlignment = Resources->texts[text]->horizontalAlignment;
		esl::VerticalTextAlignment verticalAlignment = Resources->texts[text]->verticalAlignment;
		glm::vec2 textPosition = Resources->texts[text]->position;
		if (horizontalAlignment == esl::HorizontalTextAlignment::Right)
			textPosition.x -= characterSize.x * characterCount;
		if (verticalAlignment == esl::VerticalTextAlignment::Middle)
			textPosition.y -= characterSize.y * 0.5f;
		else if (verticalAlignment == esl::VerticalTextAlignment::Top)
			textPosition.y -= characterSize.y;

		glm::vec3 color = Resources->texts[text]->color;

		for (int character = 0; character < characterCount; character++)
		{
			glm::vec3 position = glm::vec3(textPosition.x + characterSize.x * character + letterSpacing * character, textPosition.y, 0);
			esl::ubyte asciiValue = Resources->texts[text]->text[character];
			glm::vec2 uv = glm::vec2(asciiValue % 16, floor((float)asciiValue / 16.0f)) * characterUVSize;
			esl::Vertex vertices[] =
			{
				esl::Vertex{ position,                                                  uv + glm::vec2 { 0.0f, 1.0f } * characterUVSize, color },
				esl::Vertex{ position + glm::vec3(0, characterSize.y, 0),               uv, color },
				esl::Vertex{ position + glm::vec3(characterSize.x, characterSize.y, 0), uv + glm::vec2 { 1.0f, 0.0f } * characterUVSize, color },
				esl::Vertex{ position + glm::vec3(characterSize.x, 0, 0),               uv + glm::vec2 { 1.0f, 1.0f } * characterUVSize, color }
			};
			Resources->meshes[sharedMesh].vertices.insert(Resources->meshes[sharedMesh].vertices.end(), vertices, vertices + 4);
		}

		lastSharedMesh = sharedMesh;
	}

	for (int usedMesh = 0; usedMesh < usedMeshes.size(); usedMesh++)
	{
		// check if there are more characters than allocated in mesh
		if (Resources->meshes[usedMeshes[usedMesh]].indices.size() / 6 < Resources->meshes[usedMeshes[usedMesh]].vertices.size() / 4)
		{
			int lastVertexCount = 0;
			for (int character = 0; character < Resources->meshes[usedMeshes[usedMesh]].vertices.size() / 4; character++)
			{
				esl::uint indices[] = { character * 4, character * 4 + 1, character * 4 + 2, character * 4, character * 4 + 2, character * 4 + 3 };
				Resources->meshes[usedMeshes[usedMesh]].indices.insert(Resources->meshes[usedMeshes[usedMesh]].indices.end(), indices, indices + 6);
			}

			// remake mesh
			glBindVertexArray(Resources->meshes[usedMeshes[usedMesh]].vao);

			glBindBuffer(GL_ARRAY_BUFFER, Resources->meshes[usedMeshes[usedMesh]].vbo);
			glBufferData(GL_ARRAY_BUFFER, Resources->meshes[usedMeshes[usedMesh]].vertices.size() * sizeof(esl::Vertex), &Resources->meshes[usedMeshes[usedMesh]].vertices[0], GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Resources->meshes[usedMeshes[usedMesh]].ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, Resources->meshes[usedMeshes[usedMesh]].indices.size() * sizeof(esl::uint), &Resources->meshes[usedMeshes[usedMesh]].indices[0], GL_DYNAMIC_DRAW);
		}
		else
		{
			// update mesh if we don't have to change ibo
			glBindBuffer(GL_ARRAY_BUFFER, Resources->meshes[usedMeshes[usedMesh]].vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, Resources->meshes[usedMeshes[usedMesh]].vertices.size() * sizeof(esl::Vertex), &Resources->meshes[usedMeshes[usedMesh]].vertices[0]);
		}
	}
	
}
