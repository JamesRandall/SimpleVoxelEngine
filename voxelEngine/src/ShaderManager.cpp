#include <iostream>
#include <fstream>
#include <vector>
#include <glad/glad.h>
#include "ShaderManager.h"

std::unique_ptr<ShaderManager> ShaderManager::instance = nullptr;

ShaderManager::ShaderManager(std::string shaderPath): _simpleColored(0)
{
	_shaderPath = shaderPath;
}


ShaderManager::~ShaderManager()
{
	glDeleteProgram(_simpleColored);
}

void ShaderManager::init(std::string shaderPath)
{
	if (instance != nullptr)
	{
		throw ShaderManagerInitialisedException();
	}
	instance = std::unique_ptr<ShaderManager>(new ShaderManager(shaderPath));
	instance->_simpleColored = instance->loadShaders("simpleLightTransform.vertexshader", "simpleLightColor.fragmentshader");
}

GLuint ShaderManager::getSimpleColored() const
{
	return _simpleColored;
}


GLuint ShaderManager::loadShaders(const std::string vertexPath, const std::string fragmentPath) const
{
	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string fullVertexPath = _shaderPath + vertexPath;
	std::string fullFragmentPath = _shaderPath + fragmentPath;

	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(fullVertexPath, std::ios::in);
	if (vertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(vertexShaderStream, Line))
			vertexShaderCode += "\n" + Line;
		vertexShaderStream.close();
	}
	else {
		std::cout << "Impossible to open " << fullVertexPath << ".\n";
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fullFragmentPath, std::ios::in);
	if (fragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(fragmentShaderStream, Line))
			fragmentShaderCode += "\n" + Line;
		fragmentShaderStream.close();
	}
	else {
		std::cout << "Impossible to open " << fullFragmentPath << ".\n";
		getchar();
		return 0;
	}

	GLint result = GL_FALSE;
	int infoLogLength;


	// Compile Vertex Shader
	std::cout << "Compiling shader :" << vertexPath << "\n";
	//printf("Compiling shader : %s\n", vertexPath);
	char const * vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	std::cout << "Compiling shader :" << fragmentPath << "\n";
	char const * fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderID);
	glAttachShader(programId, fragmentShaderID);
	glLinkProgram(programId);

	// Check the program
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programId, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
	}

	glDetachShader(programId, vertexShaderID);
	glDetachShader(programId, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programId;

}
