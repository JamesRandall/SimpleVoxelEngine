#pragma once
#include "ISpriteBehaviour.h"
#include <memory>

class ISpriteManager;

class VoxelatedConstructionSpriteBehaviour : public ISpriteBehaviour
{
	unsigned _x, _y, _z;
	float _lastUpdate;
	bool _done;
	bool _xFirst;
	float _buildInterval;

public:
	VoxelatedConstructionSpriteBehaviour(bool xFirst, float buildInterval);
	virtual ~VoxelatedConstructionSpriteBehaviour();

	void attach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite) override;
	void detach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite) override;
	bool tick(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite, float timeDelta) override;
};

