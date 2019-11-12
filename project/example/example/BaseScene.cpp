#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>
#include <BMWModel.h>
#include <W_Time.h>
#include <W_ProjectionMatrix.h>
#include <iomanip>
#include "sound/W_SoundEngine.h"
#include "beowolf/hexGrid/HexGrid.h"
#include "camera/Camera.h"
#include "DebugCube.h"
#include "SceneRenderer.h"
#include "W_Time.h"
#include "W_Math.h"

const float DISTANCEFACTOR = 1.0f;
wolf::SoundEngine SE;
static Camera* cam;
static glm::mat4 cull;
static HexGrid* grid;
wolf::BMWModel* test;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	test = new wolf::BMWModel("resources/models/myskeleton.bmw", "resources/shaders/animatable.vsh", "resources/shaders/animatable.fsh");
	test->setTransform(glm::translate(glm::vec3(0.0f, 0.0f, 25.0f)) * glm::rotate(180.0f, glm::vec3(0, 1.0f, 0)) * glm::scale(glm::vec3(0.1, 0.1, 0.1)));

	cam = new Camera(0, 5.5, glm::vec3(0, 50.0f, 0));
	cull = cam->GetViewMatrix();

	wolf::SceneRenderer::getInstance().GenerateQuadtree(-10.0f, -10.0f, 20.0f, 20.0f);
	grid = new HexGrid(50 , 50, 5.0f, 1.0f, 20.0f, "resources/textures/tiles/Tile_Texs_1.tga");
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


