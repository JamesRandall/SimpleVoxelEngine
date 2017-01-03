#pragma once
#include <vector>
#include <memory>
#include <glad/glad.h>
#include "Chunk.h"

class ChunkGeometry
{
private:
	std::vector<GLfloat> _vertices;
	std::vector<GLfloat> _colors;
	std::vector<GLfloat> _normals;
public:
	ChunkGeometry(const IChunkManager& chunkManager, const IChunk& chunk, unsigned int chunkX, unsigned int chunkY, unsigned int chunkZ);
	~ChunkGeometry();
	const std::vector<GLfloat>& getVertices() const { return _vertices; }
	const std::vector<GLfloat>& getColors() const { return _colors; }
	const std::vector<GLfloat>& getNormals() const { return _normals; }
	bool needsRendering() const { return !_vertices.empty(); }
};

typedef std::shared_ptr<ChunkGeometry> ChunkGeometryPtr;
