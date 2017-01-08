#pragma once
#include "IChunkFactory.h"

class TerrainChunkFactory : public IChunkFactory
{
public:
	TerrainChunkFactory();
	virtual ~TerrainChunkFactory();

	std::shared_ptr<IChunk> construct(const IChunkManager* chunkManager, unsigned chunkX, unsigned chunkY, unsigned chunkZ) override;
};

