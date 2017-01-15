#pragma once
#include "IVoxel.h"

class TerrainVoxel  : public IVoxel
{
	unsigned char _type;
public:

	TerrainVoxel(unsigned char type) : _type(type)
	{
	}

	virtual ~TerrainVoxel()
	{
	}


	color getColor() const override { return _type == 0 ? color(0.1f, 0.1f, 0.1f) : color(0.9f, 0.9f, 0.9f); }

	bool getIsActive() const override { return true; }
};

