#include "W_Common.h"

namespace wolf
{
	class Math2D
	{
	public:
		static bool isOnLine(glm::vec2 point, glm::vec2 lineStart, glm::vec2 lineEnd);
		static bool isIntersection(glm::vec2 line1start, glm::vec2 line1end, glm::vec2 line2start, glm::vec2 line2end);
		static bool isPointInPolygon(glm::vec2 point, glm::vec2* arr, int arrSize);
		static float GetBound(Pos dir, glm::vec2* arr, int arrSize);
		static float GetDistance(glm::vec2 start, glm::vec2 end);
		static float GetDistanceSq(glm::vec2 start, glm::vec2 end);
		static glm::vec2 Rotate(glm::vec2 point, glm::vec2 pivot, float angle);
	};
}