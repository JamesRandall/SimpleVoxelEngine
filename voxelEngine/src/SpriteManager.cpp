#include "SpriteManager.h"
#include <future>
#include "ISprite.h"
#include "ICamera.h"
#include "ILightSource.h"
#include "ISpriteBehaviour.h"
#include <glm/detail/type_vec3.hpp>
#include "ISpriteVoxel.h"


SpriteManager::SpriteManager()
{
}


SpriteManager::~SpriteManager()
{
}

void SpriteManager::rebuildGeometry()
{
	size_t numberOfRebuilds = _spritesRequiringRebuild.size();
	if (numberOfRebuilds == 0)
	{
		return;
	}
	std::vector<std::future<void>> rebuildFutures;
	std::mutex decrementMutex;
	std::condition_variable rebuildComplete;
	std::mutex rebuildCompleteMutex;
	std::unique_lock<std::mutex> rebuildCompleteLock(rebuildCompleteMutex);

	std::for_each(std::begin(_spritesRequiringRebuild), std::end(_spritesRequiringRebuild), [this, &rebuildFutures, &decrementMutex, &numberOfRebuilds, &rebuildComplete](const std::shared_ptr<ISprite>& sprite)
	{
		rebuildFutures.push_back(std::async(std::launch::async, [this, &sprite, &decrementMutex, &numberOfRebuilds, &rebuildComplete]()
		{
			// TODO: we need to move translate into the shader so that we don't need to rebuild the geometry as we move the sprite
			sprite->rebuildGeometry();
			{
				std::lock_guard<std::mutex> guard(decrementMutex);
				numberOfRebuilds--;
				if (numberOfRebuilds == 0)
				{
					rebuildComplete.notify_one();
				}
			}
		}));
	});
	rebuildComplete.wait(rebuildCompleteLock);

	// this has to be done in the main open gl thread otherwise the buffers will not be visible to the render cycle
	std::for_each(std::begin(_spritesRequiringRebuild), std::end(_spritesRequiringRebuild), [](const std::shared_ptr<ISprite>& sprite)
	{
		sprite->prepareRenderer();
	});
}

bool SpriteManager::tick(float timeDelta)
{
	// scope to multi thread this
	std::for_each(std::begin(_sprites), std::end(_sprites), [timeDelta, this](const std::shared_ptr<ISprite>& sprite)
	{
		if (sprite->tick(*this, timeDelta))
		{
			_spritesRequiringRebuild.push_back(sprite);
		}		
	});
	
	rebuildGeometry();
	_spritesRequiringRebuild.clear();	
	return true;
}

void SpriteManager::render(const ICamera& camera, const ILightSource& light) const
{
	glm::vec3 cameraPosition = camera.getPosition();
	float cameraRange = camera.getFarDepth();
	std::for_each(std::begin(_sprites), std::end(_sprites), [&camera, &light](const std::shared_ptr<ISprite>& sprite)
	{
		//if (spriteInCamera(camera, x, y, z))
		{
			sprite->render(camera, light);
		}
	});
}

void SpriteManager::requestUpdate(const spriteUpdateRequest& request)
{
	if (request.type == spriteUpdateRequestTypeEnum::full)
	{
		_spritesRequiringRebuild.push_back(request.sprite);
	}
	else
	{
		if (request.voxels != nullptr)
		{
			std::for_each(request.voxels->begin(), request.voxels->end(), [&request](const std::shared_ptr<IVoxel>& spriteVoxel)
			{
				request.sprite->cheapUpdate(spriteVoxel);
			});
		}
	}
}

