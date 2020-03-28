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
		bool isAfk = true;

		// update key list
		for (int i = 0; i < TOTALKEYS; i++)
		{
			if (glfwGetKey(VALID_KEYS[i]) == GLFW_PRESS)
			{
				if (keys[VALID_KEYS[i]] == RELEASED)
					keys[VALID_KEYS[i]] = 0;
				keys[VALID_KEYS[i]] += delta;
				isAfk = false;
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
				isAfk = false;
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

		if (mpos_current != mpos_last)
			isAfk = false;

		if (isAfk)
			timeAfk += delta;
		else
			timeAfk = 0;

		int axisnum = glfwGetJoystickPos(0, controlleraxis, MAXCONTROLLERAXIS);

		unsigned char* buttons = new unsigned char[MAXCONTROLLERBUTTONS];
		int buttonnum = glfwGetJoystickButtons(0, buttons, MAXCONTROLLERBUTTONS);
		for (int i = 0; i < buttonnum; i++) {
			if (buttons[i])
				controllerbuttons[i] = (controllerbuttons[i] == RELEASED) ? delta : controllerbuttons[i] + delta;
			else if (controllerbuttons[i] == RELEASED)
				controllerbuttons[i] = 0.0;
			else
				controllerbuttons[i] = RELEASED;
		}
			
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
	bool Input::isKeyHeld(int key, double delay) const
	{
		return keys[key] > delay;
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

	double Input::getTimeAfk()
	{
		return timeAfk;
	}

	bool Input::isAfk(double delay)
	{
		return timeAfk > delay;
	}

	bool Input::isControllerButtonPressed(int button)
	{
		return controllerbuttons[button] == delta;
	}

	bool Input::isControllerButtonHeld(int button)
	{
		return controllerbuttons[button] > 0 && controllerbuttons[button] != RELEASED;
	}

	bool Input::isControllerButtonHeld(int button, double delay)
	{
		return controllerbuttons[button] > delay;
	}

	bool Input::isControllerButtonReleased(int button)
	{
		return controllerbuttons[button] == RELEASED;
	}

	bool Input::isControllerButtonUnheld(int button)
	{
		return controllerbuttons[button] == 0;
	}

	float Input::getControllerAxis(int axis)
	{
		return controlleraxis[axis];
	}

	glm::vec2 Input::getControllerLeftStick()
	{
		return glm::vec2(controlleraxis[INPUT_CONTROLLER_AXIS_LH], controlleraxis[INPUT_CONTROLLER_AXIS_LV]);
	}

	glm::vec2 Input::getControllerRightStick()
	{
		return glm::vec2(controlleraxis[INPUT_CONTROLLER_AXIS_RH], controlleraxis[INPUT_CONTROLLER_AXIS_RV]);
	}

	glm::vec2 Input::getControllerTriggers()
	{
		float value = controlleraxis[INPUT_CONTROLLER_AXIS_TRIGGER];
		float left = (value < 0) ? -value : 0;
		float right = (value > 0) ? value : 0;
		return glm::vec2(left, right);
	}
}