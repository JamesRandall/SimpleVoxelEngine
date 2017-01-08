#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "VoxelContainerGeometry.h"

class ILightSource;
class ICamera;

class VoxelRenderer
{
private:
	GLuint _vertexBuffer;
	GLuint _colorBuffer;
	GLuint _normalBuffer;
	GLuint _vertexArrayId;
	GLuint _visibilityBuffer;
	GLsizei _numberOfVertices;

	void deleteBuffersIfNeeded() const;
public:
	VoxelRenderer();
	~VoxelRenderer();

	void render(const ICamera& camera, const VoxelContainerGeometry& chunkGeometry, const ILightSource& light, const glm::mat4 modelMatrix=glm::mat4(1.0)) const;
	void buildBuffers(const VoxelContainerGeometry& chunkGeometry);
	void rebindVisibility(const VoxelContainerGeometry& chunkGeometry, const voxelIndex& voxelIndex) const;
};

