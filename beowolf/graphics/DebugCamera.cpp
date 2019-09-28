#include "DebugCamera.h"
#include "W_ProjectionMatrix.h"
#include "W_Input.h"

namespace wolf
{
	DebugCamera::DebugCamera(float horizontalAngle, float verticalAngle, glm::vec3 position)
	{
		m_horiz = horizontalAngle * DEG2RAD;
		m_verti = verticalAngle;
		m_pos = position;

		ApplyAngleVectors();

		m_proj = wolf::ProjMatrix::GetProjectionMatrix(wolf::ProjMatrix::GetFovCombo(90.0f));
		m_view = glm::lookAt(m_pos, m_pos + m_aim, m_up);
	}

	DebugCamera::~DebugCamera()
	{

	}

	void DebugCamera::Update(float delta)
	{
		// look angle controls
		if (wolf::Input::Instance().isMousePressed(INPUT_RMB))
			glfwDisable(GLFW_MOUSE_CURSOR);
		else if (wolf::Input::Instance().isMouseReleased(INPUT_RMB))
			glfwEnable(GLFW_MOUSE_CURSOR);

		if (wolf::Input::Instance().isMouseHeld(INPUT_RMB))
		{
			float sens = -0.22f * DEG2RAD;
			m_horiz += wolf::Input::Instance().getMouseDelta().x * sens;
			m_verti += wolf::Input::Instance().getMouseDelta().y * sens;

			ApplyAngleVectors();
		}

		// position controls
		float movespeed = delta * 3.0f;
		if (wolf::Input::Instance().isKeyHeld(INPUT_KB_W))
			m_pos += m_aim * movespeed;
		if (wolf::Input::Instance().isKeyHeld(INPUT_KB_S))
			m_pos -= m_aim * movespeed;
		if (wolf::Input::Instance().isKeyHeld(INPUT_KB_A))
			m_pos -= m_right * movespeed;
		if (wolf::Input::Instance().isKeyHeld(INPUT_KB_D))
			m_pos += m_right * movespeed;
		if (wolf::Input::Instance().isKeyHeld(INPUT_KB_SPACE))
			m_pos += m_up * movespeed;
		if (wolf::Input::Instance().isKeyHeld(INPUT_KB_Q))
			m_pos -= m_up * movespeed;

		// updating matrix incase something changed
		m_proj = wolf::ProjMatrix::GetProjectionMatrix(wolf::ProjMatrix::GetFovCombo(90.0f));
		m_view = glm::lookAt(m_pos, m_pos + m_aim, m_up);
	}

	void DebugCamera::ApplyAngleVectors()
	{
		m_aim = glm::vec3(glm::cos(m_verti) * glm::sin(m_horiz), glm::sin(m_verti), glm::cos(m_verti) * glm::cos(m_horiz));
		m_right = glm::vec3(glm::sin(m_horiz - PI / 2.0f), 0, glm::cos(m_horiz - PI / 2.0f));
		m_up = glm::cross(m_right, m_aim);
	}

	glm::mat4 DebugCamera::GetViewMatrix()
	{
		return m_proj * m_view;
	}
}
