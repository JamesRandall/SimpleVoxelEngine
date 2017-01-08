#include "BehaviourFactory.h"
#include "ISpriteBehaviour.h"
#include "behaviours/VoxelatedConstructionSpriteBehaviour.h"

std::shared_ptr<ISpriteBehaviour> BehaviourFactory::GetVoxelatedConstructionBehaviour(bool xFirst, float buildInterval)
{
	return std::make_shared<VoxelatedConstructionSpriteBehaviour>(xFirst, buildInterval);
}
