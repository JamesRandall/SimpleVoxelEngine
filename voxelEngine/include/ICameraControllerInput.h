#pragma once

struct controllerDelta
{
	float xDelta;
	float yDelta;
	controllerDelta() : xDelta(0), yDelta(0) { }
	controllerDelta(float x, float y) : xDelta(x), yDelta(y) { }
};

// for the sake of picking a consistent terminology this uses gamepad terms
class ICameraControllerInput
{
protected:
	~ICameraControllerInput() = default;
public:
	virtual controllerDelta getLeftThumbstickDelta() const = 0;
	virtual controllerDelta getRightThumbstickDelta() const = 0;
	virtual void update(float timeDelta) = 0;
};

