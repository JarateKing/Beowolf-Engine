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

	bool Math2D::isOnLine(glm::vec2 point, glm::vec2 lineStart, glm::vec2 lineEnd)
	{
		return (point.x <= std::fmax(lineStart.x, lineEnd.x) &&
				point.x >= std::fmin(lineStart.x, lineEnd.x) &&
				point.y >= std::fmax(lineStart.y, lineEnd.y) &&
				point.y <= std::fmin(lineStart.y, lineEnd.y));
	}

	bool Math2D::isIntersection(glm::vec2 line1start, glm::vec2 line1end, glm::vec2 line2start, glm::vec2 line2end)
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

	bool Math2D::isPointInPolygon(glm::vec2 point, glm::vec2* arr, int arrSize)
	{
		glm::vec2 line1start = point;
		glm::vec2 line1end = point;
		line1end.x += 1000;

		int count = 0;
		for (int i = 0; i < arrSize; i++)
		{
			glm::vec2 line2start = glm::vec2(arr[i].x, arr[i].y);
			glm::vec2 line2end = glm::vec2(arr[(i + 1) % arrSize].x, arr[(i + 1) % arrSize].y);

			if (isIntersection(line1start, line1end, line2start, line2end))
			{
				count++;
			}
		}
		return count % 2 == 1;
	}

	float Math2D::GetBound(Pos dir, glm::vec2* arr, int arrSize)
	{
		float xmax, xmin, zmax, zmin;
		xmax = arr[0].x;
		xmin = arr[0].x;
		zmax = arr[0].y;
		zmin = arr[0].y;

		for (int i = 1; i < arrSize; i++)
		{
			if (arr[i].x > xmax)
				xmax = arr[i].x;
			else if (arr[i].x < xmin)
				xmin = arr[i].x;

			if (arr[i].y > zmax)
				zmax = arr[i].y;
			else if (arr[i].y < zmin)
				zmin = arr[i].y;
		}

		switch (dir)
		{
		case North:
			return zmin;
		case East:
			return xmax;
		case South:
			return zmax;
		case West:
			return xmin;
		default:
			return xmax;
		}
	}

	float Math2D::GetDistance(glm::vec2 start, glm::vec2 end)
	{
		return glm::sqrt(GetDistanceSq(start, end));
	}

	float Math2D::GetDistanceSq(glm::vec2 start, glm::vec2 end)
	{
		return (start.x - end.x) * (start.x - end.x) + (start.y - end.y) * (start.y - end.y);
	}

	glm::vec2 Math2D::Rotate(glm::vec2 point, glm::vec2 pivot, float angle)
	{
		glm::vec2 dist = point - pivot;
		return glm::vec2(dist.x * glm::cos(angle) - dist.y * glm::sin(angle), dist.x * glm::sin(angle) + dist.y * glm::cos(angle));
	}
}