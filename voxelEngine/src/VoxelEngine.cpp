#include "VoxelEngine.h"
#include <glfw/glfw3.h>
#include "ShaderManager.h"
#include "IChunkFactory.h"
#include "ICameraControllerInput.h"
#include "ChunkManager.h"
#include "Camera.h"
#include "SpriteManager.h"
#include "FrameCounter.h"
#include "NullCameraController.h"

VoxelEngine::VoxelEngine(GLFWwindow* window,
	std::string shaderPath,
	const worldSize& worldSize,
	const std::shared_ptr<IChunkFactory>& chunkFactory,
	const cameraConfiguration& cameraConfiguration,
	const std::shared_ptr<ICameraControllerInput>& cameraInputController,
	const std::shared_ptr<ILightSource>& lightSource) :
		_window(window),
		_chunkManager(chunkFactory == nullptr ? nullptr : std::make_shared<ChunkManager>(chunkFactory, worldSize.voxelsWide/IChunk::Width, worldSize.voxelsHigh/IChunk::Height, worldSize.voxelsDeep/IChunk::Depth)),
		_camera(std::make_shared<Camera>(cameraConfiguration.x, cameraConfiguration.y, cameraConfiguration.z, cameraConfiguration.horizontalAngle, cameraConfiguration.verticalAngle, cameraConfiguration.fieldOfView, cameraConfiguration.range, cameraConfiguration.ratio)),
		_cameraInputController(cameraInputController != nullptr ? cameraInputController : std::make_shared<NullCameraController>()),
		_lightSource(lightSource),
		_spriteManager(std::make_shared<SpriteManager>()),
		_lastTickTime(-1)
{
	ShaderManager::init(shaderPath);
}

VoxelEngine::~VoxelEngine()
{
}

float VoxelEngine::tick(tickFunction updateCompleteFunc, tickFunction tickCompleteFunc)
{
	static FrameCounter frameCounter;
	frameCounter.tick();

	double frameCurrentTime = glfwGetTime();
	float timeDelta = _lastTickTime < 0 ? 0.0f : static_cast<float>(frameCurrentTime - _lastTickTime);
	
	_cameraInputController->update(timeDelta);
	_camera->update(*_cameraInputController);
	
	if (_chunkManager != nullptr)
	{
		_chunkManager->tick(timeDelta);		
	}
	if (_spriteManager != nullptr)
	{
		_spriteManager->tick(timeDelta);
	}

	if (updateCompleteFunc != nullptr)
	{
		updateCompleteFunc(timeDelta);
	}

	if (_chunkManager != nullptr)
	{
		_chunkManager->render(*_camera, *_lightSource);
	}
	if (_spriteManager != nullptr)
	{
		_spriteManager->render(*_camera, *_lightSource);
	}

	if (_lastTickTime < 0)
	{
		_lastTickTime = glfwGetTime();
	}
	else
	{
		_lastTickTime = frameCurrentTime;
	}

	_spriteManager->tickComplete(timeDelta);

	if (tickCompleteFunc != nullptr)
	{
		tickCompleteFunc(timeDelta);
	}

	return timeDelta;
}

std::shared_ptr<ISpriteManager> VoxelEngine::getSpriteManager() const
{
	return _spriteManager;
}
