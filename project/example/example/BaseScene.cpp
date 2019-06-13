#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>
#include "W_Math.h"

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	std::cout << "lerp: " << wolf::Math::lerp(5.0f, 7.0f, 0.5f);
}

void BaseScene::Update()
{
}

void BaseScene::Render()
{
}


