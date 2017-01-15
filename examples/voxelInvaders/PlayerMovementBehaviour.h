#pragma once
#include "ISpriteBehaviour.h"

class PlayerMovementBehaviour : public ISpriteBehaviour
{
	bool _isLeftDown;
	bool _isRightDown;
	float _lastTime;
	float _timeUntilMove;
public:
	PlayerMovementBehaviour(float timeUntilMove);
	virtual ~PlayerMovementBehaviour();
	void attach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite) override;
	void detach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite) override;
	bool tick(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite, float timeDelta) override;

	void setIsLeftDown(bool isDown) { _isLeftDown = isDown; }
	void setIsRightDown(bool isDown) { _isRightDown = isDown; }
};

