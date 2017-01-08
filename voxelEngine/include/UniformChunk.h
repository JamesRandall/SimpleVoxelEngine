#pragma once
#include "IChunk.h"
#include "IVoxel.h"

class VoxelContainerGeometry;
class VoxelRenderer;
class ILightSource;

class UniformChunk : public IChunk
{
	std::shared_ptr<VoxelContainerGeometry> _chunkGeometry;
	std::shared_ptr<VoxelRenderer> _chunkRenderer;
	std::shared_ptr<IVoxel> _voxel;

public:
	UniformChunk(const std::shared_ptr<IVoxel>& voxel);
	~UniformChunk();

	bool tick(float timeDelta) override;
	void forEachVoxel(const forEachVoxelFunction& func, bool includeNull) const override;
	void render(const ICamera& camera, const ILightSource& light) const override;
	void rebuildGeometry(const IChunkManager& chunkManager, unsigned chunkX, unsigned chunkY, unsigned chunkZ) override;
	void prepareRenderer() override;
	std::shared_ptr<IVoxel> getVoxel(unsigned x, unsigned y, unsigned z) const override;
	bool isConstructed() const override;
};

