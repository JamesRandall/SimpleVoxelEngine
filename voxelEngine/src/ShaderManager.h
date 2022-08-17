#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include <string>
#include <stdexcept>

class ShaderManager
{
private:
	std::string _shaderPath;
	GLuint _simpleColored;
	GLuint loadShaders(const std::string vertexPath, const std::string fragmentPath) const;
	ShaderManager(std::string shaderPath);
public:	
	static void init(std::string shaderPath);
	static std::unique_ptr<ShaderManager> instance;
	
	~ShaderManager();
	
	GLuint getSimpleColored() const;	
};

class ShaderManagerInitialisedException : public std::logic_error
{
public:
	ShaderManagerInitialisedException() : std::logic_error("The shader manager has already been initialised") { }
};

typedef std::shared_ptr<ShaderManager> ShaderManagerPtr;
