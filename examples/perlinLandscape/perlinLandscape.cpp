#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "VoxelEngine.h"
#include "PerlinNoiseChunkFactory.h"
#include "MouseAndKeyboardCameraControllerInput.h"
#include "SimpleLight.h"

#include <glm/glm.hpp>
#include <stdio.h>

bool setupEnvironment(GLFWwindow*& window);
static void error_callback(int error, const char* description);

int main(int argc, char** argv) {
	std::string programPath(argv[0]);
	size_t lastSlash = programPath.find_last_of('\\');
	std::string shaderPath = "shaders/";

	GLFWwindow* window;
	if (setupEnvironment(window)) return -1;

	// Configure the voxel engine to display voxels based on a perlin noise algorithm and connect the camera to the mouse and keyboard
	std::shared_ptr<IChunkFactory> chunkFactory = std::make_shared<PerlinNoiseChunkFactory>(4, 3); // 6,3 will give a much spikier terrain
	std::shared_ptr<ICameraControllerInput> cameraInputController = std::make_shared<MouseAndKeyboardCameraControllerInput>(window);
	std::shared_ptr<ILightSource> light = std::make_shared<SimpleLight>(lightSourcePosition(6.0f * IChunk::Width, 200.0f, 6.0f * IChunk::Depth), color(0.7f, 0.7f, 0.7f), 30000.0f);
	std::shared_ptr<VoxelEngine> voxelEngine = std::make_shared<VoxelEngine>(window,
		shaderPath,
		worldSize(18 * IChunk::Width, 2 * IChunk::Height, 18 * IChunk::Depth),
		chunkFactory,
		cameraConfiguration(5*IChunk::Width, 60.0f, 5 * IChunk::Depth, 45.0f, -1.0f, 60.0f, 800.0f),
		cameraInputController,
		light);

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		voxelEngine->tick();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glfwTerminate();
}

bool setupEnvironment(GLFWwindow*& window)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return true;
	}

	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	window = glfwCreateWindow(1024, 768, "Perlin Noise Landscape", nullptr, nullptr);
	if (window == nullptr) {
		fprintf(stderr, "Failed to open GLFW window. If you have an older Intel GPU, they are not 3.3 compatible. Nor is a VM running in Parallels for Mac.\n");
		glfwTerminate();
		return true;
	}
	glfwMakeContextCurrent(window);
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	return false;
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}