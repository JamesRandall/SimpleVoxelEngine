#pragma once
#include <vector>
#include <memory>
#include "IChunkManager.h"
#include "IChunk.h"

class IChunkFactory;
class ShaderManager;
class VoxelGeometry;

class ChunkManager : public IChunkManager
{
private:
	class ChunkRebuildInstruction
	{
		public:
			ChunkRebuildInstruction(const std::shared_ptr<IChunk>& chunk, unsigned int x, unsigned int y, unsigned int z) { this->chunk = chunk; this->x = x; this->y = y; this->z = z; }

			std::shared_ptr<IChunk> chunk;
			unsigned int x;
			unsigned int y;
			unsigned int z;
	};

	unsigned int _width;
	unsigned int _height;
	unsigned int _depth;
	std::vector<std::shared_ptr<IChunk>> _chunks;
	std::vector<std::shared_ptr<ChunkRebuildInstruction>> _chunksRequiringRebuild;
	std::shared_ptr<IChunkFactory> _chunkFactory;
	
	unsigned int getChunkIndex(unsigned int x, unsigned int y, unsigned int z) const { return z * _height * _width  + y * _width + x; }
	void rebuildGeometry();
	void constructChunks(const std::vector<std::shared_ptr<ChunkManager::ChunkRebuildInstruction>>& chunksRequiringConstruction);

public:
	ChunkManager(const std::shared_ptr<IChunkFactory>& chunkFactory, unsigned int chunksAcross, unsigned int chunksUp, unsigned int chunksDeep);
	~ChunkManager();

	const std::shared_ptr<IChunk>& getChunkWithChunkCoordinates(unsigned int x, unsigned int y, unsigned int z) const override;
	const std::shared_ptr<IChunk>& getChunkWithVoxelCoordinates(unsigned int x, unsigned int y, unsigned int z) const override;
	bool tick(float timeDelta) override;
	void forEachChunk(const forEachChunkFunction& func) const override;
	void forEachVoxel(const forEachVoxelFunction& func, bool includeNull = false) const override;
	unsigned int getWidth() const override { return _width; }
	unsigned int getHeight() const override { return _height; }
	unsigned int getDepth() const override { return _depth; }
	unsigned int getContainerWidthInVoxels() const override { return _width * IChunk::Width; }
	unsigned int getContainerHeightInVoxels() const override { return _height * IChunk::Height; }
	unsigned int getContainerDepthInVoxels() const override { return _depth * IChunk::Depth; }
	void render(const ICamera& camera, const ILightSource& light) const override;
	std::shared_ptr<IVoxel> getVoxel(unsigned int x, unsigned int y, unsigned int z) const override;
	bool chunkInCamera(const ICamera& camera, unsigned x, unsigned y, unsigned z) const override;	
};

