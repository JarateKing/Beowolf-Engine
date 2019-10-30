#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>
#include <iomanip>
#include "W_Math.h"

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	for (float i = 0.0f; i <= 1.000001f; i += 0.1f) {
		std::cout << std::fixed << std::setprecision(1) << i;
		std::cout << " = in: " << std::fixed << std::setprecision(6) << i;
		std::cout << " - out: " << std::fixed << std::setprecision(6) << i;
		std::cout << " - both: " << std::fixed << std::setprecision(6) << i << "\n";
	}
}

void BaseScene::Update()
{
}

void BaseScene::Render()
{
}


