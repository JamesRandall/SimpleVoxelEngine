#include "VoxelEngine.h"
#include <glfw/glfw3.h>
#include "ShaderManager.h"
#include "IChunkFactory.h"
#include "ICameraControllerInput.h"
#include "ChunkManager.h"
#include "Camera.h"

VoxelEngine::VoxelEngine(GLFWwindow* window,
	std::string shaderPath,
	const worldSize& worldSize,
	const std::shared_ptr<IChunkFactory>& chunkFactory,
	const cameraConfiguration& cameraConfiguration,
	const std::shared_ptr<ICameraControllerInput>& cameraInputController,
	const std::shared_ptr<ILightSource>& lightSource) :
		_window(window),
		_chunkManager(std::make_shared<ChunkManager>(chunkFactory, worldSize.voxelsWide/IChunk::Width, worldSize.voxelsHigh/IChunk::Height, worldSize.voxelsDeep/IChunk::Depth)),
		_camera(std::make_shared<Camera>(cameraConfiguration.x, cameraConfiguration.y, cameraConfiguration.z, cameraConfiguration.horizontalAngle, cameraConfiguration.verticalAngle, cameraConfiguration.fieldOfView, cameraConfiguration.range)),
		_cameraInputController(cameraInputController),
		_lastTickTime(-1),
		_lightSource(lightSource)
{
	ShaderManager::init(shaderPath);
}

VoxelEngine::~VoxelEngine()
{
}

void VoxelEngine::tick()
{
	double frameCurrentTime = glfwGetTime();
	float timeDelta = _lastTickTime < 0 ? 0.0f : static_cast<float>(frameCurrentTime - _lastTickTime);
	_lastTickTime = frameCurrentTime;

	_cameraInputController->update(timeDelta);
	_camera->update(*_cameraInputController);
	_chunkManager->tick(timeDelta);
	_chunkManager->render(*_camera, *_lightSource);
}
