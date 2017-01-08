#pragma once
#include <glfw/glfw3.h>
#include <cstdio>

class FrameCounter
{
	double _lastTime;
	unsigned _frames;

public:
	FrameCounter() : _lastTime(glfwGetTime()), _frames(0)
	{
		
	}

	void tick()
	{
		double fpsCounterCurrentTime = glfwGetTime();
		_frames++;
		if (fpsCounterCurrentTime - _lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
																 // printf and reset timer
			printf("%f ms/frame\n", 1000.0 / double(_frames));
			_frames = 0;
			_lastTime += 1.0;
		}
	}
};
