#pragma once
#include "ILightSource.h"

class SimpleLight : public ILightSource
{
	lightSourcePosition _position;
	color _color;
	float _power;
public:
	SimpleLight(const lightSourcePosition& position, const color& color, float power) : _position(position), _color(color), _power(power) { }
	virtual ~SimpleLight() override { };
	lightSourcePosition getPosition() const override { return _position; }
	float getPower() const override { return _power;  }
	color getColor() const override { return _color; }
};
