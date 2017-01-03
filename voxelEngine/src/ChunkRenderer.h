#pragma once
#include <glad/glad.h>
#include <memory>
#include "ChunkGeometry.h"
#include "ShaderManager.h"

class ILightSource;

class ChunkRenderer
{
private:
	GLuint _vertexBuffer;
	GLuint _colorBuffer;
	GLuint _normalBuffer;
	GLuint _vertexArrayId;
	GLsizei _numberOfVertices;

	void deleteBuffersIfNeeded() const;
public:
	ChunkRenderer();
	~ChunkRenderer();

	void render(const ICamera& camera, const ChunkGeometry& chunkGeometry, const ILightSource& light) const;
	void buildBuffers(const ChunkGeometry& chunkGeometry);
};

