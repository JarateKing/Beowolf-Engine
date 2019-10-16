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
wolf::BMWModel* teapot;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	cam = new wolf::DebugCamera(0, 0, glm::vec3(0, 0, -4));
	test = new wolf::BMWModel("resources/models/tree.bmw", "resources/shaders/unlit_texture.vsh", "resources/shaders/unlit_texture.fsh");
	teapot = new wolf::BMWModel("resources/models/teapot.bmw", "resources/shaders/cube.vsh", "resources/shaders/cube.fsh");
	teapot->setTransform(glm::translate(glm::mat4(), glm::vec3(1.25, 0, 0)) * glm::scale(glm::vec3(0.25, 0.25, 0.25)));
}

void BaseScene::Update()
{
	double delta = wolf::Time::Instance().deltaTime();
	cam->Update(delta);
	test->update(delta);
	teapot->update(delta);

	teapot->setTransform(teapot->getTransform() * glm::rotate(1.0f, glm::vec3(0, 1, 0)));
}

void BaseScene::Render()
{
	glDepthMask(true);
	glDisable(GL_BLEND);

	test->render(cam->GetViewMatrix(), glm::mat4(), false);
	teapot->render(cam->GetViewMatrix(), glm::mat4(), false);

	glDepthMask(false);
	glEnable(GL_BLEND);

	test->render(cam->GetViewMatrix(), glm::mat4(), true);
	teapot->render(cam->GetViewMatrix(), glm::mat4(), true);
}


