#pragma once
#include "ISpriteBehaviour.h"

class InvaderMovementSpriteBehaviour : public ISpriteBehaviour
{
	unsigned _offset;
	unsigned _maxOffset;
	float _lastTime;
	float _timeUntilMove;
	int _direction;
	unsigned _startingX;
	unsigned _y;
	unsigned _startingZ;

public:
	InvaderMovementSpriteBehaviour(float timeUntilMove, unsigned maxOffset, unsigned initialOffset=0);
	virtual ~InvaderMovementSpriteBehaviour();
	void attach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite) override;
	void detach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite) override;
	bool tick(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite, float timeDelta) override;
};

