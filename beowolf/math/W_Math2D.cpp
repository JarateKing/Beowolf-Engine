#include "W_Math2D.h"

namespace wolf
{
	int checkOrientation(glm::vec2 a, glm::vec2 b, glm::vec2 c)
	{
		float value = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
		if (value == 0) return 0;
		if (value > 0) return 1;
		return -1;
	}

	bool isOnLine(glm::vec2 point, glm::vec2 lineStart, glm::vec2 lineEnd)
	{
		return (point.x <= std::fmax(lineStart.x, lineEnd.x) &&
				point.x >= std::fmin(lineStart.x, lineEnd.x) &&
				point.y >= std::fmax(lineStart.y, lineEnd.y) &&
				point.y <= std::fmin(lineStart.y, lineEnd.y));
	}

	bool isIntersection(glm::vec2 line1start, glm::vec2 line1end, glm::vec2 line2start, glm::vec2 line2end)
	{
		int o1 = checkOrientation(line1start, line1end, line2start);
		int o2 = checkOrientation(line1start, line1end, line2end);
		int o3 = checkOrientation(line2start, line2end, line1start);
		int o4 = checkOrientation(line2start, line2end, line1end);

		if (o1 != o2 && o3 != o4)
			return true;

		if (o1 == 0 && isOnLine(line2start, line1start, line1end)) return true;
		if (o2 == 0 && isOnLine(line2end, line1start, line1end)) return true;
		if (o3 == 0 && isOnLine(line1start, line2start, line2end)) return true;
		if (o4 == 0 && isOnLine(line1end, line2start, line2end)) return true;

		return false;
	}
}