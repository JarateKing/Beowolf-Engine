#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include "DebugCamera.h"
#include "DebugCube.h"
#include "SceneRenderer.h"
#include "W_Time.h"

static wolf::DebugCamera* cam;
static glm::mat4 cull;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	glEnable(GL_DEPTH_TEST);

	cam = new wolf::DebugCamera(0, 0, glm::vec3(0, 0, -12));
	cull = cam->GetViewMatrix();

	for (int i = 0; i < 30; i++)
	{
		wolf::SceneRenderer::getInstance().AddNode((wolf::Node*)new wolf::DebugCube());
	}
	wolf::SceneRenderer::getInstance().GenerateQuadtree(-10.0f, -10.0f, 20.0f, 20.0f);
}

void BaseScene::Update()
{
	float delta = wolf::Time::Instance().deltaTime();
	cam->Update(delta);
	wolf::SceneRenderer::getInstance().Update(delta, cam->GetViewMatrix());
}

void BaseScene::Render()
{
	wolf::SceneRenderer::getInstance().Render(cam->GetViewMatrix());
}


