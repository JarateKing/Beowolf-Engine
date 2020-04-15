#ifndef CAMERA_H
#define CAMERA_H

#include "W_Common.h"
#include "W_ProjectionMatrix.h"
#include "W_Input.h"
#include <vector>

class Camera
{
public:
	//Public Methods
	Camera(const float horizontalAngle, const float verticalAngle, const glm::vec3 position);
	~Camera();
	void Update(float delta);
	void SetVerticleAngle(const float verti);
	void ForceAngleUpdate();
	void MoveToView(glm::vec3 position, glm::vec3 offset, float time);
	void InitiateShake();
	int CalculateIntersection(std::vector<float> heights, std::vector<glm::vec2> positions, float tileWidth);
	const glm::vec3 GetPos() const;
	const glm::vec3 GetUp() const;
	const glm::vec3 GetAim() const;
	const glm::mat4 GetProj() const;
	const glm::mat4 GetView() const;
	const glm::mat4 GetViewMatrix() const;
	const glm::mat4 GetVerticalInverse(float heightPlane) const;

private:
	//Private Methods
	void ApplyAngleVectors();
	glm::vec3 GetRayFromScreen();

	//Private Variables
	glm::vec3 m_startPos;
	glm::vec3 m_endPos;
	glm::vec3 m_startShakePos;
	glm::vec3 m_startShakeUp;
	glm::vec3 m_aim;
	glm::vec3 m_right;
	glm::vec3 m_up;
	glm::vec3 m_pos;
	float m_startShakeVert;
	float m_horiz;
	float m_verti;
	float m_fov = 90.0f;
	float m_startY;
	float m_timeShaking = 0.0f;
	float m_moveTime = 0.0f;
	float m_moveTimeLimit = 0.0f;
	bool m_shaking = false;
	glm::mat4 m_proj;
	glm::mat4 m_view;
};

#endif