#pragma once
#include "IVoxel.h"

class GreyScaleVoxel : public IVoxel
{
	unsigned char _color;
public:
	virtual ~GreyScaleVoxel() = default;
	GreyScaleVoxel(unsigned char color) : _color(color) { }

	color getColor() const override
	{
		float colorValue = static_cast<float>(_color) / 255.0f;
		return color(colorValue, colorValue, colorValue);
	};
};