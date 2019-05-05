#include "W_Input.h"
#include <GL/glfw.h>
#include <cmath>
#include <iostream>
#include "W_Time.h"

namespace wolf
{
	void Input::update()
	{
		delta = Time::Instance().deltaTime();

		// update key list
		for (int i = 0; i < TOTALKEYS; i++)
		{
			if (glfwGetKey(VALID_KEYS[i]) == GLFW_PRESS)
			{
				if (keys[VALID_KEYS[i]] == RELEASED)
					keys[VALID_KEYS[i]] = 0;
				keys[VALID_KEYS[i]] += delta;
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
				mbuttons[i] += delta;
			}
			else
			{
				if (mbuttons[i] == RELEASED)
					mbuttons[i] = 0;
				else if (mbuttons[i] > 0)
					mbuttons[i] = RELEASED;
			}
		}

		// update current mpos
		mpos_last.x = mpos_current.x;
		mpos_last.y = mpos_current.y;

		int rawx, rawy;
		glfwGetMousePos(&rawx, &rawy);

		mpos_current.x = rawx + 1;
		mpos_current.y = rawy + 1;
	}

	double Input::getKey(int key) const
	{
		return keys[key];
	}

	bool Input::isKeyPressed(int key) const
	{
		return keys[key] == delta;
	}

	bool Input::isKeyHeld(int key) const
	{
		return keys[key] > 0 && keys[key] != RELEASED;
	}

	bool Input::isKeyReleased(int key) const
	{
		return keys[key] == RELEASED;
	}

	bool Input::isKeyUnheld(int key) const
	{
		return keys[key] == 0 || keys[key] == RELEASED;
	}

	int Input::getNewerKey(int key1, int key2) const
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

	int Input::getOlderKey(int key1, int key2) const
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

	MousePos Input::getMousePos() const
	{
		return mpos_current;
	}

	MousePos Input::getMouseDelta() const
	{
		return mpos_current - mpos_last;
	}

	bool Input::isMousePressed(int mbutton) const
	{
		return mbuttons[mbutton] == delta;
	}

	bool Input::isMouseHeld(int mbutton) const
	{
		return mbuttons[mbutton] > 0 && mbuttons[mbutton] != RELEASED;
	}

	bool Input::isMouseReleased(int mbutton) const
	{
		return mbuttons[mbutton] == RELEASED;
	}

	bool Input::isMouseUnheld(int mbutton) const
	{
		return mbuttons[mbutton] == 0 || mbuttons[mbutton] == RELEASED;
	}
}