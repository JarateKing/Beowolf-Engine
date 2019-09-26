#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include "SceneRenderer.h"
#include "DebugCamera.h"
#include "DebugCube.h"
#include "W_Time.h"
#include "W_Input.h"

static wolf::DebugCamera* cam;
static glm::mat4 cull;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	glEnable(GL_DEPTH_TEST);

	// view stuff
	cam = new wolf::DebugCamera(0, 0, glm::vec3(0, 0, -12));
	cull = cam->GetViewMatrix();

	// setup cubes
	for (int i = 0; i < 10; i++)
	{
		wolf::SceneRenderer::getInstance().AddNode((wolf::Node*)new wolf::DebugCube());
	}

	wolf::SceneRenderer::getInstance().GenerateQuadtree(-10, -10, 20, 20);
}

void BaseScene::Update()
{
	float delta = wolf::Time::Instance().deltaTime();

	// update camera stuff
	cam->Update(delta);
	if (wolf::Input::Instance().isKeyUnheld(INPUT_KB_C))
		cull = cam->GetViewMatrix();

	// update all objects and apply culling
	wolf::SceneRenderer::getInstance().Update(delta, cull);
}

void BaseScene::Render()
{
	wolf::SceneRenderer::getInstance().Render(cam->GetViewMatrix());
	wolf::SceneRenderer::getInstance().GetQuadtree()->DebugRender(cam->GetViewMatrix());
}


