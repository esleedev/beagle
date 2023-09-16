#pragma once

#include <string>
#include <gl/glew.h>

std::string GetSourceStringFromFile(std::string FilePath);

GLuint CreateShaderProgram(std::string VertexShaderFilePath, std::string FragmentShaderFilePath);
void DeleteShaderProgram(GLuint ShaderProgram);
