#pragma once
#include "ISpriteCollisionSet.h"
#include <unordered_set>

class SpriteCollisionSet : public ISpriteCollisionSet
{
	std::unordered_set<std::shared_ptr<ISprite>> _sprites;

public:
	bool getIsSpriteInSet(const std::shared_ptr<ISprite>& sprite) const override { return _sprites.find(sprite) != _sprites.end(); }
	void addSprite(const std::shared_ptr<ISprite>& sprite) override { _sprites.emplace(sprite); }
	void removeSprite(const std::shared_ptr<ISprite>& sprite) override { _sprites.erase(sprite); }
};
