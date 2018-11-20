#include "W_Common.h"

namespace wolf
{
	bool isOnLine(glm::vec2 point, glm::vec2 lineStart, glm::vec2 lineEnd);
	bool isIntersection(glm::vec2 line1start, glm::vec2 line1end, glm::vec2 line2start, glm::vec2 line2end);
	bool isPointInPolygon(glm::vec2 point, TextureVertex* arr, int arrSize);
	float getBound(Pos dir, TextureVertex* arr, int arrSize);
}