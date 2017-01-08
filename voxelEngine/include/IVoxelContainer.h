#pragma once
#include <functional>
#include <glad/glad.h>
#include <vector>

class IVoxel;
typedef std::function<void(const std::shared_ptr<IVoxel>&, GLuint, GLuint, GLuint)> forEachVoxelFunction;

class IVoxelContainer
{
public:
	virtual ~IVoxelContainer() = default;
	virtual void forEachVoxel(const forEachVoxelFunction& func, bool includeNull = false) const=0;
	virtual std::shared_ptr<IVoxel> getVoxel(unsigned int x, unsigned int y, unsigned int z) const = 0;
	virtual unsigned getContainerWidthInVoxels() const = 0;
	virtual unsigned getContainerHeightInVoxels() const = 0;
	virtual unsigned getContainerDepthInVoxels() const = 0;	
};
