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
					std::shared_ptr<IVoxel> voxel = std::make_shared<TerrainVoxel>((x == 0 && chunkX > 0) || (z == 0 && chunkZ > 0));
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
