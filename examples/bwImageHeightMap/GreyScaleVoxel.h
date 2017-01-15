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

	bool getIsActive() const override { return true; }
};

class RgbVoxel : public IVoxel
{
	color _color;
public:
	virtual ~RgbVoxel() = default;
	RgbVoxel(unsigned r, unsigned g, unsigned b) : _color(r / 255.0f, g / 255.0f, b / 255.0f) { }

	color getColor() const override { return _color; }

	bool getIsActive() const override { return true; }
};