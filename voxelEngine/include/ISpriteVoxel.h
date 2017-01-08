#pragma once
#include "IVoxel.h"

class ISpriteVoxel : public IVoxel
{
public:
	ISpriteVoxel() = default;
	virtual ~ISpriteVoxel() = default;
	virtual void setColor(color color) = 0;	
	virtual void setIsActive(bool isActive) = 0;
};
