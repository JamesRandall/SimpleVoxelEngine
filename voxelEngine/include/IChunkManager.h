#pragma once
#include <memory>
#include <functional>
class ICamera;
class IChunk;
class ILightSource;
class IVoxel;

typedef std::function<void(const std::shared_ptr<IChunk>&, unsigned int, unsigned int, unsigned int)> forEachChunkFunction;
class IChunkManager
{
public:
	virtual ~IChunkManager() = default;
	virtual const std::shared_ptr<IChunk>& getChunkWithChunkCoordinates(unsigned int x, unsigned int y, unsigned int z) const = 0;
	virtual const std::shared_ptr<IChunk>& getChunkWithVoxelCoordinates(unsigned int x, unsigned int y, unsigned int z) const = 0;
	virtual void forEachChunk(const forEachChunkFunction& func) const = 0;
	virtual const std::shared_ptr<IVoxel>& getVoxel(unsigned int x, unsigned int y, unsigned int z) const = 0;
	virtual unsigned int getWidth() const = 0;
	virtual unsigned int getHeight() const = 0;
	virtual unsigned int getDepth() const = 0;
	virtual void render(const ICamera& camera, const ILightSource& light) const = 0;
	virtual bool tick(float timeDelta) = 0;
	virtual bool chunkInCamera(const ICamera& camera, unsigned x, unsigned y, unsigned z) const = 0;
};

