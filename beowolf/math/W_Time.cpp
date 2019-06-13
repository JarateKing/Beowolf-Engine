#include "W_Time.h"
#include <GL/glfw.h>

namespace wolf
{
	const double THRESHOLD = 5.0;

	void Time::update()
	{
		lastTime = currentTime;
		currentTime = glfwGetTime();

		// prevent loss of precision
		if (lastTime > THRESHOLD)
		{
			glfwSetTime(currentTime - THRESHOLD);
			lastTime -= THRESHOLD;
			currentTime -= THRESHOLD;
		}
	}

	double Time::deltaTime()
	{
		return (currentTime - lastTime);
	}
}