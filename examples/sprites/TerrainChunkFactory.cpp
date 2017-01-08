#include "TerrainChunkFactory.h"
#include <vector>
#include "TerrainVoxel.h"
#include "Chunk.h"
#include "IChunkManager.h"

TerrainChunkFactory::TerrainChunkFactory()
{
}


TerrainChunkFactory::~TerrainChunkFactory()
{
}

std::shared_ptr<IChunk> TerrainChunkFactory::construct(const IChunkManager* chunkManager, unsigned chunkX, unsigned chunkY, unsigned chunkZ)
{
	std::vector<std::shared_ptr<IVoxel>> voxels;
	for (unsigned z = 0; z < IChunk::Depth; z++)
	{
		unsigned worldZ = chunkZ * IChunk::Depth + z;
		for (unsigned y = 0; y < IChunk::Height; y++)
		{
			unsigned worldY = chunkY * IChunk::Height + y;

			for (unsigned x = 0; x < IChunk::Width; x++)
			{
				unsigned worldX = chunkX * IChunk::Width + x;
				if (worldY == 0)
				{
					// in theory in this instance we could reuse the voxel however if we want to deform the terrain or
					// take other discrete world effecting actions later we can't do this.
					// there are optimisations planned to allow this kind of virtualisation within the engine.
					std::shared_ptr<IVoxel> voxel = std::make_shared<TerrainVoxel>((x == 0 || z == 0) && !(worldX < IChunk::Width || worldZ < IChunk::Depth) && !(worldX > (chunkManager->getContainerWidthInVoxels() - IChunk::Width) || worldZ > (chunkManager->getContainerDepthInVoxels() - IChunk::Depth)));
					voxels.push_back(voxel);
				}
				else
				{
					voxels.push_back(nullptr);
				}
			}
		}
	}

	std::shared_ptr<IChunk> chunk = std::make_shared<Chunk>(voxels);
	return chunk;
}
