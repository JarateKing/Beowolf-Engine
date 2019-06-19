#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>
#include "W_Math.h"
#include <iomanip>

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	std::cout << std::setprecision(20) << wolf::Math::lerp(10.0f, 30.0f, 0.5f) << std::endl;
	std::cout << std::setprecision(20) << wolf::Math::lerp(0.0, 0.0, 0.5f);
}

void BaseScene::Update()
{
}

void BaseScene::Render()
{
}


