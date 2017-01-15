#include "SpriteManager.h"
#include <future>
#include "ISprite.h"
#include "ICamera.h"
#include "ILightSource.h"
#include <glm/detail/type_vec3.hpp>
#include "ISpriteVoxel.h"
#include "ISpriteCollisionSet.h"
#include "SpriteCollision.h"
#include <forward_list>

const unsigned DefaultCollisionPoolSize = 100;

SpriteManager::SpriteManager() : _numberOfCollidingSprites(0), _collisionTree(DefaultCollisionPoolSize)
{
	_spritesRequiringCollisionUpdate.reserve(DefaultCollisionPoolSize);	
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

void SpriteManager::addSpriteToCollisionSet(const std::shared_ptr<ISprite>& sprite)
{
	_collidingSprites.push_front(sprite);
	_numberOfCollidingSprites++;
	_collisionTree.insertObject(sprite);
}

void SpriteManager::markSpriteForCollisionSetUpdate(const std::shared_ptr<ISprite>& sprite)
{
	_spritesRequiringCollisionUpdate.push_back(sprite);
}

std::shared_ptr<std::forward_list<SpriteCollision>> SpriteManager::findBroadRangeCollisions(const std::forward_list<std::shared_ptr<ISprite>>& sprites) const
{
	std::shared_ptr<std::forward_list<SpriteCollision>> collisions = std::make_shared<std::forward_list<SpriteCollision>>();

	std::for_each(sprites.begin(), sprites.end(), [this, &collisions](const std::shared_ptr<ISprite>& sprite)
	{
		auto aabbCollisions = _collisionTree.queryOverlaps(sprite);
		std::for_each(aabbCollisions.begin(), aabbCollisions.end(), [&collisions, sprite](const std::shared_ptr<IAABB>& collidesWith)
		{
			collisions->push_front(SpriteCollision(sprite, std::static_pointer_cast<ISprite>(collidesWith)));
		});
	});

	return collisions;
}

std::shared_ptr<std::forward_list<SpriteCollision>> SpriteManager::findAllCollisions() const
{
	return findCollisionsForSprites(_collidingSprites);
}

std::shared_ptr<std::forward_list<SpriteCollision>> SpriteManager::findCollisionsForSprite(const std::shared_ptr<ISprite>& sprite) const
{
	std::forward_list<std::shared_ptr<ISprite>> sprites;
	sprites.push_front(sprite);
	return findCollisionsForSprites(sprites);	
}

std::shared_ptr<std::forward_list<SpriteCollision>> SpriteManager::findCollisionsForSprites(const std::forward_list<std::shared_ptr<ISprite>>& sprites) const
{
	std::shared_ptr<std::forward_list<SpriteCollision>> broadRangeCollisions = findBroadRangeCollisions(sprites);
	std::shared_ptr<std::forward_list<SpriteCollision>> narrowRangeCollisions = std::make_shared<std::forward_list<SpriteCollision>>();

	std::for_each(broadRangeCollisions->begin(), broadRangeCollisions->end(), [&narrowRangeCollisions](const SpriteCollision& collision)
	{
		if (collision.getSpriteOne()->collidesWith(*collision.getSpriteTwo()))
		{
			narrowRangeCollisions->push_front(collision);
		}
	});

	return narrowRangeCollisions;
}

void SpriteManager::tickComplete(float timeDelta)
{
	if (_spritesRequiringCollisionUpdate.empty()) return;

	// at some point (unknown at the moment) it will be more efficient to rebuild the tree than shift the items around
	// inside it but that will be at the very upper end (hence the 90% below).
	// this needs measuring
	if (_spritesRequiringCollisionUpdate.size() < _numberOfCollidingSprites * 9 / 10)
	{
		std::for_each(_spritesRequiringCollisionUpdate.begin(), _spritesRequiringCollisionUpdate.end(), [this](const std::shared_ptr<ISprite>& sprite)
		{
			_collisionTree.updateObject(sprite);
		});
	}
	else
	{
		_collisionTree = AABBTree(std::max(static_cast<unsigned>(_numberOfCollidingSprites), DefaultCollisionPoolSize));
		std::for_each(_collidingSprites.begin(), _collidingSprites.end(), [this](const std::shared_ptr<ISprite>& sprite)
		{
			_collisionTree.insertObject(sprite);
		});
	}
	_spritesRequiringCollisionUpdate.erase(_spritesRequiringCollisionUpdate.begin(), _spritesRequiringCollisionUpdate.end());
}

void SpriteManager::removeSprite(const std::shared_ptr<ISprite>& sprite)
{
	_sprites.remove(sprite);
	auto collidingIter = std::find(_collidingSprites.begin(), _collidingSprites.end(), sprite);
	if (collidingIter != _collidingSprites.end())
	{
		_collidingSprites.remove(sprite);
		_collisionTree.removeObject(sprite);
		_numberOfCollidingSprites--;
	}
	auto collisionUpdateIter = std::find(_spritesRequiringCollisionUpdate.begin(), _spritesRequiringCollisionUpdate.end(), sprite);
	if (collisionUpdateIter != _spritesRequiringCollisionUpdate.end())
	{
		_spritesRequiringCollisionUpdate.erase(collisionUpdateIter);
	}
	auto requiringRebuildIter = std::find(_spritesRequiringRebuild.begin(), _spritesRequiringRebuild.end(), sprite);
	if (requiringRebuildIter != _spritesRequiringRebuild.end())
	{
		_spritesRequiringRebuild.erase(requiringRebuildIter);
	}
}
