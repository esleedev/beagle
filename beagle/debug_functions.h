#pragma once

#include "resource_types.h"

namespace esl_debug
{
	void DrawLine(std::shared_ptr<esl::Resources> Resources, esl::Line Line, glm::vec4 DiffuseColor = glm::vec4(1, 1, 0, 1));
}