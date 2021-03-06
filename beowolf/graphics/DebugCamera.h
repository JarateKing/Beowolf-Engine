#ifndef CAMERA_H
#define CAMERA_H

#include "W_Common.h"

namespace wolf
{
	class DebugCamera
	{
	public:
		DebugCamera(float horizontalAngle, float verticalAngle, glm::vec3 position);
		~DebugCamera();
		void Update(float delta);
		glm::mat4 GetViewMatrix();


	private:
		void ApplyAngleVectors();

		float m_horiz;
		float m_verti;
		glm::vec3 m_aim;
		glm::vec3 m_right;
		glm::vec3 m_up;
		glm::vec3 m_pos;

		glm::mat4 m_proj;
		glm::mat4 m_view;
	};
}

#endif