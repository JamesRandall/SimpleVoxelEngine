#pragma once
#include "ISprite.h"
#include <vector>
#include <memory>
#include "ISpriteManager.h"
#include <mutex>
#include <future>

class ISpriteBehaviour;

class SpriteManager : public ISpriteManager
{
	std::vector<std::shared_ptr<ISprite>> _sprites;
	std::vector<std::shared_ptr<ISprite>> _spritesRequiringRebuild;

	void rebuildGeometry();
public:
	SpriteManager();
	~SpriteManager();

	void addSprite(const std::shared_ptr<ISprite>& sprite) override { _sprites.push_back(sprite); _spritesRequiringRebuild.push_back(sprite); }
	void attachSpriteBehaviour(const std::shared_ptr<ISprite>& sprite, const std::shared_ptr<ISpriteBehaviour>& behaviour, float executionDeferral = 0.0f) override { sprite->attachSpriteBehaviour(*this, behaviour, executionDeferral); }
	void detachSpriteBehaviour(const std::shared_ptr<ISprite>& sprite, const std::shared_ptr<ISpriteBehaviour>& behaviour) override { sprite->detachSpriteBehaviour(*this, behaviour); }
	bool tick(float timeDelta);
	void render(const ICamera& camera, const ILightSource& light) const;
	void requestUpdate(const spriteUpdateRequest& request) override;
};

