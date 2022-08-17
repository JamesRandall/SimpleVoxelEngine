#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "VoxelEngine.h"
#include "MouseAndKeyboardCameraControllerInput.h"
#include "SimpleLight.h"
#include <glm/glm.hpp>
#include <stdio.h>
#include "ISprite.h"
#include "TerrainChunkFactory.h"
#include <random>
#include "InvaderMovementSpriteBehaviour.h"
#include "PlayerMovementBehaviour.h"
#include "BulletMovementSpriteBehaviour.h"

const unsigned bulletSpriteType = 1;
const unsigned invaderSpriteType = 2;

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
	const unsigned demoDepth = 24;
	const unsigned invadersAcross = 8;
	const unsigned invaderSpacing = 6;
	const unsigned invaderWidth = 11;
	const unsigned invaderDepth = 128;
	const unsigned invaderMovementRange = 32;
	const float minTimeBetweenBullets = 0.75f;
	const float invaderSpeed = 0.1f;
	unsigned midpoint = demoWidth * IChunk::Width / 2;

	std::shared_ptr<IChunkFactory> chunkFactory = std::make_shared<TerrainChunkFactory>();
	std::shared_ptr<ILightSource> light = std::make_shared<SimpleLight>(lightSourcePosition(midpoint, 120.0f, invaderDepth/2), color(0.7f, 0.7f, 0.7f), 15000.0f);
	std::shared_ptr<VoxelEngine> voxelEngine = std::make_shared<VoxelEngine>(window,
		shaderPath,
		worldSize(demoWidth * IChunk::Width, 1 * IChunk::Height, demoDepth * IChunk::Depth),
		chunkFactory,
		cameraConfiguration(midpoint, 60, -16.0f, 0.0f, 0.1f, 60.0f, 800.0f, 1920.0f/1080.0f),
		nullptr,
		light);
	const std::shared_ptr<ISpriteManager>& spriteManager = voxelEngine->getSpriteManager();
	std::vector<std::shared_ptr<ISprite>> invaders;
	const unsigned baseX = midpoint - (invaderWidth + invaderSpacing)*invadersAcross / 2;
	
	for (unsigned invaderX = 0; invaderX < invadersAcross; invaderX++)
	{
		for (unsigned invaderY = 0; invaderY < 6; invaderY++)
		{
			std::shared_ptr<ISprite> invader;
			if ((invaderY % 2) ==0)
			{
				std::string frame0 = spritePath + "invader_f0.vox";
				std::string frame1 = spritePath + "invader_f1.vox";
				invader = ISprite::load(std::vector<std::string> { frame0, frame1 }, 0.5f, 0.0f, invaderSpriteType);
			}
			else
			{
				std::string frame0 = spritePath + "invader2_f0.vox";
				std::string frame1 = spritePath + "invader2_f1.vox";
				invader = ISprite::load(std::vector<std::string> { frame0, frame1 }, 0.5f, 0.25f, invaderSpriteType);				
			}
			
			invader->setPosition(spriteVec3(baseX + invaderX * (invader->getSize().width + invaderSpacing) - invaderMovementRange/2, 80 + invaderY * (invader->getSize().height + 4), invaderDepth));
			spriteManager->addSprite(invader);
			invaders.push_back(invader);
			spriteManager->attachSpriteBehaviour(invader, std::make_shared<InvaderMovementSpriteBehaviour>(invaderSpeed, invaderMovementRange));
			spriteManager->addSpriteToCollisionSet(invader);
		}
	}

	std::shared_ptr<ISprite> player = ISprite::load(spritePath + "player.vox");
	player->setPosition(spriteVec3(midpoint, 2, invaderDepth));
	spriteManager->addSprite(player);
	std::shared_ptr<PlayerMovementBehaviour> playerBehaviour = std::make_shared<PlayerMovementBehaviour>(0.025f);
	spriteManager->attachSpriteBehaviour(player, playerBehaviour);
		
	float timeSinceLastBullet = minTimeBetweenBullets;
	std::forward_list<std::shared_ptr<ISprite>> bullets;

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		playerBehaviour->setIsLeftDown(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
		playerBehaviour->setIsRightDown(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
		
		voxelEngine->tick([&](float timeDelta)
		{
			auto collisions = spriteManager->findCollisionsForSprites(bullets);
			if (!collisions->empty())
			{
				std::for_each(collisions->begin(), collisions->end(), [&spriteManager, &bullets](const SpriteCollision& collision)
				{
					spriteManager->removeSprite(collision.getSpriteOne());
					spriteManager->removeSprite(collision.getSpriteTwo());

					const std::shared_ptr<ISprite>& bulletSprite = collision.getSpriteOne()->getType() == bulletSpriteType ? collision.getSpriteOne() : collision.getSpriteTwo();
					bullets.remove(bulletSprite);
				});
			}

			timeSinceLastBullet += timeDelta;
			if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && timeSinceLastBullet >= minTimeBetweenBullets)
			{
				std::shared_ptr<ISprite> bullet = ISprite::load(spritePath + "playerBullet.vox", bulletSpriteType);
				bullet->setPosition(spriteVec3(player->getPosition().x + 5, 3, invaderDepth));
				spriteManager->addSprite(bullet);
				bullet->attachSpriteBehaviour(*spriteManager, std::make_shared<BulletMovementSpriteBehaviour>(0.005f));
				bullet->setVelocity(glm::vec3(0, 120.0, 0.0));				
				spriteManager->addSpriteToCollisionSet(bullet);
				timeSinceLastBullet = 0.0f;
				bullets.push_front(bullet);
			}
		});
		
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