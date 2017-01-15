#include "InvaderMovementSpriteBehaviour.h"
#include "ISprite.h"
#include "ISpriteManager.h"


InvaderMovementSpriteBehaviour::InvaderMovementSpriteBehaviour(float timeUntilMove, unsigned maxOffset, unsigned initialOffset) : _offset(initialOffset), _maxOffset(maxOffset), _lastTime(0.0f), _timeUntilMove(timeUntilMove), _direction(1), _startingX(0), _y(0), _startingZ(0)
{
}


InvaderMovementSpriteBehaviour::~InvaderMovementSpriteBehaviour()
{
}

void InvaderMovementSpriteBehaviour::attach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite)
{
	_startingX = sprite->getPosition().x;
	_y = sprite->getPosition().y;
	_startingZ = sprite->getPosition().z;
}

void InvaderMovementSpriteBehaviour::detach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite)
{
}

bool InvaderMovementSpriteBehaviour::tick(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite, float timeDelta)
{
	_lastTime += timeDelta;
	if (_lastTime > _timeUntilMove)
	{
		_offset = _offset + _direction;
		if (_offset == 0 || _offset == _maxOffset)
		{
			_direction *= -1;
			_y -= 3;
		}
		sprite->setPosition(spriteVec3(_startingX + _offset, _y, _startingZ));
		_lastTime -= _timeUntilMove;
		spriteManager.markSpriteForCollisionSetUpdate(sprite);
	}	

	return false;
}
