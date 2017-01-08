#pragma once
#include "ISpriteBehaviour.h"
#include <memory>

class BehaviourFactory
{
public:
	static std::shared_ptr<ISpriteBehaviour> GetVoxelatedConstructionBehaviour(bool xFirst = true, float buildInterval = 0.006f);
};

