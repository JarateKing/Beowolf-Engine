#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>

#include <BMWModel.h>
#include <W_Time.h>
#include <W_ProjectionMatrix.h>
#include <DebugCamera.h>

wolf::DebugCamera* cam;
wolf::BMWModel* test;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	cam = new wolf::DebugCamera(0, 0, glm::vec3(0, 0, -12));
	test = new wolf::BMWModel("resources/models/cube.bmw", "resources/shaders/cube.vsh", "resources/shaders/cube.fsh");
}

void BaseScene::Update()
{
	double delta = wolf::Time::Instance().deltaTime();
	cam->Update(delta);
	test->update(delta);

	std::cout << (cam->GetViewMatrix()[3][1]);
}

void BaseScene::Render()
{
	test->render(cam->GetViewMatrix(), glm::mat4());
}


