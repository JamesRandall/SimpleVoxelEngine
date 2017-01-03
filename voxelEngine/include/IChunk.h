#pragma once
#include <functional>
#include <memory>
#include <glad/glad.h>

class ICamera;
class IVoxel;
class IChunkManager;
class ILightSource;

typedef std::function<void(const std::shared_ptr<IVoxel>&, GLuint, GLuint, GLuint)> forEachVoxelFunction;
class IChunk
{
public:
	virtual ~IChunk() = default;
	virtual void forEachVoxel(const forEachVoxelFunction& func, bool includeNull = false) const = 0;
	virtual void render(const ICamera& camera, const ILightSource& light) const=0;
	virtual void prepareRenderer()=0;
	virtual void rebuildGeometry(const IChunkManager& chunkManager, unsigned int chunkX, unsigned int chunkY, unsigned int chunkZ) =0;
	virtual const std::shared_ptr<IVoxel>& getVoxel(unsigned int x, unsigned int y, unsigned int z) const = 0;
	virtual bool isConstructed() const = 0;
	virtual bool tick(float timeDelta) = 0;

	static const unsigned int Width = 16;
	static const unsigned int Height = 16;
	static const unsigned int Depth = 16;
};
