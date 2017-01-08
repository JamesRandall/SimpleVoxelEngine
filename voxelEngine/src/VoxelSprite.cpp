#include "VoxelSprite.h"
#include "SpriteVoxel.h"
#include "VoxelEngineException.h"
#include "ISpriteBehaviour.h"
#include <glm/gtc/matrix_transform.inl>
#include <algorithm>

VoxelSprite::VoxelSprite(const spriteSize& spriteSize, const spritePosition& spritePosition) : _size(spriteSize), _position(spritePosition)
{
	unsigned size = spriteSize.width * spriteSize.height * spriteSize.depth;
	_voxels.reserve(size);
	for(unsigned index = 0; index < size; index++)
	{
		_voxels.push_back(nullptr);
	}
}

VoxelSprite::VoxelSprite(const spriteSize& spriteSize, const spritePosition& spritePosition, const std::vector<std::shared_ptr<ISpriteVoxel>>& voxels) : _voxels(voxels), _size(spriteSize), _position(spritePosition)
{
	if (_voxels.size() != (spriteSize.width * spriteSize.height * spriteSize.depth))
	{
		throw VoxelEngineException("Wrong number of voxels for the sprite");
	}
}

bool VoxelSprite::tick(ISpriteManager& spriteManager, float timeDelta)
{
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
	if (_renderer == nullptr) return;
	_renderer->render(camera, *_geometry, light, glm::translate(glm::mat4(1.0f), glm::vec3(_position.x, _position.y, _position.z)));
}

std::shared_ptr<IVoxel> VoxelSprite::getVoxel(unsigned int x, unsigned int y, unsigned int z) const
{
	unsigned int index = getVoxelIndex(x, y, z);	
	return _voxels[index];
}

void VoxelSprite::setVoxel(unsigned int x, unsigned int y, unsigned int z, const std::shared_ptr<ISpriteVoxel>& voxel)
{
	unsigned int index = getVoxelIndex(x, y, z);
	_voxels[index] = voxel;
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
				const std::shared_ptr<IVoxel>& voxel = _voxels[vectorIndex];
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
	if (_geometry != nullptr)
	{
		voxelIndex voxelIndex = _geometry->setVisibility(voxel, voxel->getIsActive());
		if (_renderer != nullptr)
		{
			_renderer->rebindVisibility(*_geometry, voxelIndex);
		}
	}
}

void VoxelSprite::rebuildGeometry()
{
	_geometry = std::make_shared<VoxelContainerGeometry>(*this, *this, 0, 0, 0); // sprites are translated in the vertex shader so the geometry is always built around the origin
}

void VoxelSprite::prepareRenderer()
{
	if (_geometry == nullptr) return;
	_renderer = std::make_shared<VoxelRenderer>();
	_renderer->buildBuffers(*_geometry);
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
