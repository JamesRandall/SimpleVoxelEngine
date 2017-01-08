#pragma once
#include "ISpriteBehaviour.h"
#include <memory>

enum class spriteUpdateRequestTypeEnum
{
	full,
	cheap
};
struct spriteUpdateRequest
{
	std::shared_ptr<std::vector<std::shared_ptr<IVoxel>>> voxels;
	std::shared_ptr<ISprite> sprite;
	spriteUpdateRequestTypeEnum type;

	spriteUpdateRequest() : voxels(nullptr), sprite(nullptr), type(spriteUpdateRequestTypeEnum::full) { }
	spriteUpdateRequest(std::shared_ptr<ISprite> sprite, std::shared_ptr<std::vector<std::shared_ptr<IVoxel>>> voxels, spriteUpdateRequestTypeEnum type) : voxels(voxels), sprite(sprite), type(type) { }
};


class ISpriteManager
{
public:
	virtual ~ISpriteManager() = default;
	virtual void addSprite(const std::shared_ptr<ISprite>& sprite) = 0;
	virtual void attachSpriteBehaviour(const std::shared_ptr<ISprite>& sprite, const std::shared_ptr<ISpriteBehaviour>& behaviour, float executionDeferral=0.0f) = 0;
	virtual void detachSpriteBehaviour(const std::shared_ptr<ISprite>& sprite, const std::shared_ptr<ISpriteBehaviour>& behaviour) = 0;
	virtual void requestUpdate(const spriteUpdateRequest& request)  = 0;
};
