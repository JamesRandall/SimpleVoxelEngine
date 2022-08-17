#pragma once
#include <memory>
#include <functional>
#include <string>
#include "ISpriteVoxel.h"
#include "AABB.h"
#include <vector>
#include "IAABB.h"
#include <glm/vec3.hpp>
#include <glm/detail/type_vec3.hpp>

class ISpriteBehaviour;
class ISpriteManager;

struct spriteSize {
	unsigned width;
	unsigned height;
	unsigned depth;

	spriteSize(unsigned width, unsigned height, unsigned depth) : width(width), height(height), depth(depth) { }
};

struct spriteVec3
{
	unsigned x;
	unsigned y;
	unsigned z;

	spriteVec3(unsigned x, unsigned y, unsigned z) : x(x), y(y), z(z) { }
};

struct spriteVelocity
{
	float x;
	float y;
	float z;

	spriteVelocity(float x, float y, float z) : x(x), y(y), z(z) { }
};

class ICamera;
class ILightSource;

typedef long spriteCollisionBitMask;

typedef std::function<void(const std::shared_ptr<ISpriteBehaviour>)> forEachSpriteBehaviourFunction;
class ISprite : public IAABB
{
public:
	virtual ~ISprite() = default;
	virtual spriteVec3 getPosition() const = 0;
	virtual void setPosition(spriteVec3 position) = 0;
	virtual glm::vec3 getVelocity() const = 0;
	virtual void setVelocity(glm::vec3 velocity) = 0;
	virtual spriteSize getSize() const = 0;
	virtual bool tick(ISpriteManager& spriteManager, float timeDelta) = 0; // should return true if geometry needs updating
	virtual void render(const ICamera& camera, const ILightSource& light) const = 0;
	virtual void rebuildGeometry() = 0;
	virtual void prepareRenderer() = 0;
	virtual void attachSpriteBehaviour(ISpriteManager& spriteManager, const std::shared_ptr<ISpriteBehaviour> behaviour, float executionDeferral=0.0f)=0;
	virtual void detachSpriteBehaviour(ISpriteManager& spriteManager, const std::shared_ptr<ISpriteBehaviour> behaviour)=0;
	virtual void forEachBehaviour(const forEachSpriteBehaviourFunction& function) const = 0;
	virtual void cheapUpdate(const std::shared_ptr<IVoxel>& voxel) const = 0;
	virtual unsigned getType() const = 0;
	virtual void setType(unsigned type) = 0;
	virtual bool collidesWith(const ISprite& sprite) const = 0;
	
	static std::shared_ptr<ISprite> load(std::string spriteFile, unsigned type=0);
	static std::shared_ptr<ISprite> load(const std::vector<std::string>& spriteFrameFilenames, float timeBetweenFrames, float frameChangeDeferral=0.0f, unsigned type=0);
};
