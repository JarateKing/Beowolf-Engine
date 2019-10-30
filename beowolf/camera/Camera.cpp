#include "Camera.h"
#include <iostream>

Camera::Camera(float horizontalAngle, float verticalAngle, glm::vec3 position)
{
	m_horiz = horizontalAngle * DEG2RAD;
	m_verti = verticalAngle;
	m_pos = position;
	startY = position.y;

	ApplyAngleVectors();

	m_proj = wolf::ProjMatrix::GetProjectionMatrix(wolf::ProjMatrix::GetFovCombo(fov));
	m_view = glm::lookAt(m_pos, m_pos + m_aim, m_up);
}

Camera::~Camera()
{

}

void Camera::Update(float delta)
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

		ApplyAngleVectors();
	}

	// position controls
	float movespeed = delta * 25.0f;
	if (wolf::Input::Instance().isKeyHeld(INPUT_KB_W))
		m_pos += glm::vec3(m_aim.x, 0.0f, m_aim.z) * movespeed;
	if (wolf::Input::Instance().isKeyHeld(INPUT_KB_S))
		m_pos -= glm::vec3(m_aim.x, 0.0f, m_aim.z) * movespeed;
	if (wolf::Input::Instance().isKeyHeld(INPUT_KB_A))
		m_pos -= m_right * movespeed;
	if (wolf::Input::Instance().isKeyHeld(INPUT_KB_D))
		m_pos += m_right * movespeed;
	if (wolf::Input::Instance().isKeyHeld(INPUT_KB_Q))
	{
		if (fov >= 75.0f && fov <= 90.0f)
			fov -= 0.1f;
		m_pos += glm::vec3(0.0f, m_aim.y, 0.0f) * movespeed;
	}
	if (wolf::Input::Instance().isKeyHeld(INPUT_KB_SPACE))
	{
		if (fov >= 75.0f && fov <= 90.0f)
			fov += 0.1f;
		m_pos -= glm::vec3(0.0f, m_aim.y, 0.0f) * movespeed;
	}

	if (fov <= 75.0f)
		fov = 75.0f;
	if (fov >= 90.0f)
		fov = 90.0f;
	if (m_pos.y >= startY + 10)
		m_pos.y = startY + 10;
	if (m_pos.y <= startY - 10)
		m_pos.y = startY - 10;

	// updating matrix incase something changed
	m_proj = wolf::ProjMatrix::GetProjectionMatrix(wolf::ProjMatrix::GetFovCombo(fov));
	m_view = glm::lookAt(m_pos, m_pos + m_aim, m_up);
}

void Camera::ApplyAngleVectors()
{
	m_aim = glm::vec3(glm::cos(m_verti) * glm::sin(m_horiz), glm::sin(m_verti), glm::cos(m_verti) * glm::cos(m_horiz));
	m_right = glm::vec3(glm::sin(m_horiz - PI / 2.0f), 0, glm::cos(m_horiz - PI / 2.0f));
	m_up = glm::cross(m_right, m_aim);
}

glm::mat4 Camera::GetViewMatrix()
{
	return m_proj * m_view;
}