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

	class VoxelSpriteFrame
	{
	public:
		VoxelSpriteFrame() { }
		VoxelSpriteFrame(const std::vector<std::shared_ptr<ISpriteVoxel>>& voxels) : voxels(voxels) { }
		std::vector<std::shared_ptr<ISpriteVoxel>> voxels;
		std::shared_ptr<VoxelContainerGeometry> geometry;
		std::shared_ptr<VoxelRenderer> renderer;
	};
	typedef std::shared_ptr<VoxelSpriteFrame> VoxelSpriteFramePtr;
	
	int _frameIndex;
	float _timeBetweenFrames;
	float _frameUpdateTime;
	std::vector<VoxelSpriteFramePtr> _frames;		
	spriteSize _size;
	spriteVec3 _position;
	glm::vec3 _velocity;
	AABB _aabb;
	std::forward_list<std::shared_ptr<DeferredSpriteBehaviourDecorator>> _behaviours;
	unsigned _type;
	glm::vec3 _cumulativeVelocity; // float based variant of the sprites position, we use this to track velocity changes

	unsigned int getVoxelIndex(unsigned int x, unsigned int y, unsigned int z) const { return z * _size.height * _size.width + y * _size.width + x; }
	VoxelSpriteFramePtr getCurrentFrame() const { return _frames[_frameIndex]; }

public:
	VoxelSprite(const spriteSize& spriteSize, const spriteVec3& spritePosition);
	VoxelSprite(const spriteSize& spriteSize, const spriteVec3& spritePosition, const std::vector<std::shared_ptr<ISpriteVoxel>>& voxels);
	VoxelSprite(const spriteSize& spriteSize, const spriteVec3& spritePosition, const std::vector<std::vector<std::shared_ptr<ISpriteVoxel>>>& frameVoxels, float timeBetweenFrames, float frameChangeDeferral=0.0f);
	spriteSize getSize() const override { return _size; };
	spriteVec3 getPosition() const override { return _position; }	
	void setPosition(spriteVec3 newPos) override
	{
		_position = newPos;
		_aabb = AABB(_position.x, _position.y, _position.z, _position.x + _size.width, _position.y + _size.height, _position.z + _size.depth);
	}
	glm::vec3 getVelocity() const override { return _velocity; }
	void setVelocity(glm::vec3 velocity) override { _velocity = velocity; }
	bool tick(ISpriteManager& spriteManager, float timeDelta) override;
	void render(const ICamera& camera, const ILightSource& light) const override;
	void rebuildGeometry() override;
	void prepareRenderer() override;
	std::shared_ptr<IVoxel> getVoxel(unsigned int x, unsigned int y, unsigned int z) const override;
	void setVoxel(unsigned int x, unsigned int y, unsigned int z, const std::shared_ptr<ISpriteVoxel>& voxel) const;
	unsigned getContainerWidthInVoxels() const override { return _size.width; }
	unsigned getContainerHeightInVoxels() const override { return _size.height; }
	unsigned getContainerDepthInVoxels() const override { return _size.depth; }
	void forEachVoxel(const forEachVoxelFunction& func, bool includeNull = false) const override;
	void attachSpriteBehaviour(ISpriteManager& spriteManager, const std::shared_ptr<ISpriteBehaviour> behaviour, float executionDeferral = 0.0f) override;
	void detachSpriteBehaviour(ISpriteManager& spriteManager, const std::shared_ptr<ISpriteBehaviour> behaviour) override;
	void forEachBehaviour(const forEachSpriteBehaviourFunction& function) const override;
	void cheapUpdate(const std::shared_ptr<IVoxel>& voxel) const override;
	unsigned getType() const override { return _type; };
	void setType(unsigned type) override { _type = type; }
	bool collidesWith(const ISprite& sprite) const override;	

	// IAAAB
	AABB getAABB() const override { return _aabb; } // TODO: when we add velocity we will grow the AABB by the velocity to reduce tree modification operations
};
