#include "VoxelGeometry.h"
#include "IVoxel.h"

void VoxelGeometry::AddVerticesAndColors(const IVoxel& voxel,
	GLuint x, GLuint y, GLuint z,
	std::vector<GLfloat>& vertices,
	std::vector<GLfloat>& colors,
	std::vector<GLfloat>& normals)
{
	const GLfloat VoxelSize = 1.0f;
	const GLfloat VoxelHalfSize = VoxelSize / 2;

	GLfloat normalBufferData[] = {
		0, 0, 1, // front
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,

		0, 0, -1, // rear
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,

		1, 0, 0, // right
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,

		-1, 0, 0, // left
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,

		0, 1, 0, // top
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,

		0, -1, 0, // bottom
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0
	};
	unsigned normalsArraySize = sizeof(normalBufferData) / sizeof(GLfloat);
	normals.insert(normals.end(), &normalBufferData[0], &normalBufferData[normalsArraySize]);

	GLfloat vertexBufferData[] = {
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize, // front
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,

		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize, // rear
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,

		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize, // right
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,

		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize, // left
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,

		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize, // top
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,

		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize, // bottom
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + VoxelHalfSize,
		(x*VoxelSize) + -VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize,
		(x*VoxelSize) + VoxelHalfSize, (y*VoxelSize) + -VoxelHalfSize, (z*VoxelSize) + -VoxelHalfSize
	};
	unsigned verticesArraySize = sizeof(vertexBufferData) / sizeof(GLfloat);
	vertices.insert(vertices.end(), &vertexBufferData[0], &vertexBufferData[verticesArraySize]);
	
	size_t numberOfVertices = verticesArraySize / 3;
	for (int triangleIndex = 0; triangleIndex < numberOfVertices; triangleIndex++)
	{
		color voxelColor = voxel.getColor();
		colors.push_back(voxelColor.r);
		colors.push_back(voxelColor.g);
		colors.push_back(voxelColor.b);
	}	
}
