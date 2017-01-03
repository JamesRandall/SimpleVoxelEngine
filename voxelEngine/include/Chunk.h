#pragma once
#include <vector>
#include <memory>
#include "IChunk.h"
#include "IVoxel.h"

class ChunkGeometry;
class ChunkRenderer;

class Chunk : public IChunk
{
private:
	
	std::vector<std::shared_ptr<IVoxel>> _voxels;
	std::shared_ptr<ChunkGeometry> _chunkGeometry;
	std::shared_ptr<ChunkRenderer> _chunkRenderer;

	static unsigned int getVoxelIndex(unsigned int x, unsigned int y, unsigned int z) { return z * IChunk::Height * IChunk::Width + y * IChunk::Width + x; }

public:	
	Chunk(const std::vector<std::shared_ptr<IVoxel>>& voxels);
	~Chunk();

	void prepareRenderer() override;
	void rebuildGeometry(const IChunkManager& chunkManager, unsigned int chunkX, unsigned int chunkY, unsigned int chunkZ) override;
	void forEachVoxel(const forEachVoxelFunction& func, bool includeNull=false) const override;
	void render(const ICamera& camera, const ILightSource& light) const override;
	const std::shared_ptr<IVoxel>& getVoxel(unsigned int x, unsigned int y, unsigned int z) const override;
	bool isConstructed() const override { return true; }
	bool tick(float timeDelta) override { return false; }
};

typedef std::shared_ptr<Chunk> ChunkPtr;
typedef std::weak_ptr<Chunk> ChunkWeakPtr;