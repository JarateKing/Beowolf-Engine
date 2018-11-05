#include "W_Input.h"
#include <GL/glfw.h>
#include <cmath>
#include <iostream>

namespace wolf
{
	void Input::update()
	{
		// update key list
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

		// update mbutton list
		for (int i = 0; i < TOTALMBUTTONS; i++)
		{
			if (glfwGetMouseButton(i) == GLFW_PRESS)
			{
				if (mbuttons[i] == RELEASED)
					mbuttons[i] = 0;
				mbuttons[i]++;
			}
			else
			{
				if (mbuttons[i] == RELEASED)
					mbuttons[i] = 0;
				else if (mbuttons[i] > 0)
					mbuttons[i] = RELEASED;
			}
		}

		printf("%d\n", mbuttons[1]);

		// update current mpos
		mpos_last.x = mpos_current.x;
		mpos_last.y = mpos_current.y;

		int rawx, rawy;
		glfwGetMousePos(&rawx, &rawy);

		mpos_current.x = rawx + 1;
		mpos_current.y = rawy + 1;
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

	int Input::getNewerKey(int key1, int key2)
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

	int Input::getOlderKey(int key1, int key2)
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

	MousePos Input::getMousePos()
	{
		return mpos_current;
	}

	MousePos Input::getMouseDelta()
	{
		return mpos_current - mpos_last;
	}

	bool Input::isMousePressed(int mbutton)
	{
		return mbuttons[mbutton] == 1;
	}

	bool Input::isMouseHeld(int mbutton)
	{
		return mbuttons[mbutton] > 0 && mbuttons[mbutton] != RELEASED;
	}

	bool Input::isMouseReleased(int mbutton)
	{
		return mbuttons[mbutton] == RELEASED;
	}

	bool Input::isMouseUnheld(int mbutton)
	{
		return mbuttons[mbutton] == 0 || mbuttons[mbutton] == RELEASED;
	}
}