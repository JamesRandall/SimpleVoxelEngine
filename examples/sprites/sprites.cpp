#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "VoxelEngine.h"
#include "MouseAndKeyboardCameraControllerInput.h"
#include "SimpleLight.h"

#include <glm/glm.hpp>
#include <stdio.h>
#include "ISprite.h"
#include "TerrainChunkFactory.h"
#include "BehaviourFactory.h"
#include <random>

bool setupEnvironment(GLFWwindow*& window);
static void error_callback(int error, const char* description);

int main(int argc, char** argv) {
	std::string programPath(argv[0]);
	size_t lastSlash = programPath.find_last_of('\\');
	std::string shaderPath = "shaders/";
	std::string spritePath = "";

	GLFWwindow* window;
	if (setupEnvironment(window)) return -1;

	const unsigned demoWidth = 16;
	const unsigned demoDepth = 16;

	std::shared_ptr<IChunkFactory> chunkFactory = std::make_shared<TerrainChunkFactory>();
	std::shared_ptr<ICameraControllerInput> cameraInputController = std::make_shared<MouseAndKeyboardCameraControllerInput>(window, 0.005, 32.0f);
	std::shared_ptr<ILightSource> light = std::make_shared<SimpleLight>(lightSourcePosition(3.0f * IChunk::Width, 120.0f, 4.0f * IChunk::Depth), color(0.7f, 0.7f, 0.7f), 15000.0f);
	std::shared_ptr<VoxelEngine> voxelEngine = std::make_shared<VoxelEngine>(window,
		shaderPath,
		worldSize(demoWidth * IChunk::Width, 1 * IChunk::Height, demoDepth * IChunk::Depth),
		chunkFactory,
		cameraConfiguration(demoWidth / 2 * IChunk::Width, 60, demoDepth / 2 * IChunk::Depth, 180.0f, 270, 60.0f, 800.0f, 1920.0f/1080.0f),
		cameraInputController,
		light);

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1, 100);
	auto dice = std::bind(distribution, generator);
	const float deferralGap = 0.5f;
	float currentDeferral = 0.0f;
	for (unsigned x = 1; x < demoWidth-1; x++)
	{
		for (unsigned z = 1; z < demoDepth-1; z++)
		{
			std::shared_ptr<ISprite> sprite;
			int diceRoll = dice();
			if (diceRoll > 90)
			{
				sprite = ISprite::load(spritePath + "supermarket.vox");
			}
			else if (diceRoll > 80)
			{
				sprite = ISprite::load(spritePath + "highTowerBlock.vox");
			}
			else if (diceRoll > 40)
			{
				sprite = ISprite::load(spritePath + "towerBlock.vox");
			}
			else
			{
				sprite = ISprite::load(spritePath + "fourHouses.vox");
			}
			float deferral = 0.5f + dice() / 5.0f;
			voxelEngine->getSpriteManager()->addSprite(sprite);
			voxelEngine->getSpriteManager()->attachSpriteBehaviour(sprite, BehaviourFactory::GetVoxelatedConstructionBehaviour(dice() > 50, 0.012f), deferral);
			sprite->setPosition(spriteVec3(1 + x * IChunk::Width, 1, 1 + z * IChunk::Depth));

			currentDeferral += deferralGap;
		}
	}

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

	window = glfwCreateWindow(1920, 1080, "City Block Construction", nullptr, nullptr);
	if (window == nullptr) {
		fprintf(stderr, "Failed to open GLFW window. If you have an older Intel GPU, they are not 3.3 compatible. Nor is a VM running in Parallels for Mac.\n");
		glfwTerminate();
		return true;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
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