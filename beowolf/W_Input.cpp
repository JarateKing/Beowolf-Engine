#include "W_Input.h"
#include <GL/glfw.h>
#include <cmath>

namespace wolf
{
	void Input::update()
	{
		for (int i = 0; i < TOTALKEYS; i++)
		{
			if (glfwGetKey(VALID_KEYS[i]) == GLFW_PRESS)
			{
				if (keys[VALID_KEYS[i]] == RELEASED)
					keys[VALID_KEYS[i]] = 0;
				keys[VALID_KEYS[i]]++;
			}
			else
			{
				if (keys[VALID_KEYS[i]] == RELEASED)
					keys[VALID_KEYS[i]] = 0;
				else if (keys[VALID_KEYS[i]] > 0)
					keys[VALID_KEYS[i]] = RELEASED;
			}
		}
	}

	int Input::getKey(int key)
	{
		return keys[key];
	}

	bool Input::isKeyPressed(int key)
	{
		return keys[key] == 1;
	}

	bool Input::isKeyHeld(int key)
	{
		return keys[key] > 0 && keys[key] != RELEASED;
	}

	bool Input::isKeyReleased(int key)
	{
		return keys[key] == RELEASED;
	}

	bool Input::isKeyUnheld(int key)
	{
		return keys[key] == 0 || keys[key] == RELEASED;
	}

	int Input::getNewer(int key1, int key2)
	{
		bool key1unheld = isKeyUnheld(key1);
		bool key2unheld = isKeyUnheld(key2);

		if (key1unheld && key2unheld)
			return -1;
		else if (key1unheld)
			return key2;
		else if (key2unheld)
			return key1;
		else if (keys[key1] <= keys[key2])
			return key1;
		else
			return key2;
	}

	int Input::getOlder(int key1, int key2)
	{
		bool key1unheld = isKeyUnheld(key1);
		bool key2unheld = isKeyUnheld(key2);

		if (key1unheld && key2unheld)
			return -1;
		else if (key1unheld)
			return key2;
		else if (key2unheld)
			return key1;
		else if (keys[key1] >= keys[key2])
			return key1;
		else
			return key2;
	}
}