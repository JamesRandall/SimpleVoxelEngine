#pragma once
#include "ISpriteBehaviour.h"
#include <memory>
#include "SpriteCollision.h"
#include <forward_list>

class ISpriteCollisionSet;

enum class spriteUpdateRequestTypeEnum
{
	full,
	cheap
};
struct spriteUpdateRequest
{
	std::shared_ptr<std::vector<std::shared_ptr<IVoxel>>> voxels;
	std::shared_ptr<ISprite> sprite;
	spriteUpdateRequestTypeEnum type;

	spriteUpdateRequest() : voxels(nullptr), sprite(nullptr), type(spriteUpdateRequestTypeEnum::full) { }
	spriteUpdateRequest(std::shared_ptr<ISprite> sprite, std::shared_ptr<std::vector<std::shared_ptr<IVoxel>>> voxels, spriteUpdateRequestTypeEnum type) : voxels(voxels), sprite(sprite), type(type) { }
};


class ISpriteManager
{
public:
	virtual ~ISpriteManager() = default;
	virtual void addSprite(const std::shared_ptr<ISprite>& sprite) = 0;
	virtual void removeSprite(const std::shared_ptr<ISprite>& sprite) = 0;
	virtual void attachSpriteBehaviour(const std::shared_ptr<ISprite>& sprite, const std::shared_ptr<ISpriteBehaviour>& behaviour, float executionDeferral=0.0f) = 0;
	virtual void detachSpriteBehaviour(const std::shared_ptr<ISprite>& sprite, const std::shared_ptr<ISpriteBehaviour>& behaviour) = 0;
	virtual void requestUpdate(const spriteUpdateRequest& request)  = 0;
	virtual std::shared_ptr<std::forward_list<SpriteCollision>> findAllCollisions() const = 0;
	virtual std::shared_ptr<std::forward_list<SpriteCollision>> findCollisionsForSprite(const std::shared_ptr<ISprite>& sprite) const = 0;
	virtual std::shared_ptr<std::forward_list<SpriteCollision>> findCollisionsForSprites(const std::forward_list<std::shared_ptr<ISprite>>& sprites) const = 0;
	virtual void addSpriteToCollisionSet(const std::shared_ptr<ISprite>& sprite) = 0;
	virtual void markSpriteForCollisionSetUpdate(const std::shared_ptr<ISprite>& sprite) = 0;
	//virtual void addSpriteToCollisionSet(const std::shared_ptr<ISprite>& sprite, std::shared_ptr<ISpriteCollisionSet> collisionSet);
};

