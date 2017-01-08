#pragma once
#include <memory>
#include <functional>
#include <string>
#include "ISpriteVoxel.h"

class ISpriteBehaviour;
class ISpriteManager;

struct spriteSize {
	unsigned width;
	unsigned height;
	unsigned depth;

	spriteSize(unsigned width, unsigned height, unsigned depth) : width(width), height(height), depth(depth) { }
};

struct spritePosition
{
	unsigned x;
	unsigned y;
	unsigned z;

	spritePosition(unsigned x, unsigned y, unsigned z) : x(x), y(y), z(z) { }
};

class ICamera;
class ILightSource;

typedef std::function<void(const std::shared_ptr<ISpriteBehaviour>)> forEachSpriteBehaviourFunction;
class ISprite
{
public:
	virtual ~ISprite() = default;
	virtual spritePosition getPosition() const = 0;
	virtual void setPosition(spritePosition position) = 0;
	virtual spriteSize getSize() const = 0;
	virtual bool tick(ISpriteManager& spriteManager, float timeDelta) = 0; // should return true if geometry needs updating
	virtual void render(const ICamera& camera, const ILightSource& light) const = 0;
	virtual void rebuildGeometry() = 0;
	virtual void prepareRenderer() = 0;
	virtual void attachSpriteBehaviour(ISpriteManager& spriteManager, const std::shared_ptr<ISpriteBehaviour> behaviour, float executionDeferral=0.0f)=0;
	virtual void detachSpriteBehaviour(ISpriteManager& spriteManager, const std::shared_ptr<ISpriteBehaviour> behaviour)=0;
	virtual void forEachBehaviour(const forEachSpriteBehaviourFunction& function) const = 0;
	virtual void cheapUpdate(const std::shared_ptr<IVoxel>& voxel) const = 0;

	static std::shared_ptr<ISprite> load(std::string spriteFile);
};
