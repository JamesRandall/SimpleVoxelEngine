#include "Chunk.h"
#include "ChunkGeometry.h"
#include "VoxelGeometry.h"
#include "ChunkRenderer.h"

Chunk::Chunk(const std::vector<std::shared_ptr<IVoxel>>& voxels)
{
	_voxels = voxels;
}

Chunk::~Chunk()
{
}

const std::shared_ptr<IVoxel>& Chunk::getVoxel(unsigned int x, unsigned int y, unsigned int z) const
{
	unsigned int index = getVoxelIndex(x, y, z);
	return _voxels[index];
}

void Chunk::forEachVoxel(const forEachVoxelFunction& func, bool includeNull) const
{
	unsigned int vectorIndex = 0;
	for (int z = 0; z < Depth; z++)
	{
		for (int y = 0; y < Height; y++)
		{
			for (int x = 0; x < Width; x++)
			{
				const std::shared_ptr<IVoxel>& voxel = _voxels[vectorIndex];
				vectorIndex++;
				if (voxel != nullptr || includeNull)
				{
					func(voxel, x, y, z);
				}				
			}
		}
	}
}

void Chunk::rebuildGeometry(const IChunkManager& chunkManager, unsigned int chunkX, unsigned int chunkY, unsigned int chunkZ)
{
	_chunkGeometry = std::make_shared<ChunkGeometry>(chunkManager, *this, chunkX, chunkY, chunkZ);	
}

void Chunk::prepareRenderer()
{
	if (_chunkGeometry == nullptr) return;
	_chunkRenderer = std::make_shared<ChunkRenderer>();
	_chunkRenderer->buildBuffers(*_chunkGeometry);
}

void Chunk::render(const ICamera& camera, const ILightSource& light) const
{
	if (_chunkRenderer == nullptr) return;
	_chunkRenderer->render(camera, *_chunkGeometry, light);
}
