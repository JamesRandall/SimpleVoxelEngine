#pragma once
#include <stdexcept>
class VoxelEngineException : public std::logic_error
{
public:
	VoxelEngineException(std::string msg) : std::logic_error(msg) { }
};