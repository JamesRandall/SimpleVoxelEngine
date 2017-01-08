#pragma once
#include "ISprite.h"
#include <vector>
#include "VoxelRenderer.h"
#include "IVoxelContainer.h"
#include "ISpriteBehaviour.h"
#include <forward_list>

class ISpriteVoxel;

class VoxelSprite : public ISprite, public IVoxelContainer, public std::enable_shared_from_this<ISprite>
{
	class DeferredSpriteBehaviourDecorator : public ISpriteBehaviour
	{
		float _timeRemainingToExecution;
		std::shared_ptr<ISpriteBehaviour> _behaviour;
	public:
		virtual ~DeferredSpriteBehaviourDecorator() = default;
		DeferredSpriteBehaviourDecorator(const std::shared_ptr<ISpriteBehaviour> behaviour, float deferrment) : _timeRemainingToExecution(deferrment), _behaviour(behaviour) { }
		const std::shared_ptr<ISpriteBehaviour>& getBehaviour() const { return _behaviour; }
		void attach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite) override { _behaviour->attach(spriteManager, sprite); };
		void detach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite) override { _behaviour->detach(spriteManager, sprite); };
		bool tick(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite, float timeDelta) override
		{
			if (_timeRemainingToExecution > 0.0f) _timeRemainingToExecution -= timeDelta;
			if (_timeRemainingToExecution <= 0.0f)
			{
				return _behaviour->tick(spriteManager, sprite, timeDelta);
			}
			return false;
		}
	};

	std::vector<std::shared_ptr<ISpriteVoxel>> _voxels;
	spriteSize _size;
	spritePosition _position;
	std::shared_ptr<VoxelContainerGeometry> _geometry;
	std::shared_ptr<VoxelRenderer> _renderer;
	std::forward_list<std::shared_ptr<DeferredSpriteBehaviourDecorator>> _behaviours;

	unsigned int getVoxelIndex(unsigned int x, unsigned int y, unsigned int z) const { return z * _size.height * _size.width + y * _size.width + x; }

public:
	VoxelSprite::VoxelSprite(const spriteSize& spriteSize, const spritePosition& spritePosition);
	VoxelSprite(const spriteSize& spriteSize, const spritePosition& spritePosition, const std::vector<std::shared_ptr<ISpriteVoxel>>& voxels);
	spriteSize getSize() const override { return _size; };
	spritePosition getPosition() const override { return _position; }
	void setPosition(spritePosition newPos) override { _position = newPos; }
	bool tick(ISpriteManager& spriteManager, float timeDelta) override;
	void render(const ICamera& camera, const ILightSource& light) const override;
	void rebuildGeometry() override;
	void prepareRenderer() override;
	std::shared_ptr<IVoxel> VoxelSprite::getVoxel(unsigned int x, unsigned int y, unsigned int z) const override;
	void VoxelSprite::setVoxel(unsigned int x, unsigned int y, unsigned int z, const std::shared_ptr<ISpriteVoxel>& voxel);
	unsigned getContainerWidthInVoxels() const override { return _size.width; }
	unsigned getContainerHeightInVoxels() const override { return _size.height; }
	unsigned getContainerDepthInVoxels() const override { return _size.depth; }
	void forEachVoxel(const forEachVoxelFunction& func, bool includeNull = false) const override;
	void attachSpriteBehaviour(ISpriteManager& spriteManager, const std::shared_ptr<ISpriteBehaviour> behaviour, float executionDeferral = 0.0f) override;
	void detachSpriteBehaviour(ISpriteManager& spriteManager, const std::shared_ptr<ISpriteBehaviour> behaviour) override;
	void forEachBehaviour(const forEachSpriteBehaviourFunction& function) const override;
	void cheapUpdate(const std::shared_ptr<IVoxel>& voxel) const override;
};