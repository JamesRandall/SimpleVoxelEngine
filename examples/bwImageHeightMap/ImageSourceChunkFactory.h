#pragma once
#include <algorithm>
#include <vector>
#include "IChunkFactory.h"
#include "IChunk.h"
#pragma warning  (push)
#pragma warning (disable : 4319 4083 4018)
#include "CImg.h"
#pragma warning  (pop)

#include "GreyScaleVoxel.h"

class IChunkManager;

class ImageSourceChunkFactory : public IChunkFactory
{
	cimg_library::CImg<unsigned char> _image;
	bool _inverseHeight;
	bool _inverseColor;
	bool _isBlackAndWhite;
public:
	ImageSourceChunkFactory(std::string sourceImageFilename, bool inverseHeight=false, bool inverseColor=false, bool isBlackAndWhite=false);
	virtual ~ImageSourceChunkFactory();
	
	unsigned int requiredWidth() const { return _image.width() + ((_image.width() % IChunk::Width) > 0 ? (16 - (_image.width() % IChunk::Width)) : 0); }
	unsigned int requiredDepth() const { return _image.height() + ((_image.height() % IChunk::Depth) > 0 ? (16 - (_image.height() % IChunk::Depth)) : 0); }
	
	std::shared_ptr<IChunk> construct(const IChunkManager* chunkManager, unsigned chunkX, unsigned chunkY, unsigned chunkZ) override;
};
