#pragma once
#include <vector>
#include <memory>
#include <glfw/glfw3.h>
#include "IVoxel.h"

class VoxelGeometry
{
private:
	VoxelGeometry()
	{
	}

public:
	static void AddVerticesAndColors(const IVoxel& voxel,
		GLuint x,
		GLuint y,
		GLuint z,
		std::vector<GLfloat>& vertices,
		std::vector<GLfloat>& colors,
		std::vector<GLfloat>& normals);
};
