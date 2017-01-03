#include <future>
#include <glm/glm.hpp>
#include "ChunkManager.h"
#include <algorithm>
#include "IChunkFactory.h"
#include "VoxelGeometry.h"
#include "Camera.h"

ChunkManager::ChunkManager(const std::shared_ptr<IChunkFactory>& chunkFactory, unsigned int chunksAcross, unsigned int chunksUp, unsigned int chunksDeep)
{
	_width = chunksAcross;
	_height = chunksUp;
	_depth = chunksDeep;
	_chunkFactory = chunkFactory;
	_chunks.reserve(chunksAcross * chunksUp * chunksDeep);

	for (unsigned int z = 0; z < _depth; z++)
	{
		for (unsigned int y = 0; y < _height; y++)
		{
			for (unsigned int x = 0; x < _width; x++)
			{
				_chunks.push_back(nullptr);
			}
		}
	}
}

ChunkManager::~ChunkManager()
{
}

const std::shared_ptr<IChunk>& ChunkManager::getChunkWithChunkCoordinates(unsigned int x, unsigned int y, unsigned int z) const
{
	unsigned int index = getChunkIndex(x, y, z);
	return _chunks[index];
}

const std::shared_ptr<IChunk>& ChunkManager::getChunkWithVoxelCoordinates(unsigned int x, unsigned int y, unsigned int z) const
{
	unsigned int chunkX = x / IChunk::Width;
	unsigned int chunkY = y / IChunk::Height;
	unsigned int chunkZ = z / IChunk::Depth;

	return getChunkWithChunkCoordinates(chunkX, chunkY, chunkZ);
}

const std::shared_ptr<IVoxel>& ChunkManager::getVoxel(unsigned int x, unsigned int y, unsigned int z) const
{
	unsigned int voxelX = x % IChunk::Width;
	unsigned int voxelY = y % IChunk::Height;
	unsigned int voxelZ = z % IChunk::Depth;
	std::shared_ptr<IChunk> chunk = getChunkWithVoxelCoordinates(x, y, z);
	if (chunk->isConstructed())
	{
		return chunk->getVoxel(voxelX, voxelY, voxelZ);
	}
	return IVoxel::nullVoxel;
}

void ChunkManager::forEachChunk(const forEachChunkFunction& func) const
{
	unsigned int vectorIndex = 0;
	for (unsigned int z = 0; z < _depth; z++)
	{
		for (unsigned int y = 0; y < _height; y++)
		{
			for (unsigned int x = 0; x < _width; x++)
			{
				const std::shared_ptr<IChunk>& chunk= _chunks[vectorIndex];
				vectorIndex++;
				func(chunk, x, y, z);				
			}
		}
	}
}

void ChunkManager::render(const ICamera& camera, const ILightSource& light) const
{
	glm::vec3 cameraPosition = camera.getPosition();
	float cameraRange = camera.getFarDepth();
	forEachChunk([&](const std::shared_ptr<IChunk>& chunk, unsigned int x, unsigned int y, unsigned int z)
	{
		if (chunkInCamera(camera, x, y, z))
		{
			chunk->render(camera, light);
		}		
	});
}

bool ChunkManager::chunkInCamera(const ICamera& camera, unsigned x, unsigned y, unsigned z) const
{
	glm::vec3 frontLeftBottom(x * IChunk::Width, y * IChunk::Height, z * IChunk::Depth);
	if (camera.pointInCamera(frontLeftBottom)) return true;
	
	glm::vec3 frontLeftTop(x * IChunk::Width, y * IChunk::Height + IChunk::Height, z * IChunk::Depth);
	if (camera.pointInCamera(frontLeftTop)) return true;

	glm::vec3 frontRightTop(x * IChunk::Width + IChunk::Width, y * IChunk::Height + IChunk::Height, z * IChunk::Depth);
	if (camera.pointInCamera(frontRightTop)) return true;

	glm::vec3 frontRightBottom(x * IChunk::Width + IChunk::Width, y * IChunk::Height, z * IChunk::Depth);
	if (camera.pointInCamera(frontRightBottom)) return true;

	glm::vec3 rearLeftBottom(x * IChunk::Width, y * IChunk::Height, z * IChunk::Depth + IChunk::Depth);
	if (camera.pointInCamera(rearLeftBottom)) return true;

	glm::vec3 rearLeftTop(x * IChunk::Width, y * IChunk::Height + IChunk::Height, z * IChunk::Depth + IChunk::Depth);
	if (camera.pointInCamera(rearLeftTop)) return true;

	glm::vec3 rearRightTop(x * IChunk::Width + IChunk::Width, y * IChunk::Height + IChunk::Height, z * IChunk::Depth + IChunk::Depth);
	if (camera.pointInCamera(rearRightTop)) return true;

	glm::vec3 rearRightBottom(x * IChunk::Width + IChunk::Width, y * IChunk::Height, z * IChunk::Depth + IChunk::Depth);
	if (camera.pointInCamera(rearRightBottom)) return true;

	return false;
}

void ChunkManager::rebuildGeometry()
{
	size_t numberOfRebuilds = _chunksRequiringRebuild.size();
	if (numberOfRebuilds == 0)
	{
		return;
	}
	std::vector<std::future<void>> rebuildFutures;
	std::mutex decrementMutex;
	std::condition_variable rebuildComplete;
	std::mutex rebuildCompleteMutex;
	std::unique_lock<std::mutex> rebuildCompleteLock(rebuildCompleteMutex);

	std::for_each(std::begin(_chunksRequiringRebuild), std::end(_chunksRequiringRebuild), [this, &rebuildFutures, &decrementMutex, &numberOfRebuilds, &rebuildComplete](const std::shared_ptr<ChunkRebuildInstruction>& instruction)
	{
		if (instruction->chunk->isConstructed())
		{
			unsigned x = instruction->x, y = instruction->y, z = instruction->z;
			IChunk& chunk = *instruction->chunk;

			rebuildFutures.push_back(std::async(std::launch::async, [this, &chunk, &decrementMutex, &numberOfRebuilds, &rebuildComplete, x, y, z]()
			{
				chunk.rebuildGeometry(*this, x, y, z);
				{
					std::lock_guard<std::mutex> guard(decrementMutex);
					numberOfRebuilds--;
					if (numberOfRebuilds == 0)
					{
						rebuildComplete.notify_one();
					}
				}
			}));			
		}
	});
	rebuildComplete.wait(rebuildCompleteLock);

	std::for_each(std::begin(_chunksRequiringRebuild), std::end(_chunksRequiringRebuild), [](const std::shared_ptr<ChunkRebuildInstruction>& instruction)
	{
		instruction->chunk->prepareRenderer();
	});
}

void ChunkManager::constructChunks(const std::vector<std::shared_ptr<ChunkManager::ChunkRebuildInstruction>>& chunksRequiringConstruction)
{
	if (!chunksRequiringConstruction.empty())
	{
		std::vector<std::future<void>> constructionFutures;
		size_t numberOfJobs = chunksRequiringConstruction.size();
		constructionFutures.reserve(numberOfJobs);

		std::mutex assignmentMutex;
		std::condition_variable constructionComplete;
		std::mutex constructionCompleteMutex;
		std::unique_lock<std::mutex> constructionCompleteLock(constructionCompleteMutex);
		std::for_each(std::begin(chunksRequiringConstruction), std::end(chunksRequiringConstruction), [&](const std::shared_ptr<ChunkRebuildInstruction>& element) {
			unsigned x = element->x, y = element->y, z = element->z;
			unsigned int chunkIndex = getChunkIndex(x, y, z);
			constructionFutures.push_back(std::async(std::launch::async, [this, x, y, z, chunkIndex, numberOfJobs, &assignmentMutex, &constructionComplete]() {
				std::shared_ptr<IChunk> newChunk = _chunkFactory->construct(this, x, y, z);
				{
					std::lock_guard<std::mutex> guard(assignmentMutex);
					_chunks[chunkIndex] = newChunk;
					_chunksRequiringRebuild.push_back(std::make_shared<ChunkRebuildInstruction>(newChunk, x, y, z));
					if (_chunksRequiringRebuild.size() == numberOfJobs)
					{
						constructionComplete.notify_one();
					}
				}
			}));
		});
		constructionComplete.wait(constructionCompleteLock);
	}
}

bool ChunkManager::tick(float timeDelta)
{
	bool didUpdate = false;
	std::vector<std::shared_ptr<ChunkRebuildInstruction>> chunksRequiringConstruction;

	forEachChunk([&](const std::shared_ptr<IChunk>& chunk, unsigned int x, unsigned int y, unsigned int z)
	{
		// we do construction in this way to later enable paging / dynamic loading of chunks - it will need refinement!
		if (chunk == nullptr || !chunk->isConstructed())
		{
			chunksRequiringConstruction.push_back(std::make_shared<ChunkRebuildInstruction>(chunk, x, y, z));
		}
		else if (chunk != nullptr && chunk->tick(timeDelta))
		{
			_chunksRequiringRebuild.push_back(std::make_shared<ChunkRebuildInstruction>(chunk, x, y, z));
		}
	});

	constructChunks(chunksRequiringConstruction);	
	
	rebuildGeometry();

	_chunksRequiringRebuild.clear();

	return didUpdate;
}
