#include "shader.h"
#include <SDL_opengl.h>
#include <fstream>
#include <iostream>

std::string GetSourceStringFromFile(std::string FilePath)
{
	std::string line, lines;
	std::ifstream file;
	file.open(FilePath);
	if (file.is_open())
	{
		std::cout << "Opened file: " << FilePath << std::endl;
		while (std::getline(file, line))
		{
			lines += line + "\n";
		}
		file.close();
		std::cout << "Source loaded:\n" << lines << std::endl;
	}
	else
	{
		std::cout << "File not found: " << FilePath << std::endl;
	}

	return lines;
}

GLuint CreateShaderProgram(std::string VertexShaderFilePath, std::string FragmentShaderFilePath)
{
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderSourceString = GetSourceStringFromFile(VertexShaderFilePath);
	const GLchar* vertexShaderSource = vertexShaderSourceString.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	GLint hasShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &hasShaderCompiled);
	if (hasShaderCompiled != GL_TRUE)
	{
		std::cout << "Vertex shader failed to compile" << std::endl;
	}

	glAttachShader(shaderProgram, vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentShaderSourceString = GetSourceStringFromFile(FragmentShaderFilePath);
	const GLchar* fragmentShaderSource = fragmentShaderSourceString.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	hasShaderCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &hasShaderCompiled);
	if (hasShaderCompiled != GL_TRUE)
	{
		std::cout << "Fragment shader failed to compile" << std::endl;
	}

	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	GLint hasLinked = GL_FALSE;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &hasLinked);
	if (hasLinked != GL_TRUE)
	{
		std::cout << "Shader program failed to link" << std::endl;
	}

	// free to detach and delete after linking
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	std::cout << "Shader program created" << std::endl;

	return shaderProgram;
}

void DeleteShaderProgram(GLuint ShaderProgram)
{
	glDeleteProgram(ShaderProgram);
}

Shader GetShaderWithLocations(GLuint ShaderProgram)
{
	Shader shader;
	shader.shaderProgram = ShaderProgram;
	shader.objectMatrixUniform = glGetUniformLocation(ShaderProgram, "objectMatrix");
	shader.projectionMatrixUniform = glGetUniformLocation(ShaderProgram, "projectionMatrix");
	shader.viewMatrixUniform = glGetUniformLocation(ShaderProgram, "viewMatrix");
	shader.textureUniform = glGetUniformLocation(ShaderProgram, "texture");
	return shader;
}