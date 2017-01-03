#pragma once
#include <memory>
#include "color.h"

class IVoxel
{
protected:
	~IVoxel() = default;
public:
	virtual color getColor() const = 0;

	static const std::shared_ptr<IVoxel>& nullVoxel;
};