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
