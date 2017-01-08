#pragma once
#include <memory>
#include "worldSize.h"
#include "ICamera.h"
#include "ICameraControllerInput.h"
#include "IChunk.h"
#include "IChunkFactory.h"
#include "IChunkManager.h"
#include "ILightSource.h"
#include "ISprite.h"
#include "ISpriteManager.h"

struct GLFWwindow;

class SpriteManager;

class VoxelEngine
{
	GLFWwindow *_window;
	std::shared_ptr<IChunkManager> _chunkManager;
	std::shared_ptr<ICamera> _camera;
	std::shared_ptr<ICameraControllerInput> _cameraInputController;
	std::shared_ptr<ILightSource> _lightSource;
	std::shared_ptr<SpriteManager> _spriteManager;
	double _lastTickTime;
public:
	VoxelEngine(GLFWwindow* window,
		std::string shaderPath,
		const worldSize& worldSize,
		const std::shared_ptr<IChunkFactory>& chunkFactory,
		const cameraConfiguration& cameraConfiguration,
		const std::shared_ptr<ICameraControllerInput>& cameraInputController,
		const std::shared_ptr<ILightSource>& lightSource);
	~VoxelEngine();
	void tick();
	worldSize getWorldSize() const { return worldSize(_chunkManager->getWidth() * IChunk::Width, _chunkManager->getHeight() * IChunk::Height, _chunkManager->getDepth() * IChunk::Depth); }
	void addSprite(const std::shared_ptr<ISprite>& sprite) const;
	std::shared_ptr<ISpriteManager> getSpriteManager() const;
};

