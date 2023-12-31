#pragma once

#include <string>

#include "common_types.h"
#include "resource_types.h"

namespace esl
{
	esl::uint CreateShaderProgram(std::string VertexShaderFilePath, std::string FragmentShaderFilePath);
	void DeleteShaderProgram(esl::uint ShaderProgram);

	esl::Shader GetShaderWithLocations(esl::uint ShaderProgram);
}