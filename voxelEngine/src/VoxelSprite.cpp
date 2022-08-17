#include "VoxelSprite.h"
#include "SpriteVoxel.h"
#include "VoxelEngineException.h"
#include "ISpriteBehaviour.h"
#include "ISpriteManager.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

VoxelSprite::VoxelSprite(const spriteSize& spriteSize, const spriteVec3& spritePosition) :
	_frameIndex(0),
	_timeBetweenFrames(0),
	_frameUpdateTime(0),
	_size(spriteSize),
	_position(spritePosition),
	_aabb(spritePosition.x, spritePosition.y, spritePosition.z, spritePosition.x + spriteSize.width, spritePosition.y + spriteSize.height, spritePosition.z + spriteSize.depth),
	_type(0),
	_velocity(0.0f, 0.0f, 0.0f),
	_cumulativeVelocity(0.0f,0.0f,0.0f)
{
	unsigned size = spriteSize.width * spriteSize.height * spriteSize.depth;
	VoxelSpriteFramePtr frame = std::make_shared<VoxelSpriteFrame>();
	frame->voxels.reserve(size);
	for (unsigned index = 0; index < size; index++)
	{
		frame->voxels.push_back(nullptr);
	}
	_frames.push_back(frame);
}

VoxelSprite::VoxelSprite(const spriteSize& spriteSize, const spriteVec3& spritePosition, const std::vector<std::shared_ptr<ISpriteVoxel>>& voxels) :
	_frameIndex(0),
	_timeBetweenFrames(0),
	_frameUpdateTime(0),
	_size(spriteSize),
	_position(spritePosition),
	_aabb(spritePosition.x, spritePosition.y, spritePosition.z, spritePosition.x + spriteSize.width, spritePosition.y + spriteSize.height, spritePosition.z + spriteSize.depth),
	_type(0),
	_velocity(0.0f, 0.0f, 0.0f),
	_cumulativeVelocity(0.0f, 0.0f, 0.0f)
{
	if (voxels.size() != (spriteSize.width * spriteSize.height * spriteSize.depth))
	{
		throw VoxelEngineException("Wrong number of voxels for the sprite");
	}
	VoxelSpriteFramePtr frame = std::make_shared<VoxelSpriteFrame>(voxels);
	_frames.push_back(frame);
}

VoxelSprite::VoxelSprite(const spriteSize& spriteSize, const spriteVec3& spritePosition, const std::vector<std::vector<std::shared_ptr<ISpriteVoxel>>>& frameVoxels, float timeBetweenFrames, float frameChangeDeferral) :
	_frameIndex(0),
	_size(spriteSize),
	_position(spritePosition),
	_type(0),
	_timeBetweenFrames(timeBetweenFrames),
	_frameUpdateTime(-frameChangeDeferral),
	_velocity(0.0f, 0.0f, 0.0f),
	_cumulativeVelocity(0.0f, 0.0f, 0.0f)
{
	std::for_each(std::begin(frameVoxels), std::end(frameVoxels), [this, spriteSize](const std::vector<std::shared_ptr<ISpriteVoxel>>& voxels)
	              {
		              if (voxels.size() != (spriteSize.width * spriteSize.height * spriteSize.depth))
		              {
			              throw VoxelEngineException("Wrong number of voxels for the sprite");
		              }
		              VoxelSpriteFramePtr frame = std::make_shared<VoxelSpriteFrame>(voxels);
		              _frames.push_back(frame);
	              });
}

bool VoxelSprite::tick(ISpriteManager& spriteManager, float timeDelta)
{
	if (_frames.size() > 1 && timeDelta > 0.0 && _timeBetweenFrames > 0.0)
	{
		_frameUpdateTime += timeDelta;
		while (_frameUpdateTime > _timeBetweenFrames)
		{
			_frameIndex++;
			if (_frameIndex >= _frames.size())
			{
				_frameIndex = 0;
			}

			_frameUpdateTime -= _timeBetweenFrames;
		}
	}

	std::forward_list<std::shared_ptr<DeferredSpriteBehaviourDecorator>> toRemove;
	std::for_each(std::begin(_behaviours), std::end(_behaviours), [&spriteManager, &toRemove, timeDelta, this](const std::shared_ptr<DeferredSpriteBehaviourDecorator>& behaviour)
	{
		if (behaviour->tick(spriteManager, shared_from_this(), timeDelta))
		{
			toRemove.push_front(behaviour);
		}
	});

	std::for_each(std::begin(toRemove), std::end(toRemove), [&spriteManager, this](const std::shared_ptr<DeferredSpriteBehaviourDecorator>& behaviour) {
		_behaviours.remove(behaviour);
		behaviour->detach(spriteManager, shared_from_this());		
	});

	return false;
}

void VoxelSprite::render(const ICamera& camera, const ILightSource& light) const
{
	const VoxelSpriteFramePtr& frame = getCurrentFrame();
	if (frame->renderer == nullptr) return;
	
	frame->renderer->render(camera, *frame->geometry, light, glm::translate(glm::mat4(1.0f), glm::vec3(_position.x, _position.y, _position.z)));
}

std::shared_ptr<IVoxel> VoxelSprite::getVoxel(unsigned int x, unsigned int y, unsigned int z) const
{
	unsigned int index = getVoxelIndex(x, y, z);
	return getCurrentFrame()->voxels[index];
}

void VoxelSprite::setVoxel(unsigned int x, unsigned int y, unsigned int z, const std::shared_ptr<ISpriteVoxel>& voxel) const
{
	unsigned int index = getVoxelIndex(x, y, z);
	getCurrentFrame()->voxels[index] = voxel;
}

void VoxelSprite::forEachVoxel(const forEachVoxelFunction& func, bool includeNull) const
{
	unsigned int vectorIndex = 0;
	for (unsigned z = 0; z < _size.depth; z++)
	{
		for (unsigned y = 0; y < _size.height; y++)
		{
			for (unsigned x = 0; x < _size.width; x++)
			{
				const std::shared_ptr<IVoxel>& voxel = getCurrentFrame()->voxels[vectorIndex];
				vectorIndex++;
				if (voxel != nullptr || includeNull)
				{
					func(voxel, x, y, z);
				}
			}
		}
	}
}

void VoxelSprite::forEachBehaviour(const forEachSpriteBehaviourFunction& function) const
{
	std::for_each(std::begin(_behaviours), std::end(_behaviours), [&function](const std::shared_ptr<DeferredSpriteBehaviourDecorator>& behaviour)
	{
		function(behaviour->getBehaviour());
	});
}

void VoxelSprite::cheapUpdate(const std::shared_ptr<IVoxel>& voxel) const
{
	if (getCurrentFrame()->geometry != nullptr)
	{
		voxelIndex voxelIndex = getCurrentFrame()->geometry->setVisibility(voxel, voxel->getIsActive());
		if (getCurrentFrame()->renderer != nullptr)
		{
			getCurrentFrame()->renderer->rebindVisibility(*getCurrentFrame()->geometry, voxelIndex);
		}
	}
}

void VoxelSprite::rebuildGeometry()
{
	unsigned oldFrameIndex = _frameIndex;
	_frameIndex = 0;
	std::for_each(std::begin(_frames), std::end(_frames), [this](VoxelSpriteFramePtr& frame)
	{
		frame->geometry = std::make_shared<VoxelContainerGeometry>(*this, *this, 0, 0, 0); // sprites are translated in the vertex shader so the geometry is always built around the origin
		_frameIndex++;
	});	
	_frameIndex = oldFrameIndex;
}

void VoxelSprite::prepareRenderer()
{
	std::for_each(std::begin(_frames), std::end(_frames), [this](VoxelSpriteFramePtr& frame)
	{
		if (frame->geometry == nullptr) return;
		frame->renderer = std::make_shared<VoxelRenderer>();
		frame->renderer->buildBuffers(*frame->geometry);
	});	
}

void VoxelSprite::attachSpriteBehaviour(ISpriteManager& spriteManager, const std::shared_ptr<ISpriteBehaviour> behaviour, float executionDeferral)
{
	std::shared_ptr<DeferredSpriteBehaviourDecorator> decorator = std::make_shared<DeferredSpriteBehaviourDecorator>(behaviour, executionDeferral);
	_behaviours.push_front(decorator);
	decorator->attach(spriteManager, shared_from_this());
}

void VoxelSprite::detachSpriteBehaviour(ISpriteManager& spriteManager, const std::shared_ptr<ISpriteBehaviour> behaviour)
{
	// the decorator means we have to search for a matching inner pointer but this is seen as a rare thing.
	// sprites will tend to die or decorators be removed by themselves by returning true from tick
	// when they are done

	auto result = std::find_if(std::begin(_behaviours), std::end(_behaviours), [&behaviour](const std::shared_ptr<DeferredSpriteBehaviourDecorator>& decorator)
	{
		return decorator->getBehaviour() == behaviour;
	});

	_behaviours.remove(*result);
	behaviour->detach(spriteManager, shared_from_this());
};

bool VoxelSprite::collidesWith(const ISprite& other) const
{
	// intended as a narrow range collision on a voxel grid
	const VoxelSprite& otherSprite = static_cast<const VoxelSprite&>(other);
	AABB intersection = getAABB().intersection(otherSprite.getAABB());
	for (unsigned z = intersection.minZ; z < intersection.maxZ; z++)
	{
		for (unsigned y = intersection.minY; y < intersection.maxY; y++)
		{
			for (int x = intersection.minX; x < intersection.maxX; x++)
			{
				const std::shared_ptr<IVoxel>& thisVoxel = getVoxel(x - _position.x, y - _position.y, z - _position.z);
				if (thisVoxel != nullptr)
				{
					spriteVec3 otherPosition = other.getPosition();
					const std::shared_ptr<IVoxel>& otherVoxel = otherSprite.getVoxel(x - otherPosition.x, y - otherPosition.y, z - otherPosition.z);
					if (otherVoxel != nullptr)
					{
						return true;
					}
				}								
			}
		}
	}
	return false;
}