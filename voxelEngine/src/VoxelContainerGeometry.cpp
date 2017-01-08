#include "VoxelContainerGeometry.h"
#include "Chunk.h"
#include "VoxelGeometry.h"
#include "ChunkManager.h"

VoxelContainerGeometry::VoxelContainerGeometry(const IVoxelContainer& voxelWorld, const IVoxelContainer& voxelContainer, unsigned int chunkX, unsigned int chunkY, unsigned int chunkZ)
{
	std::vector<GLfloat>& vertices = _vertices;
	std::vector<GLfloat>& colors = _colors;
	std::vector<GLfloat>& normals = _normals;
	std::vector<GLfloat>& visibilites = _visbilites;

	unsigned int xOffset = chunkX * IChunk::Width;
	unsigned int yOffset = chunkY * IChunk::Height;
	unsigned int zOffset = chunkZ * IChunk::Depth;
	unsigned int worldWidth = voxelWorld.getContainerWidthInVoxels();
	unsigned int worldHeight = voxelWorld.getContainerHeightInVoxels();
	unsigned int worldDepth = voxelWorld.getContainerDepthInVoxels();
	unsigned int containerWidth = voxelContainer.getContainerWidthInVoxels();
	unsigned int containerHeight = voxelContainer.getContainerHeightInVoxels();
	unsigned int containerDepth = voxelContainer.getContainerDepthInVoxels();

	voxelContainer.forEachVoxel([&](const std::shared_ptr<IVoxel>& voxel, GLuint x, GLuint y, GLuint z) {
		//if (voxel->getIsActive())
		{
			GLuint worldX = x + xOffset;
			GLuint worldY = y + yOffset;
			GLuint worldZ = z + zOffset;

			// you can squeeze all this into an expression but it hurts the eyes
			// if we know the voxel is in this chunk then we get the voxel directly from the chunk, if it's not we use the chunk manager
			// the reason for this conditional is that if we use the chunk manager their is an additional search to find the chunk
			const std::shared_ptr<IVoxel> left = worldX == 0 ? nullptr : x > 0 ? voxelContainer.getVoxel(x - 1, y, z) : voxelWorld.getVoxel(worldX - 1, worldY, worldZ);
			const std::shared_ptr<IVoxel> right = worldX == (worldWidth - 1) ? nullptr : x < (containerWidth - 1) ? voxelContainer.getVoxel(x + 1, y, z) : voxelWorld.getVoxel(worldX + 1, worldY, worldZ);
			const std::shared_ptr<IVoxel> top = worldY == 0 ? nullptr : y > 0 ? voxelContainer.getVoxel(x, y - 1, z) : voxelWorld.getVoxel(worldX, worldY - 1, worldZ);
			const std::shared_ptr<IVoxel> bottom = worldY == (worldHeight - 1) ? nullptr : y < (containerHeight - 1) ? voxelContainer.getVoxel(x, y + 1, z) : voxelWorld.getVoxel(worldX, worldY + 1, worldZ);
			const std::shared_ptr<IVoxel> front = worldZ == 0 ? nullptr : z > 0 ? voxelContainer.getVoxel(x, y, z - 1) : voxelWorld.getVoxel(worldX, worldY, worldZ - 1);
			const std::shared_ptr<IVoxel> rear = worldZ == (worldDepth - 1) ? nullptr : z < (containerDepth - 1) ? voxelContainer.getVoxel(x, y, z + 1) : voxelWorld.getVoxel(worldX, worldY, worldZ + 1);

			// we render if any of the sides are theoretically showing.
			// in future a further optimisation we could consider is looking for closed spaces and only rendering if a side is not next to an enclosed space
			// however this is unlikely to happen in the current usage of the engine
			bool needToRender = left == nullptr || right == nullptr || top == nullptr || bottom == nullptr || front == nullptr || rear == nullptr;

			if (needToRender)
			{
				size_t priorSize = vertices.size();
				VoxelGeometry::AddVerticesAndColors(*voxel, worldX, worldY, worldZ, vertices, colors, normals);
				size_t newSize = vertices.size();

				size_t numberOfNewVertices = (newSize - priorSize) / 3;

				// we maintain a set of active statuses that we can push to the GPU. this is because many active
				// state changes do not require a geometry rebuild and it's cheaper to push this data to the GPU
				// and discard in the fragment shader than it is to recaluclate geometry on the CPU
				size_t priorVisibilitySize = visibilites.size();
				for (int index = 0; index < numberOfNewVertices; index++)
				{
					visibilites.push_back(voxel->getIsActive() ? 1.0f : 0.0f);
					visibilites.push_back(voxel->getIsActive() ? 1.0f : 0.0f);
				}
				_visibilityIndexes[voxel] = voxelIndex(priorVisibilitySize, visibilites.size() - 1);
			}			
		}
	});	
}

VoxelContainerGeometry::~VoxelContainerGeometry()
{
}

voxelIndex VoxelContainerGeometry::setVisibility(const std::shared_ptr<IVoxel>& voxel, bool visibility)
{
	voxelIndex index = _visibilityIndexes[voxel];
	std::fill(_visbilites.begin() + index.start, _visbilites.begin() + index.end, visibility ? 1.0f : 0.0f);
	return index;
}
