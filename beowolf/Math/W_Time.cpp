#include "W_Time.h"
#include <GL/glfw.h>

namespace wolf
{
	void Time::update()
	{
		lastTime = currentTime;
		currentTime = glfwGetTime();
	}

	double Time::deltaTime()
	{
		return (currentTime - lastTime);
	}
}