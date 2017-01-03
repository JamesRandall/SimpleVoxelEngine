#pragma once
#include "IChunkFactory.h"
class PerlinNoise;

class PerlinNoiseChunkFactory : public IChunkFactory
{
	std::shared_ptr<PerlinNoise> _perlinNoise;
	int _octaves;
	double _persistence;
public:
	PerlinNoiseChunkFactory(unsigned int octaves, double persistence);
	PerlinNoiseChunkFactory(std::shared_ptr<PerlinNoise>& perlinNoise, unsigned int octaves, double persistence);
	virtual ~PerlinNoiseChunkFactory();

	std::shared_ptr<IChunk> construct(const IChunkManager* chunkManager, unsigned chunkX, unsigned chunkY, unsigned chunkZ) override;
};

