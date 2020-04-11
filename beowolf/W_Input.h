#ifndef W_INPUT_H
#define W_INPUT_H

#include "W_Input_Keys.h"
#include "W_Common.h"

namespace wolf
{
	struct MousePos
	{
		// raw position
		int x, y;

		// constructor
		MousePos(int x = 0, int y = 0) : x(x), y(y) {}

		// get percent relative to thing
		glm::vec2 relative(const glm::vec2& size) const
		{
			return glm::vec2(size.x / x, size.y / y);
		}
		glm::vec2 relative(double width, double height) const
		{
			return glm::vec2(width / x, height / y);
		}
		glm::vec2 relative(const glm::vec2& size, const glm::vec2& offset) const
		{
			return glm::vec2(size.x / x - offset.x, size.y / y - offset.y);
		}
		glm::vec2 relative(double width, double height, double horizontalOffset, double verticalOffset) const
		{
			return glm::vec2(width / x - horizontalOffset, height / y - verticalOffset);
		}

		// operators
		bool operator==(const MousePos& input) const
		{
			return (x == input.x) && (y == input.y);
		}
		bool operator!=(const MousePos& input) const
		{
			return !(*this == input);
		}
		MousePos operator+(const MousePos& input) const
		{
			return MousePos(x + input.x, y + input.y);
		}
		MousePos operator-(const MousePos& input) const
		{
			return MousePos(x - input.x, y - input.y);
		}
		MousePos operator*(const MousePos& input) const
		{
			return MousePos(x * input.x, y * input.y);
		}
		MousePos operator/(const MousePos& input) const
		{
			return MousePos(x / input.x, y / input.y);
		}
	};

	class Input
	{
	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		// singleton pattern get instance
		static Input& Instance()
		{
			static Input m_instance;
			return m_instance;
		}
		// remove methods that break the singleton pattern
		Input(Input const&) = delete;
		void operator=(Input const&) = delete;

		void update();

		// keyboard input
		double getKey(int key) const;
		bool isKeyPressed(int key) const;
		bool isKeyHeld(int key) const;
		bool isKeyHeld(int key, double delay) const;
		bool isKeyReleased(int key) const;
		bool isKeyUnheld(int key) const;
		int getNewerKey(int key1, int key2) const;
		int getOlderKey(int key1, int key2) const;

		// mouse input
		MousePos getMousePos() const;
		MousePos getMouseDelta() const;
		
		bool isMousePressed(int mbutton) const;
		bool isMouseHeld(int mbutton) const;
		bool isMouseReleased(int mbutton) const;
		bool isMouseUnheld(int mbutton) const;

		// controller input
		bool isControllerButtonPressed(int button) const;
		bool isControllerButtonHeld(int button) const;
		bool isControllerButtonHeld(int button, double delay) const;
		bool isControllerButtonReleased(int button) const;
		bool isControllerButtonUnheld(int button) const;

		// controller axis
		float getControllerAxis(int axis) const;
		glm::vec2 getControllerLeftStick() const;
		glm::vec2 getControllerRightStick() const;
		glm::vec2 getControllerTriggers() const;

		// combined input
		double getTimeAfk() const;
		bool isAfk(double delay) const;

	private:
		//-------------------------------------------------------------------------
		// PRIVATE METHODS
		//-------------------------------------------------------------------------
		// private constructor
		Input() {}

		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		const double RELEASED = -1.0;
		static const int TOTALKEYS = 120;
		static const int MAXKEY = 349;
		static const int TOTALMBUTTONS = 5;
		const int VALID_KEYS[TOTALKEYS] = { 32, 39, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 59, 61, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 96, 161, 162, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 280, 281, 282, 283, 284, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 340, 341, 342, 343, 344, 345, 346, 347, 348 };
		
		// keep track of duration in seconds they were held down for
		// to compare with other keys and see which was pressed first
		// double will run out after a decade or so of being held down
		double delta = 0;
		double keys[MAXKEY] = { 0 };
		double mbuttons[TOTALMBUTTONS] = { 0 };

		// controller stuff
		static const int MAXCONTROLLERAXIS = 6;
		static const int MAXCONTROLLERBUTTONS = 10;
		float controlleraxis[MAXCONTROLLERAXIS] = { 0 };
		double controllerbuttons[MAXCONTROLLERBUTTONS] = { 0 };

		// time afk
		double timeAfk;

		// check for mouse position
		MousePos mpos_current;
		MousePos mpos_last;
	};
}

#endif