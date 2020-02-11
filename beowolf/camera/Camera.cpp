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
		float sens = -0.11f * DEG2RAD;
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
		float m_posZ = m_pos.z;
		if (m_pos.y >= startY - 15)
		{
			m_pos += m_aim * movespeed;
		}
	}
	if (wolf::Input::Instance().isKeyHeld(INPUT_KB_SPACE))
	{
		float m_posZ = m_pos.z;
		if (m_pos.y <= startY + 15)
		{
			m_pos -= m_aim * movespeed;
		}
	}

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

glm::vec3 Camera::GetRayFromScreen()
{
	int width, height;
	glfwGetWindowSize(&width, &height);

	float x = ((2.0f * (float)wolf::Input::Instance().getMousePos().x) / (float)width) - 1.0f;
	float y = 1.0f - ((2.0f * (float)wolf::Input::Instance().getMousePos().y) / (float)height);

	glm::vec4 ray(x, y, -1.0f, 1.0f);
	glm::vec4 ray_eye = glm::inverse(m_proj) * ray;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

	glm::vec4 ray_worT = (glm::inverse(m_view) * ray_eye);
	glm::vec3 ray_wor(ray_worT.x, ray_worT.y, ray_worT.z);
	//dir
	ray_wor = glm::normalize(ray_wor);

	return ray_wor;
}

int Camera::CalculateIntersection(std::vector<float> heights, std::vector<glm::vec2> positions, float tileWidth)
{
	glm::vec3 direction = GetRayFromScreen();
	glm::vec3 intersection;
	glm::vec2 currPos;
	float toEdge = sqrt(pow(tileWidth / 2, 2) - pow(tileWidth / 4, 2));
	int target = -1;
	int intersecting = 0;

	for (int i = 0; i < heights.size(); i++)
	{
		currPos = positions.at(i);
		intersection = m_pos + ((heights.at(i) - m_pos.y) / direction.y) * direction;
		if ((intersection.x >= (currPos.x - toEdge)) && (intersection.x <= (currPos.x + toEdge)) && (intersection.z <= (currPos.y + (tileWidth / 2))) && (intersection.z >= (currPos.y - (tileWidth / 2))))
		{
			intersecting++;
			if (target == -1)
				target = i;
			else if (target >= 0)
			{
				if (heights.at(target) < heights.at(i))
					target = i;
			}
		}
	}
	if (target > heights.size() || target < 0)
		target = -1;
	return target;
}

void Camera::SetVerticleAngle(float verti) {
	m_verti = verti;
}

void Camera::ForceAngleUpdate() {
	ApplyAngleVectors();
}

void Camera::MoveToView(glm::vec3 position, glm::vec3 offset, float time) {
	offset = (glm::vec3)(glm::vec4(offset.x, offset.y, offset.z, 0) * glm::rotate(-m_horiz * RAD2DEG, glm::vec3(0, 1, 0)));
	position += offset;

	if (time == 0) {
		m_pos = position;
	}
	else {
		m_startPos = m_pos;
		m_endPos = position;

		m_moveTime = 0.0f;
		m_moveTimeLimit = time;
	}
}
