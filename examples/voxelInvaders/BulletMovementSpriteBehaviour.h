#pragma once
#include "ISpriteBehaviour.h"

class BulletMovementSpriteBehaviour : public ISpriteBehaviour
{
	float _lastTime;
	float _timeUntilMove;
public:
	BulletMovementSpriteBehaviour(float timeUntilMove);
	virtual ~BulletMovementSpriteBehaviour();
	void attach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite) override { };
	void detach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite) override { };
	bool tick(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite, float timeDelta) override;
};

