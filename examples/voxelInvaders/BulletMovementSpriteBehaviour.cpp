#include "BulletMovementSpriteBehaviour.h"
#include "ISprite.h"


BulletMovementSpriteBehaviour::BulletMovementSpriteBehaviour(float timeUntilMove) : _lastTime(0), _timeUntilMove(timeUntilMove)
{

}


BulletMovementSpriteBehaviour::~BulletMovementSpriteBehaviour()
{
}

bool BulletMovementSpriteBehaviour::tick(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite, float timeDelta)
{
	bool shouldDetach = false;

	_lastTime += timeDelta;
	while (_lastTime > _timeUntilMove)
	{
		sprite->setPosition(spriteVec3(sprite->getPosition().x, sprite->getPosition().y + 1, sprite ->getPosition().z));
		_lastTime -= _timeUntilMove;
	}

	if (sprite->getPosition().y > 250)
	{
		shouldDetach = true;
	}

	return shouldDetach;
}
