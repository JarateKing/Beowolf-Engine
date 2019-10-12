#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>

#include <BMWModel.h>
#include <W_Time.h>
#include <W_ProjectionMatrix.h>

wolf::BMWModel* test;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	test = new wolf::BMWModel("resources/models/cube.bmw", "resources/shaders/cube.vsh", "resources/shaders/cube.fsh");
}

void BaseScene::Update()
{
	test->update(wolf::Time::Instance().deltaTime());
}

void BaseScene::Render()
{
	test->render(glm::mat4(), wolf::ProjMatrix::GetProjectionMatrix(90.0f));
}


