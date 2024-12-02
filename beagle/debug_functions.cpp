#include <gl/glew.h>
#include <iostream>

#include "debug_functions.h"
#include "mesh_functions.h"
#include "shape_types.h"

void esl_debug::DrawLine(std::shared_ptr<esl::Resources> Resources, esl::Line Line, glm::vec4 DiffuseColor)
{
	if (Resources->internalLineMeshes.size() == 0)
	{
		Resources->internalLineMeshes.push_back(esl::GenerateLineMesh(Line));
		Resources->internalLineMeshes[0].diffuseColor = DiffuseColor;
	}
	else
	{
		int sharedLineMesh = -1;
		for (int lineMesh = 0; lineMesh < Resources->internalLineMeshes.size(); lineMesh++)
		{
			// use same line mesh if it shares color
			if (Resources->internalLineMeshes[lineMesh].diffuseColor == DiffuseColor)
				sharedLineMesh = lineMesh;
		}
		if (sharedLineMesh == -1)
		{
			Resources->internalLineMeshes.push_back(esl::GenerateLineMesh(Line));
			sharedLineMesh = Resources->internalLineMeshes.size() - 1;
			Resources->internalLineMeshes[sharedLineMesh].diffuseColor = DiffuseColor;
		}
		Resources->internalLineMeshes[sharedLineMesh].lines.push_back(Line);
	}
}