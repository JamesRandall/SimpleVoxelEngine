#include "VoxelatedConstructionSpriteBehaviour.h"
#include "ISprite.h"
#include "IVoxelContainer.h"
#include "ISpriteVoxel.h"
#include "SpriteVoxel.h"
#include "ISpriteManager.h"

VoxelatedConstructionSpriteBehaviour::VoxelatedConstructionSpriteBehaviour(bool xFirst, float buildInterval) : _x(0), _y(0), _z(0), _lastUpdate(0.0f), _done(false), _xFirst(xFirst), _buildInterval(buildInterval)
{
}


VoxelatedConstructionSpriteBehaviour::~VoxelatedConstructionSpriteBehaviour()
{
}

void VoxelatedConstructionSpriteBehaviour::attach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite)
{
	bool requireRebuild = false;
	
	const std::shared_ptr<IVoxelContainer> voxelContainer = std::dynamic_pointer_cast<IVoxelContainer> (sprite);
	voxelContainer->forEachVoxel([&requireRebuild](const std::shared_ptr<IVoxel>& voxel, GLuint, GLuint, GLuint)
	{		
		const std::shared_ptr<ISpriteVoxel>& spriteVoxel = std::dynamic_pointer_cast<ISpriteVoxel>(voxel);
		requireRebuild |= spriteVoxel->getIsActive();
		spriteVoxel->setIsActive(false);
	});
}

void VoxelatedConstructionSpriteBehaviour::detach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite)
{
	
}

bool VoxelatedConstructionSpriteBehaviour::tick(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite, float timeDelta)
{
	const float _updateDelay = _buildInterval;
	if (_done)
	{
		return true;
	}

	std::shared_ptr<IVoxelContainer> voxelContainer = std::dynamic_pointer_cast<IVoxelContainer> (sprite);
	bool requireRebuild = false;
	_lastUpdate += timeDelta;
	spriteSize size = sprite->getSize();
	std::shared_ptr<std::vector<std::shared_ptr<IVoxel>>> voxelsToUpdate = std::make_shared<std::vector<std::shared_ptr<IVoxel>>>();
	while(_lastUpdate > _updateDelay)
	{
		std::shared_ptr<IVoxel> underlyingVoxel = voxelContainer->getVoxel(_x, _y, _z);
		std::shared_ptr<ISpriteVoxel> voxel = std::dynamic_pointer_cast<ISpriteVoxel>(underlyingVoxel);
		if (voxel != nullptr)
		{
			voxel->setIsActive(true);
			voxelsToUpdate->push_back(underlyingVoxel);
		}

		unsigned *firstInc = _xFirst ? &_x : &_z;
		unsigned firstMax = _xFirst ? size.width : size.depth;
		unsigned *secondInc = _xFirst ? &_z : &_x;
		unsigned secondMax = _xFirst ? size.depth : size.width;

		(*firstInc)++;
		if ((*firstInc) >= firstMax)
		{
			(*firstInc) = 0;
			(*secondInc)++;
			if ((*secondInc) >= secondMax)
			{
				(*secondInc) = 0;
				_y++;
				if (_y >= size.height)
				{
					_done = true;
					_lastUpdate = 0.0f;
				}
			}
		}

		if (voxel != nullptr)
		{
			_lastUpdate -= _updateDelay;
		}		
	}

	if (!voxelsToUpdate->empty())
	{
		spriteManager.requestUpdate(spriteUpdateRequest(sprite, voxelsToUpdate, spriteUpdateRequestTypeEnum::cheap));
	}

	return _done;
}
