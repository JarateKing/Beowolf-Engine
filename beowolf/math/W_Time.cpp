#include "W_Time.h"
#include <GL/glfw.h>

namespace wolf
{
	const double THRESHOLD = 60.0;
	const int HISTORY_SIZE = 30;

	void Time::update()
	{
		lastTime = currentTime;
		currentTime = glfwGetTime();

		if (times.size() < HISTORY_SIZE) {
			times.push_back(deltaTime());

			double timeSum = 0.0;
			for (int i = 0; i < times.size(); i++) {
				timeSum += times[i];
			}
			timeAvg = timeSum / times.size();
		}
		else {
			timeAvg = timeAvg + (deltaTime() / HISTORY_SIZE) - (times[timeCount] / HISTORY_SIZE);
			times[timeCount] = deltaTime();
			timeCount = (timeCount + 1) % HISTORY_SIZE;
		}

		// prevent loss of precision
		if (lastTime > THRESHOLD)
		{
			glfwSetTime(currentTime - THRESHOLD);
			lastTime -= THRESHOLD;
			currentTime -= THRESHOLD;

			double timeSum = 0.0;
			for (int i = 0; i < times.size(); i++) {
				timeSum += times[i];
			}
			timeAvg = timeSum / times.size();
		}
	}

	double Time::deltaTime()
	{
		return (currentTime - lastTime);
	}

	double Time::getFPS()
	{
		return 1.0 / timeAvg;
	}
}