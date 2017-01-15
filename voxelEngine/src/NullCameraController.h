#pragma once
#include "ICameraControllerInput.h"

class NullCameraController : public ICameraControllerInput
{
public:
	NullCameraController() { }
	virtual ~NullCameraController() { }
	controllerDelta getLeftThumbstickDelta() const override { return controllerDelta(0, 0); }
	controllerDelta getRightThumbstickDelta() const override { return controllerDelta(0, 0); }
	void update(float timeDelta) override { }
};

