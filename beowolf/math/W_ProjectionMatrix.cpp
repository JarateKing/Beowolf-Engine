#include "W_ProjectionMatrix.h"
#include "W_Math.h"

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
				toret[i][j] = Math::lerp(perspective[i][j], orthographic[i][j], percent);
			}
		}

		return toret;
	}

	// get projection matrix from frustum values
	glm::mat4 ProjMatrix::GetProjectionMatrix(float l, float r, float b, float t, float n, float f)
	{
		// references
		// https://github.com/godotengine/godot/blob/2c6daf73f3a1077dfae0ca88117a3f4b583eb7e6/core/math/camera_matrix.cpp#L184-L211
		
		glm::mat4 projection = glm::mat4();

		projection[0][0] = 2 * n / (r - l);
		projection[1][1] = 2 * n / (t - b);
		projection[2][0] = (r + l) / (r - l);
		projection[2][1] = (t + b) / (t - b);
		projection[2][2] = -(f + n) / (f - n);
		projection[2][3] = -1;
		projection[3][2] = -2 * f * n / (f - n);
		projection[3][3] = 0;

		return projection;
	}

	// get projection matrix from an arbitrary view frustum
	glm::mat4 ProjMatrix::GetProjectionMatrix(Plane* planes)
	{
		// references
		// https://www.gamedev.net/forums/topic/512563-projection-matrix-from-6-arbitrary-frustum-planes/

		// @TODO:
		// - support near and far values from planes
		// - support orthographic and partially-orthographic frustums

		float n = NEAR;
		float f = FAR;
		float r = (planes[0].c * n) / planes[0].a;
		float l = (planes[1].c * n) / planes[1].a;
		float t = (planes[2].c * n) / planes[2].b;
		float b = (planes[3].c * n) / planes[3].b;

		return GetProjectionMatrix(l, r, b, t, n, f);
	}

	// get the view frustum from an arbitrary matrix
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