#pragma once
#include "color.h"

struct lightSourcePosition
{
	float x;
	float y;
	float z;

	lightSourcePosition(float x, float y, float z) : x(x), y(y), z(z) {}
};


class ILightSource
{
public:
	virtual ~ILightSource() = default;
	virtual lightSourcePosition getPosition() const = 0;
	virtual float getPower() const = 0;
	virtual color getColor() const = 0;
};