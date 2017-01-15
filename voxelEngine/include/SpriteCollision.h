#pragma once
#include <memory>

class ISprite;

class SpriteCollision
{
	std::shared_ptr<ISprite> _spriteOne;
	std::shared_ptr<ISprite> _spriteTwo;
public:
	SpriteCollision(const std::shared_ptr<ISprite>& spriteOne, const std::shared_ptr<ISprite>& spriteTwo)
		: _spriteOne(spriteOne), _spriteTwo(spriteTwo) { }

	std::shared_ptr<ISprite> getSpriteOne() const { return _spriteOne; }
	std::shared_ptr<ISprite> getSpriteTwo() const { return _spriteTwo; }
};
