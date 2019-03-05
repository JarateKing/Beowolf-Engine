#include "W_Common.h"

namespace wolf
{
	class ProjMatrix
	{
	public:
		static float GetAspectRatio(const glm::vec2& fov);
		static glm::vec2 GetScreenSize();
		static glm::vec2 GetFovCombo(float verticalFov, float aspectRatio);
		static glm::vec2 GetFovCombo(float verticalFov, const glm::vec2& screenSize);
		static glm::vec2 GetFovCombo(float verticalFov);
		static glm::mat4 GetProjectionMatrix(float verticalFov);
		static glm::mat4 GetProjectionMatrix(const glm::vec2& fov);
		static glm::mat4 GetProjectionMatrix(const glm::vec2& fov, bool isOrtho, float orthoDistance);
		static glm::mat4 GetProjectionMatrix(const glm::vec2& fov, float orthoPercent, float orthoDistance);
		static Plane* GetFrustum(const glm::mat4& matrix);
	};
}