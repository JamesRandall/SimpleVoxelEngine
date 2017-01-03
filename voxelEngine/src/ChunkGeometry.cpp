#include "ChunkGeometry.h"
#include "Chunk.h"
#include "VoxelGeometry.h"
#include "ChunkManager.h"

ChunkGeometry::ChunkGeometry(const IChunkManager& chunkManager, const IChunk& chunk, unsigned int chunkX, unsigned int chunkY, unsigned int chunkZ)
{
	std::vector<GLfloat>& vertices = _vertices;
	std::vector<GLfloat>& colors = _colors;
	std::vector<GLfloat>& normals = _normals;

	unsigned int xOffset = chunkX * IChunk::Width;
	unsigned int yOffset = chunkY * IChunk::Height;
	unsigned int zOffset = chunkZ * IChunk::Depth;
	unsigned int worldWidth = chunkManager.getWidth() * IChunk::Width;
	unsigned int worldHeight = chunkManager.getHeight() * IChunk::Height;
	unsigned int worldDepth = chunkManager.getDepth() * IChunk::Depth;

	chunk.forEachVoxel([&](const std::shared_ptr<IVoxel>& voxel, GLuint x, GLuint y, GLuint z) {
		GLuint worldX = x + xOffset;
		GLuint worldY = y + yOffset;
		GLuint worldZ = z + zOffset;

		// you can squeeze all this into an expression but it hurts the eyes
		// if we know the voxel is in this chunk then we get the voxel directly from the chunk, if it's not we use the chunk manager
		// the reason for this conditional is that if we use the chunk manager their is an additional search to find the chunk
		const std::shared_ptr<IVoxel>& left = worldX == 0 ? nullptr : x > 0 ? chunk.getVoxel(x - 1, y, z) : chunkManager.getVoxel(worldX-1, worldY, worldZ);
		const std::shared_ptr<IVoxel>& right = worldX == (worldWidth-1) ? nullptr : x < (IChunk::Width-1) ? chunk.getVoxel(x + 1, y, z) : chunkManager.getVoxel(worldX + 1, worldY, worldZ);
		const std::shared_ptr<IVoxel>& top = worldY == 0 ? nullptr : y > 0 ? chunk.getVoxel(x, y-1, z) : chunkManager.getVoxel(worldX, worldY-1, worldZ);
		const std::shared_ptr<IVoxel>& bottom = worldY == (worldHeight - 1) ? nullptr : y < (IChunk::Height - 1) ? chunk.getVoxel(x, y + 1, z) : chunkManager.getVoxel(worldX, worldY + 1, worldZ);
		const std::shared_ptr<IVoxel>& front = worldZ == 0 ? nullptr : z > 0 ? chunk.getVoxel(x, y, z - 1) : chunkManager.getVoxel(worldX, worldY, worldZ - 1);
		const std::shared_ptr<IVoxel>& rear = worldZ == (worldDepth - 1) ? nullptr : z < (IChunk::Depth - 1) ? chunk.getVoxel(x, y, z + 1) : chunkManager.getVoxel(worldX, worldY, worldZ + 1);

		// we render if any of the sides are theoretically showing.
		// in future a further optimisation we could consider is looking for closed spaces and only rendering if a side is not next to an enclosed space
		// however this is unlikely to happen in the current usage of the engine
		bool needToRender = left == nullptr || right == nullptr || top == nullptr || bottom == nullptr || front == nullptr || rear == nullptr;

		if (needToRender)
		{
			VoxelGeometry::AddVerticesAndColors(*voxel, worldX, worldY, worldZ, vertices, colors, normals);
		}
		return;
	});	
}

ChunkGeometry::~ChunkGeometry()
{
}
