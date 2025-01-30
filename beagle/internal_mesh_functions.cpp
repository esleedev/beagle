#include <gl/glew.h>

#include "internal_mesh_functions.h"
#include "shape_types.h"

void esl_internal::UpdateLines(std::shared_ptr<esl::Resources> Resources)
{
	for (int mesh = 0; mesh < Resources->internalLineMeshes.size(); mesh++)
	{
		Resources->internalLineMeshes[mesh].vertices.clear();
		Resources->internalLineMeshes[mesh].indices.clear();
		int lineCount = Resources->internalLineMeshes[mesh].lines.size();
		if (lineCount == 0) continue;
		for (int line = 0; line < lineCount; line++)
		{
			Resources->internalLineMeshes[mesh].vertices.push_back(esl::LineVertex(Resources->internalLineMeshes[mesh].lines[line].pointA, Resources->internalLineMeshes[mesh].lines[line].pointB, Resources->internalLineMeshes[mesh].lines[line].pointA));
			Resources->internalLineMeshes[mesh].vertices.push_back(esl::LineVertex(Resources->internalLineMeshes[mesh].lines[line].pointA, Resources->internalLineMeshes[mesh].lines[line].pointB, Resources->internalLineMeshes[mesh].lines[line].pointA));
			Resources->internalLineMeshes[mesh].vertices.push_back(esl::LineVertex(Resources->internalLineMeshes[mesh].lines[line].pointB, Resources->internalLineMeshes[mesh].lines[line].pointB, Resources->internalLineMeshes[mesh].lines[line].pointA));
			Resources->internalLineMeshes[mesh].vertices.push_back(esl::LineVertex(Resources->internalLineMeshes[mesh].lines[line].pointB, Resources->internalLineMeshes[mesh].lines[line].pointB, Resources->internalLineMeshes[mesh].lines[line].pointA));
			esl::uint indexOffset = line * 4;
			Resources->internalLineMeshes[mesh].indices.push_back(indexOffset);
			Resources->internalLineMeshes[mesh].indices.push_back(indexOffset + 1);
			Resources->internalLineMeshes[mesh].indices.push_back(indexOffset + 2);
			Resources->internalLineMeshes[mesh].indices.push_back(indexOffset + 1);
			Resources->internalLineMeshes[mesh].indices.push_back(indexOffset + 3);
			Resources->internalLineMeshes[mesh].indices.push_back(indexOffset + 2);
		}
		glBindVertexArray(Resources->internalLineMeshes[mesh].vao);
		glBindBuffer(GL_ARRAY_BUFFER, Resources->internalLineMeshes[mesh].vbo);
		glBufferData(GL_ARRAY_BUFFER, Resources->internalLineMeshes[mesh].vertices.size() * sizeof(esl::LineVertex), &Resources->internalLineMeshes[mesh].vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Resources->internalLineMeshes[mesh].ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * lineCount * sizeof(esl::uint), &Resources->internalLineMeshes[mesh].indices[0], GL_STATIC_DRAW);
		glBindVertexArray(0);
	}
}

void esl_internal::UpdateTexts(std::shared_ptr<esl::Resources> Resources)
{
	for (int text = 0; text < Resources->texts.size(); text++)
	{
		if (!Resources->texts[text]->shouldUpdateMesh) continue;

		// get text alignment
		glm::vec2 origin = { 0, 0 };
		if (Resources->texts[text]->horizontalTextAlignment == esl::HorizontalTextAlignment::Middle)
			origin.x = 0.5f;
		else if (Resources->texts[text]->horizontalTextAlignment == esl::HorizontalTextAlignment::Right)
			origin.x = 1;
		if (Resources->texts[text]->verticalTextAlignment == esl::VerticalTextAlignment::Middle)
			origin.y = 0.5f;
		else if (Resources->texts[text]->verticalTextAlignment == esl::VerticalTextAlignment::Bottom)
			origin.y = 1;

		std::string string = Resources->texts[text]->string;
		std::shared_ptr<esl::Font> font = Resources->fonts[Resources->texts[text]->font];

		// get how many lines in string + the width of each line
		std::vector<float> lineWidths;
		float lineWidth = 0;
		for (int character = 0; character < string.size(); character++)
		{
			if (string[character] == '\n')
			{
				lineWidths.push_back(lineWidth);
				lineWidth = 0;
				continue;
			}
			// add glyph's width to line width
			int ascii = (int)string[character] - 32;
			lineWidth += font->glyphs[ascii].uvSize.x;
		}
		lineWidths.push_back(lineWidth);

		// get mesh
		short mesh = Resources->texts[text]->mesh;

		// clear mesh
		Resources->meshes[mesh].vertices.clear();
		Resources->meshes[mesh].indices.clear();

		// construct mesh
		// todo: replace height and adjust vertical alignment with font's actual height properties
		float height = 0.12f;
		glm::vec3 position = glm::vec3(-lineWidths[0] * origin.x, lineWidths.size() * height * origin.y, 0);
		int unskippedCharacterCount = 0;
		int line = 0;
		for (int character = 0; character < string.size(); character++)
		{
			if (string[character] == '\n')
			{
				// new line, reset position
				line++;
				position.x = -lineWidths[line] * origin.x;
				position.y -= height;
				continue;
			}
			int ascii = (int)string[character] - 32;
			esl::Vertex vertices[] =
			{
				esl::Vertex{ position, font->glyphs[ascii].uv + glm::vec2{0, font->glyphs[ascii].uvSize.y} },
				esl::Vertex{ position + glm::vec3{ 0, font->glyphs[ascii].uvSize.y, 0 }, font->glyphs[ascii].uv + glm::vec2{ 0, 0 }},
				esl::Vertex{ position + glm::vec3{ font->glyphs[ascii].uvSize.x, font->glyphs[ascii].uvSize.y, 0 }, font->glyphs[ascii].uv + glm::vec2{font->glyphs[ascii].uvSize.x, 0}},
				esl::Vertex{ position + glm::vec3{ font->glyphs[ascii].uvSize.x, 0, 0 }, font->glyphs[ascii].uv + font->glyphs[ascii].uvSize}
			};
			esl::uint indices[] =
			{
				unskippedCharacterCount * 4, unskippedCharacterCount * 4 + 1, unskippedCharacterCount * 4 + 2,
				unskippedCharacterCount * 4, unskippedCharacterCount * 4 + 2, unskippedCharacterCount * 4 + 3
			};
			Resources->meshes[mesh].indices.insert(Resources->meshes[mesh].indices.end(), indices, indices + 6);
			Resources->meshes[mesh].vertices.insert(Resources->meshes[mesh].vertices.end(), vertices, vertices + 4);
			position.x += font->glyphs[ascii].uvSize.x;
			unskippedCharacterCount++;
		}

		// update vao
		glBindVertexArray(Resources->meshes[mesh].vao);

		glBindBuffer(GL_ARRAY_BUFFER, Resources->meshes[mesh].vbo);
		glBufferData(GL_ARRAY_BUFFER, Resources->meshes[mesh].vertices.size() * sizeof(esl::Vertex), (string.size() > 0) ? &Resources->meshes[mesh].vertices[0] : nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Resources->meshes[mesh].ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Resources->meshes[mesh].indices.size() * sizeof(esl::uint), (string.size() > 0) ? &Resources->meshes[mesh].indices[0] : nullptr, GL_DYNAMIC_DRAW);

		Resources->texts[text]->shouldUpdateMesh = false;
	}
}
