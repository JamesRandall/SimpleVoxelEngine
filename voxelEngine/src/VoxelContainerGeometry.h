#pragma once
#include <vector>
#include <memory>
#include <map>
#include <glad/glad.h>
#include "IVoxelContainer.h"

class IChunkManager;

struct voxelIndex
{
	unsigned start;
	unsigned end;
	voxelIndex() : start(0), end(0) { }
	voxelIndex(unsigned start, unsigned end) : start(start), end(end) { }
};

class VoxelContainerGeometry
{
private:
	std::vector<GLfloat> _vertices;
	std::vector<GLfloat> _colors;
	std::vector<GLfloat> _normals;
	std::vector<GLfloat> _visbilites;
	std::map<std::shared_ptr<IVoxel>, voxelIndex> _visibilityIndexes;
public:
	VoxelContainerGeometry(const IVoxelContainer& voxelWorld, const IVoxelContainer& voxelContainer, unsigned int chunkX, unsigned int chunkY, unsigned int chunkZ);
	~VoxelContainerGeometry();
	const std::vector<GLfloat>& getVertices() const { return _vertices; }
	const std::vector<GLfloat>& getColors() const { return _colors; }
	const std::vector<GLfloat>& getNormals() const { return _normals; }
	const std::vector<GLfloat>& getVisbility() const { return _visbilites; }
	bool needsRendering() const { return !_vertices.empty(); }
	voxelIndex setVisibility(const std::shared_ptr<IVoxel>& voxel, bool visibility);
};


