#include "ImageSourceChunkFactory.h"
#include "Chunk.h"
#include "IChunkManager.h"
#include "GreyScaleVoxel.h"


ImageSourceChunkFactory::ImageSourceChunkFactory(std::string sourceImageFilename, bool inverseHeight, bool inverseColor) : _image(sourceImageFilename.c_str()), _inverseHeight(inverseHeight), _inverseColor(inverseColor)
{
	
}

ImageSourceChunkFactory::~ImageSourceChunkFactory()
{
}

std::shared_ptr<IChunk> ImageSourceChunkFactory::construct(const IChunkManager* chunkManager, unsigned chunkX, unsigned chunkY, unsigned chunkZ)
{
	std::vector<std::shared_ptr<IVoxel>> voxels;
	unsigned rgbValues[IChunk::Width][IChunk::Height];

	for (unsigned z = 0; z < IChunk::Depth; z++)
	{
		unsigned worldZ = chunkZ * IChunk::Depth + z;
		for (unsigned x = 0; x < IChunk::Width; x++)
		{
			unsigned worldX = chunkX * IChunk::Width + x;
			if (worldX < (unsigned)_image.width() && worldZ < (unsigned)_image.height())
			{
				unsigned char r = _image(worldX, worldZ, 0, 0);
				rgbValues[x][z] = r;
			}
		}
	}

	for (unsigned z = 0; z < IChunk::Depth; z++)
	{
		for (unsigned y = 0; y < IChunk::Height; y++)
		{
			unsigned worldY = chunkY * IChunk::Height + y;

			for (unsigned x = 0; x < IChunk::Width; x++)
			{
				unsigned char r = (unsigned char)rgbValues[x][z];
				float fractionalR = (float)r / 255.0f;
				if (_inverseHeight) fractionalR = 1.0f - fractionalR;
				if (_inverseColor) r = 255 - r;
				unsigned colorWorldY = (unsigned)((chunkManager->getHeight() * IChunk::Height) * fractionalR);
				if (colorWorldY > worldY)
				{
					voxels.push_back(std::make_shared<GreyScaleVoxel>(r));
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
