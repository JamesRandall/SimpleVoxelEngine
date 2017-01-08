#pragma once
#include <memory>
class ISprite;
class ISpriteManager;

class ISpriteBehaviour : public std::enable_shared_from_this<ISpriteBehaviour>
{
protected:
	~ISpriteBehaviour() = default;
public:
	virtual void attach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite) = 0;
	virtual void detach(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite) = 0;
	// should return true if the behaviour needs detaching, behaviours may run in parallel in a threaded fashion and
	// so should not attempt to detach themselves
	virtual bool tick(ISpriteManager& spriteManager, const std::shared_ptr<ISprite>& sprite, float timeDelta)=0; 
};

