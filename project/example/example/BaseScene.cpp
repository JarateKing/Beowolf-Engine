#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>

#include <BMWModel.h>
#include <W_Time.h>

wolf::BMWModel* test;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	test = new wolf::BMWModel("resources/models/hand.bmw", "", "");
}

void BaseScene::Update()
{
	test->update(wolf::Time::Instance().deltaTime());
}

void BaseScene::Render()
{
	test->render(glm::mat4(), glm::mat4());
}


