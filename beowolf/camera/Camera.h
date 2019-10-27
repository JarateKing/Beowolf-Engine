#ifndef CAMERA_H
#define CAMERA_H

#include "W_Common.h"
#include "W_ProjectionMatrix.h"
#include "W_Input.h"

class Camera
{
public:
	Camera(float horizontalAngle, float verticalAngle, glm::vec3 position);
	~Camera();
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

#endif