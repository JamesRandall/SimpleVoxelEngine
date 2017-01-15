#include "PlayerMovementBehaviour.h"
#include "ISprite.h"

PlayerMovementBehaviour::PlayerMovementBehaviour(float timeUntilMove) : _isLeftDown(false), _isRightDown(false), _lastTime(0), _timeUntilMove(timeUntilMove)
{
}


PlayerMovementBehaviour::~PlayerMovementBehaviour()
{
}

void PlayerMovementBehaviour::attach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite)
{
}

void PlayerMovementBehaviour::detach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite)
{
}

bool PlayerMovementBehaviour::tick(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite, float timeDelta)
{
	_lastTime += timeDelta;
	if (_lastTime < _timeUntilMove) return false;

	float velocity = 0;
	if (_isLeftDown)
	{
		velocity += 1;
	}
	if (_isRightDown)
	{
		velocity -= 1;
	}

	auto x = static_cast<int>(sprite->getPosition().x);
	while(_lastTime >= _timeUntilMove)
	{
		x += velocity;
		if (x < 0) x = 0;

		_lastTime -= _timeUntilMove;
	}

	sprite->setPosition(spriteVec3(static_cast<unsigned>(x), sprite->getPosition().y, sprite->getPosition().z));

	return false;
}
