#pragma once
#include <memory>
#include "IChunk.h"

class IChunkManager;

class IChunkFactory
{
protected:
	~IChunkFactory() = default;
public:
	virtual std::shared_ptr<IChunk> construct(const IChunkManager* chunkManager, unsigned int chunkX, unsigned int chunkY, unsigned int chunkZ) = 0;
};

