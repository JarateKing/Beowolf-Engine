#include "W_Time.h"
#include <GL/glfw.h>

namespace wolf
{
	const double THRESHOLD = 60.0;
	const int HISTORY_SIZE = 50;
	const double FPS_THRESHOLD = 0.125;

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
			if (++timeCount >= HISTORY_SIZE)
				timeCount = 0;
		}

		timeSinceDisplayUpdate += deltaTime();
		if (timeSinceDisplayUpdate >= FPS_THRESHOLD) {
			timeSinceDisplayUpdate -= FPS_THRESHOLD;
			timeToDisplay = timeAvg;
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
		return 1.0 / timeToDisplay;
	}
}