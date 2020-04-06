#include "Camera.h"
#include <iostream>
#include "W_Math.h"
#include <cmath>
#include "W_Keybind.h"

const float CONTROLLER_AXIS_THRESHOLD = 0.2;

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
	if (m_shaking)
	{
		m_timeShaking += delta;
		if (m_timeShaking <= 0.20)
		{
			m_pos.x += sin(m_timeShaking * 50)/20;
			m_verti += sin(m_timeShaking * 1000)/100;
			ApplyAngleVectors();
		}
		else
		{
			m_shaking = false;
			m_timeShaking = 0.0f;
			m_pos = m_startShakePos;
			m_verti = m_startShakeVert;
		}
	}

	// look angle controls
	if (wolf::Input::Instance().isMousePressed(INPUT_RMB))
		glfwDisable(GLFW_MOUSE_CURSOR);
	else if (wolf::Input::Instance().isMouseReleased(INPUT_RMB))
		glfwEnable(GLFW_MOUSE_CURSOR);

	
	if (wolf::Keybind::Instance().getBind("movecamera"))
	{
		float sens = -0.11f * DEG2RAD;
		m_horiz += wolf::Input::Instance().getMouseDelta().x * sens;
	}

	if (std::abs(wolf::Input::Instance().getControllerRightStick().x) > CONTROLLER_AXIS_THRESHOLD)
		m_horiz += -wolf::Input::Instance().getControllerRightStick().x * DEG2RAD * 1.75f;

	ApplyAngleVectors();

	// position controls
	float movespeed = delta * 25.0f;
	if (wolf::Keybind::Instance().getBind("cameraforward"))
		m_pos += glm::vec3(m_aim.x, 0.0f, m_aim.z) * movespeed;
	if (wolf::Keybind::Instance().getBind("cameraback"))
		m_pos -= glm::vec3(m_aim.x, 0.0f, m_aim.z) * movespeed;
	if (wolf::Keybind::Instance().getBind("cameraleft"))
		m_pos -= m_right * movespeed;
	if (wolf::Keybind::Instance().getBind("cameraright"))
		m_pos += m_right * movespeed;
	if (wolf::Keybind::Instance().getBind("camerain"))
	{
		float m_posZ = m_pos.z;
		//if (m_pos.y >= startY - 15)
		//{
			m_pos += m_aim * movespeed;
		//}
	}
	if (wolf::Keybind::Instance().getBind("cameraout"))
	{
		float m_posZ = m_pos.z;
		//if (m_pos.y <= startY + 15)
		//{
			m_pos -= m_aim * movespeed;
		//}
	}

	if (std::abs(wolf::Input::Instance().getControllerLeftStick().x) > CONTROLLER_AXIS_THRESHOLD)
		m_pos += m_right * wolf::Input::Instance().getControllerLeftStick().x * movespeed;
	if (std::abs(wolf::Input::Instance().getControllerLeftStick().y) > CONTROLLER_AXIS_THRESHOLD)
		m_pos += glm::vec3(m_aim.x, 0.0f, m_aim.z) * wolf::Input::Instance().getControllerLeftStick().y * movespeed;

	if (std::abs(wolf::Input::Instance().getControllerAxis(INPUT_CONTROLLER_AXIS_TRIGGER)) > CONTROLLER_AXIS_THRESHOLD)
	{
		float m_posZ = m_pos.z;
		m_pos += m_aim * delta * 30.0f * wolf::Input::Instance().getControllerAxis(INPUT_CONTROLLER_AXIS_TRIGGER);
	}

	if (m_moveTimeLimit != 0 && m_moveTime != m_moveTimeLimit) {
		m_moveTime += delta;
		if (m_moveTime > m_moveTimeLimit)
			m_moveTime = m_moveTimeLimit;

		m_pos = wolf::Math::lerp(m_startPos, m_endPos, wolf::Math::ease(m_moveTime / m_moveTimeLimit));
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

glm::mat4 Camera::GetVerticalInverse(float heightPlane)
{
	glm::vec3 newPos = m_pos;
	newPos.y = -newPos.y + heightPlane * 2;
	glm::vec3 newAim = m_aim;
	newAim.y = -newAim.y;
	glm::vec3 newUp = glm::cross(m_right, newAim);

	return m_proj * (glm::lookAt(newPos, newPos + newAim, newUp));
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

glm::vec3 Camera::GetPos()
{
	return m_pos;
}

glm::vec3 Camera::GetUp()
{
	return m_up;
}

glm::vec3 Camera::GetAim()
{
	return m_aim;
}

glm::mat4 Camera::GetProj()
{
	return m_proj;
}

glm::mat4 Camera::GetView()
{
	return m_view;
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

void Camera::InitiateShake()
{
	if (!m_shaking)
	{
		m_startShakeVert = m_verti;
		m_startShakePos = m_pos;
		m_shaking = true;
	}
}
