#pragma once
#include "ISpriteVoxel.h"

class SpriteVoxel : public ISpriteVoxel
{
	color _color;
	bool _isActive;
public:

	SpriteVoxel(color color, bool isActive=true) : _color(color), _isActive(isActive)
	{
	}

	virtual ~SpriteVoxel()
	{
	}

	color getColor() const override { return _color; }
	void setColor(color color) override { _color = color; }

	bool getIsActive() const override { return _isActive; }
	void setIsActive(bool isActive) override { _isActive = isActive; }
};

