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
}