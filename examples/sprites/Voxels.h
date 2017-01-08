#pragma once
#include "IVoxel.h"

class GrassVoxel : public IVoxel
{
public:
	virtual ~GrassVoxel() = default;
	color getColor() const override { return color(0.0f, 0.95f, 0.0f); }
};

class MountainVoxel : public IVoxel
{
public:
	virtual ~MountainVoxel() = default;
	color getColor() const override { return color(0.5f, 0.5f, 0.5f); }
};

class SnowVoxel : public IVoxel
{
public:
	virtual ~SnowVoxel() = default;
	color getColor() const override { return color(0.95f, 0.95f, 0.95f); }
};

