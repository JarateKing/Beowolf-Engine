#include "W_Common.h"

namespace wolf
{
	float GetAspectRatio(glm::vec2 fov);
	glm::vec2 GetFovCombo(float verticalFov, float aspectRatio);
	glm::vec2 GetFovCombo(float verticalFov, glm::vec2 screenSize);
	glm::vec2 GetFovCombo(float verticalFov);
	glm::mat4 GetProjectionMatrix(float verticalFov);
	glm::mat4 GetProjectionMatrix(glm::vec2 fov);
	glm::mat4 GetProjectionMatrix(glm::vec2 fov, bool isOrtho, float orthoDistance);
	glm::mat4 GetProjectionMatrix(glm::vec2 fov, float orthoPercent, float orthoDistance);
}