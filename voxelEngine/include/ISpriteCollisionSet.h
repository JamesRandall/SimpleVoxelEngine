#pragma once
#include <memory>
#include "ISprite.h"

class ISpriteCollisionSet
{
public:
	virtual ~ISpriteCollisionSet() = default;
	virtual bool getIsSpriteInSet(const std::shared_ptr<ISprite>& sprite) const = 0;
	virtual void addSprite(const std::shared_ptr<ISprite>& sprite) = 0;
	virtual void removeSprite(const std::shared_ptr<ISprite>& sprite) = 0;
};
