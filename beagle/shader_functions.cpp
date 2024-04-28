#include <gl/glew.h>

#include <fstream>
#include <iostream>

#include "shader_functions.h"
#include "file_functions.h"

esl::uint esl::CreateShaderProgram(std::string VertexShaderFilePath, std::string FragmentShaderFilePath)
{
	esl::uint shaderProgram = glCreateProgram();

	// load vertex shader
	esl::uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderSource = esl::GetFileSource(VertexShaderFilePath);
	const char* vertexShaderCString = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderCString, nullptr);
	
	// compile vertex shader
	glCompileShader(vertexShader);

	int hasShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &hasShaderCompiled);
	if (hasShaderCompiled != GL_TRUE)
	{
		std::cout << "Vertex shader failed to compile" << std::endl;
	}

	// load fragment shader
	esl::uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentShaderSource = esl::GetFileSource(FragmentShaderFilePath);
	const char* fragmentShaderCString = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderCString, nullptr);
	
	// compile fragment shader
	glCompileShader(fragmentShader);

	hasShaderCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &hasShaderCompiled);
	if (hasShaderCompiled != GL_TRUE)
	{
		std::cout << "Fragment shader failed to compile" << std::endl;
	}

	// attach vertex and fragment shaders to program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// link shaders
	glLinkProgram(shaderProgram);

	int hasLinked = GL_FALSE;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &hasLinked);
	if (hasLinked != GL_TRUE)
	{
		std::cout << "Shader program failed to link" << std::endl;
	}

	// free to detach and delete vertex and fragment shaders after linking
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void esl::DeleteShaderProgram(esl::uint ShaderProgram)
{
	glDeleteProgram(ShaderProgram);
}

esl::Shader esl::GetShaderWithLocations(esl::uint ShaderProgram)
{
	esl::Shader shader;
	shader.program = ShaderProgram;
	shader.objectMatrixUniform = glGetUniformLocation(ShaderProgram, "objectMatrix");
	shader.projectionMatrixUniform = glGetUniformLocation(ShaderProgram, "projectionMatrix");
	shader.viewMatrixUniform = glGetUniformLocation(ShaderProgram, "viewMatrix");
	shader.textureUniform = glGetUniformLocation(ShaderProgram, "texture");
	shader.diffuseColorUniform = glGetUniformLocation(ShaderProgram, "diffuseColor");
	return shader;
}
