#pragma once
#include "ISprite.h"
#include <vector>
#include <memory>
#include "ISpriteManager.h"
#include "AABBTree.h"
#include <forward_list>
#include <list>

class ISpriteBehaviour;
class ISpriteCollisionSet;
class SpriteCollision;
class AABBTree;

class SpriteManager : public ISpriteManager
{
	std::list<std::shared_ptr<ISprite>> _sprites;
	std::vector<std::shared_ptr<ISprite>> _spritesRequiringRebuild;
	std::vector<std::shared_ptr<ISprite>> _spritesRequiringCollisionUpdate;
	std::forward_list<std::shared_ptr<ISprite>> _collidingSprites;
	unsigned _numberOfCollidingSprites;
	AABBTree _collisionTree;

	void rebuildGeometry();
	std::shared_ptr<std::forward_list<SpriteCollision>> findBroadRangeCollisions(const std::forward_list<std::shared_ptr<ISprite>>& sprites) const;
public:
	SpriteManager();
	~SpriteManager();

	void addSprite(const std::shared_ptr<ISprite>& sprite) override { _sprites.push_back(sprite); _spritesRequiringRebuild.push_back(sprite); }
	void removeSprite(const std::shared_ptr<ISprite>& sprite) override;
	void attachSpriteBehaviour(const std::shared_ptr<ISprite>& sprite, const std::shared_ptr<ISpriteBehaviour>& behaviour, float executionDeferral = 0.0f) override { sprite->attachSpriteBehaviour(*this, behaviour, executionDeferral); }
	void detachSpriteBehaviour(const std::shared_ptr<ISprite>& sprite, const std::shared_ptr<ISpriteBehaviour>& behaviour) override { sprite->detachSpriteBehaviour(*this, behaviour); }
	bool tick(float timeDelta);
	void tickComplete(float timeDelta);
	void render(const ICamera& camera, const ILightSource& light) const;
	void requestUpdate(const spriteUpdateRequest& request) override;
	void addSpriteToCollisionSet(const std::shared_ptr<ISprite>& sprite) override;
	void markSpriteForCollisionSetUpdate(const std::shared_ptr<ISprite>& sprite) override;
	std::shared_ptr<std::forward_list<SpriteCollision>> findAllCollisions() const override;
	std::shared_ptr<std::forward_list<SpriteCollision>> findCollisionsForSprite(const std::shared_ptr<ISprite>& sprite) const override;
	std::shared_ptr<std::forward_list<SpriteCollision>> findCollisionsForSprites(const std::forward_list<std::shared_ptr<ISprite>>& sprites) const override;
};

