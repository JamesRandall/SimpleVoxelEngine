#pragma once
#include "VoxelEngineException.h"
#include "IChunk.h"

struct worldSize {
	unsigned voxelsWide;
	unsigned voxelsHigh;
	unsigned voxelsDeep;

	worldSize(unsigned vw, unsigned vh, unsigned vd) : voxelsWide(vw), voxelsHigh(vh), voxelsDeep(vd)
	{
		if (vw % IChunk::Width > 0 || vh % IChunk::Height > 0 || vd % IChunk::Depth > 0)
		{
			throw VoxelEngineException("Width, height and depth must be a multiple of IChunk::Width, IChunk::Height and IChunk::Depth respectively");
		}
	}
};