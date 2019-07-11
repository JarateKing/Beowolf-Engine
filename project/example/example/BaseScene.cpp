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
	glm::mat4 proj = wolf::ProjMatrix::GetProjectionMatrix(90.0f);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << proj[i][j] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl << std::endl;

	glm::mat4 proj2 = wolf::ProjMatrix::GetProjectionMatrix(wolf::ProjMatrix::GetFrustum(proj));
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << proj2[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void BaseScene::Update()
{
}

void BaseScene::Render()
{
}


