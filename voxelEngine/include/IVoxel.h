#pragma once
#include <memory>
#include "color.h"

class IVoxel
{
public:
	IVoxel() = default;
	virtual ~IVoxel() = default;
	virtual color getColor() const = 0;
	virtual bool getIsActive() const = 0;
	static const std::shared_ptr<IVoxel>& nullVoxel;
};