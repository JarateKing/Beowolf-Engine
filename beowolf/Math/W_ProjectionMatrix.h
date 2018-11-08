#include "W_Common.h"

namespace wolf
{
	float GetAspectRatio(const glm::vec2& fov);
	glm::vec2 GetScreenSize();
	glm::vec2 GetFovCombo(float verticalFov, float aspectRatio);
	glm::vec2 GetFovCombo(float verticalFov, const glm::vec2& screenSize);
	glm::vec2 GetFovCombo(float verticalFov);
	glm::mat4 GetProjectionMatrix(float verticalFov);
	glm::mat4 GetProjectionMatrix(const glm::vec2& fov);
	glm::mat4 GetProjectionMatrix(const glm::vec2& fov, bool isOrtho, float orthoDistance);
	glm::mat4 GetProjectionMatrix(const glm::vec2& fov, float orthoPercent, float orthoDistance);
}