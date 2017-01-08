#include "UniformChunk.h"
#include "VoxelRenderer.h"
#include "IChunkManager.h"

UniformChunk::UniformChunk(const std::shared_ptr<IVoxel>& voxel) : _voxel(voxel)
{
	
}


UniformChunk::~UniformChunk()
{
}

bool UniformChunk::tick(float timeDelta)
{
	return false;
}

void UniformChunk::forEachVoxel(const forEachVoxelFunction& func, bool includeNull) const
{
	unsigned int vectorIndex = 0;
	for (int z = 0; z < Depth; z++)
	{
		for (int y = 0; y < Height; y++)
		{
			for (int x = 0; x < Width; x++)
			{
				func(_voxel, x, y, z);
			}
		}
	}
}

void UniformChunk::render(const ICamera& camera, const ILightSource& light) const
{
	if (_chunkRenderer == nullptr) return;
	_chunkRenderer->render( camera, *_chunkGeometry, light);
}

void UniformChunk::rebuildGeometry(const IChunkManager& chunkManager, unsigned chunkX, unsigned chunkY, unsigned chunkZ)
{
	_chunkGeometry = std::make_shared<VoxelContainerGeometry>(chunkManager, *this, chunkX, chunkY, chunkZ);
	
}

void UniformChunk::prepareRenderer()
{
	if (_chunkGeometry == nullptr) return;
	_chunkRenderer = std::make_shared<VoxelRenderer>();
	_chunkRenderer->buildBuffers(*_chunkGeometry);
}

std::shared_ptr<IVoxel> UniformChunk::getVoxel(unsigned x, unsigned y, unsigned z) const
{
	return _voxel;
}

bool UniformChunk::isConstructed() const
{
	return true;
}
