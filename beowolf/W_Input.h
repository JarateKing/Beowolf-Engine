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
		glm::vec2 relative(glm::vec2 size)
		{
			return glm::vec2(size.x / x, size.y / y);
		}
		glm::vec2 relative(double width, double height)
		{
			return glm::vec2(width / x, height / y);
		}
		glm::vec2 relative(glm::vec2 size, glm::vec2 offset)
		{
			return glm::vec2(size.x / x - offset.x, size.y / y - offset.y);
		}
		glm::vec2 relative(double width, double height, double horizontalOffset, double verticalOffset)
		{
			return glm::vec2(width / x - horizontalOffset, height / y - verticalOffset);
		}

		// operators
		bool operator==(MousePos input)
		{
			return (x == input.x) && (y == input.y);
		}
		MousePos operator+(MousePos input)
		{
			return MousePos(x + input.x, y + input.y);
		}
		MousePos operator-(MousePos input)
		{
			return MousePos(x - input.x, y - input.y);
		}
		MousePos operator*(MousePos input)
		{
			return MousePos(x * input.x, y * input.y);
		}
		MousePos operator/(MousePos input)
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
		int getKey(int key);
		bool isKeyPressed(int key);
		bool isKeyHeld(int key);
		bool isKeyReleased(int key);
		bool isKeyUnheld(int key);
		int getNewerKey(int key1, int key2);
		int getOlderKey(int key1, int key2);

		//mouse input
		MousePos getMousePos();
		MousePos getMouseDelta();
		
		bool isMousePressed(int mbutton);
		bool isMouseHeld(int mbutton);
		bool isMouseReleased(int mbutton);
		bool isMouseUnheld(int mbutton);

	private:
		//-------------------------------------------------------------------------
		// PRIVATE METHODS
		//-------------------------------------------------------------------------
		// private constructor
		Input() {}

		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		static const unsigned long long RELEASED = -1;
		static const int TOTALKEYS = 120;
		static const int MAXKEY = 349;
		static const int TOTALMBUTTONS = 5;
		const int VALID_KEYS[TOTALKEYS] = { 32, 39, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 59, 61, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 96, 161, 162, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 280, 281, 282, 283, 284, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 340, 341, 342, 343, 344, 345, 346, 347, 348 };
		
		// keep track of how many frames they were held down for
		// to compare with other keys and see which was pressed first
		// unsigned long long will run out after a decade or so of being held down
		unsigned long long keys[MAXKEY] = { 0 };
		unsigned long long mbuttons[TOTALMBUTTONS] = { 0 };

		// check for mouse position
		MousePos mpos_current;
		MousePos mpos_last;
	};
}