#include "W_Input_Keys.h"

namespace wolf
{
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
		int getKey(int key);
		bool isKeyPressed(int key);
		bool isKeyHeld(int key);
		bool isKeyReleased(int key);
		bool isKeyUnheld(int key);
		int getNewer(int key1, int key2);
		int getOlder(int key1, int key2);
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
		const int VALID_KEYS[TOTALKEYS] = { 32, 39, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 59, 61, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 96, 161, 162, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 280, 281, 282, 283, 284, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 340, 341, 342, 343, 344, 345, 346, 347, 348 };
		
		// keep track of how many frames they were held down for
		// to compare with other keys and see which was pressed first
		// unsigned long long will run out after a decade or so of being held down
		unsigned long long keys[MAXKEY] = { 0 };
	};
}