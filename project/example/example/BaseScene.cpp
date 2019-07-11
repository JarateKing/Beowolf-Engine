#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>
#include "W_Math.h"
#include "W_ProjectionMatrix.h"
#include <iomanip>

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	glm::vec3 test = glm::normalize(glm::vec3(1.0f, 0.5f, 0.00f));
	std::cout << test.x << ", " << test.y << ", " << test.z << "\n";

	test = glm::eulerAngles(wolf::Math::toQuaternion(test));
	std::cout << test.x << ", " << test.y << ", " << test.z << "\n";
}

void BaseScene::Update()
{
}

void BaseScene::Render()
{
}


