#include "W_ProjectionMatrix.h"

namespace wolf
{
	const float NEAR = 0.1f;
	const float FAR = 1000.0f;

	// get aspect ratio from horizontal and vertical fov
	float ProjMatrix::GetAspectRatio(const glm::vec2& fov)
	{
		return glm::tan(DEG2RAD * fov.y / 2) * glm::tan(DEG2RAD * fov.x / 2);
	}

	glm::vec2 ProjMatrix::GetScreenSize()
	{
		int width, height;
		glfwGetWindowSize(&width, &height);
		return glm::vec2(width, height);
	}

	// get horizontal and vertical fov using an aspect ratio
	glm::vec2 ProjMatrix::GetFovCombo(float verticalFov, float aspectRatio)
	{
		float horizontal = RAD2DEG * 2 * glm::atan(glm::tan(verticalFov * DEG2RAD / 2) * aspectRatio);
		return glm::vec2(verticalFov, horizontal);
	}

	// get horizontal and vertical fov using a width and height
	glm::vec2 ProjMatrix::GetFovCombo(float verticalFov, const glm::vec2& screenSize)
	{
		float aspectRatio = ((float)screenSize.x) / fmax((float)screenSize.y, 1.0f);
		return GetFovCombo(verticalFov, aspectRatio);
	}

	// get horizontal and vertical fov using the window size
	glm::vec2 ProjMatrix::GetFovCombo(float verticalFov)
	{
		return GetFovCombo(verticalFov, GetScreenSize());
	}

	// get perspective matrix based off of fov
	glm::mat4 ProjMatrix::GetProjectionMatrix(float verticalFov)
	{
		return GetProjectionMatrix(GetFovCombo(verticalFov), false, 1.0f);
	}
	glm::mat4 ProjMatrix::GetProjectionMatrix(const glm::vec2& fov)
	{
		return GetProjectionMatrix(fov, false, 1.0f);
	}

	// get projection matrix that is either perspective or orthographic
	// perspective is defined by horizontal and vertical fov
	// orthographic is defined by the distance where the view will be the same as perspective with the same fov values
	glm::mat4 ProjMatrix::GetProjectionMatrix(const glm::vec2& fov, bool isOrtho, float orthoDistance)
	{
		float aspect = GetAspectRatio(fov);

		if (isOrtho)
		{
			float top = glm::atan(DEG2RAD * fov.y) * orthoDistance;
			float right = top * aspect;
			return glm::ortho(-right, right, -top, top, NEAR, FAR);
		}
		else
		{
			return glm::perspective(fov.y / 2.0f, aspect, NEAR, FAR);
		}
	}

	// get projection matrix that is some percent perspective, some percent orthographic
	glm::mat4 ProjMatrix::GetProjectionMatrix(const glm::vec2& fov, float orthoPercent, float orthoDistance)
	{
		// special cases, percent breaks when <0 and >1
		if (orthoPercent >= 1.0f)
			return GetProjectionMatrix(fov, true, orthoDistance);
		else if (orthoPercent <= 0.0f)
			return GetProjectionMatrix(fov, false, orthoDistance);

		// more visually accurate
		float percent = glm::sqrt(orthoPercent);

		glm::mat4 toret = glm::mat4();
		glm::mat4 perspective = GetProjectionMatrix(fov, false, orthoDistance);
		glm::mat4 orthographic = GetProjectionMatrix(fov, true, orthoDistance);

		// lerp each element in the matrix
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				toret[i][j] = perspective[i][j] * (1 - percent) + orthographic[i][j] * (percent);
			}
		}

		return toret;
	}

	Plane* ProjMatrix::GetFrustum(const glm::mat4& matrix)
	{
		// Hartman/Gribbs method
		// right, left, top, bottom, far, near
		Plane* toret = new Plane[6];

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				toret[i][j] = matrix[j][3] + (i % 2 * 2 - 1) * matrix[j][i / 2];
			}
		}

		return toret;
	}
}