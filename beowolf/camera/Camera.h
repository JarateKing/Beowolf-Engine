#ifndef CAMERA_H
#define CAMERA_H

#include "W_Common.h"
#include "W_ProjectionMatrix.h"
#include "W_Input.h"
#include <vector>

class Camera
{
public:
	Camera(float horizontalAngle, float verticalAngle, glm::vec3 position);
	~Camera();
	void Update(float delta);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetVerticalInverse(float heightPlane);
	int CalculateIntersection(std::vector<float> heights, std::vector<glm::vec2> positions, float tileWidth);
	void SetVerticleAngle(float verti);
	void ForceAngleUpdate();
	glm::vec3 GetPos();
	glm::vec3 GetUp();
	glm::vec3 GetAim();
	glm::mat4 GetProj();
	glm::mat4 GetView();
	void MoveToView(glm::vec3 position, glm::vec3 offset, float time);

private:
	void ApplyAngleVectors();
	glm::vec3 GetRayFromScreen();

	float m_horiz;
	float m_verti;
	float fov = 90.0f;
	float startY;
	glm::vec3 m_aim;
	glm::vec3 m_right;
	glm::vec3 m_up;
	glm::vec3 m_pos;

	glm::mat4 m_proj;
	glm::mat4 m_view;

	float m_moveTime = 0.0f;
	float m_moveTimeLimit = 0.0f;
	glm::vec3 m_startPos;
	glm::vec3 m_endPos;
};

#endif