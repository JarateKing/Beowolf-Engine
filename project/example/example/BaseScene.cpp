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
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	cam = new wolf::DebugCamera(0, 0, glm::vec3(0, 0, -4));
	test = new wolf::BMWModel("resources/models/myskeleton.bmw", "resources/shaders/cube.vsh", "resources/shaders/cube.fsh");
	test->setTransform(glm::rotate(180.0f, glm::vec3(0, 1.0f, 0)) * glm::scale(glm::vec3(0.01, 0.01, 0.01)));
}

void BaseScene::Update()
{
	double delta = wolf::Time::Instance().deltaTime();
	cam->Update(delta);
	test->update(delta);
}

void BaseScene::Render()
{
	glDepthMask(true);
	glDisable(GL_BLEND);

	test->render(cam->GetViewMatrix(), glm::mat4(), false);

	glDepthMask(false);
	glEnable(GL_BLEND);

	test->render(cam->GetViewMatrix(), glm::mat4(), true);
}


