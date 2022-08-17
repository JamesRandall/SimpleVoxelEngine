#pragma once
#include <GLFW/glfw3.h>
#include "ICameraControllerInput.h"
#include <memory>

class MouseAndKeyboardCameraControllerInput : public ICameraControllerInput
{
	GLFWwindow* _window;
	float _mouseSpeed;
	float _keyboardSpeed;
	bool _firstUpdate;
	float _windowWidth;
	float _windowHeight;

	controllerDelta _leftThumbStickDelta;
	controllerDelta _rightThumbStickDelta;
public:
	MouseAndKeyboardCameraControllerInput(GLFWwindow* window, float mouseSpeed = 0.005f, float keyboardSpeed = 16.0f);
	virtual ~MouseAndKeyboardCameraControllerInput();

	controllerDelta getLeftThumbstickDelta() const override { return _leftThumbStickDelta; }
	controllerDelta getRightThumbstickDelta() const override { return _rightThumbStickDelta; }
	void update(float timeDelta) override;
};

