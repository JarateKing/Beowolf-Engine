#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>
#include "W_Math.h"
#include "W_ProjectionMatrix.h"
//#include <BMWModel.h>
#include <W_Time.h>
#include <W_ProjectionMatrix.h>
#include <iomanip>
//#include "DebugCamera.h"
#include "sound/W_SoundEngine.h"
#include "beowolf/hexGrid/HexGrid.h"
#include "camera/Camera.h"
#include "DebugCube.h"
#include "SceneRenderer.h"
#include "W_Time.h"
#include "W_Math.h"
#include "W_Input.h"
#include "W_ResourceLoader.h"
#include "camera/HexSelector.h"
#include "ComponentHexPos.h"

const float DISTANCEFACTOR = 1.0f;
wolf::SoundEngine SE;
static Camera* cam;
static glm::mat4 cull;
static HexGrid* grid;
wolf::MousePos mouse;
static HexSelector* selector;
wolf::BMWModel* test;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	auto shaders = wolf::ResourceLoader::Instance().getShaders("animatable_uv");
	test = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel("myskeleton.bmw"), shaders.first, shaders.second);
	test->setTransform(glm::translate(glm::vec3(0.0f, 20.0f, 20.0f)) * glm::rotate(180.0f, glm::vec3(0, 1.0f, 0)) * glm::scale(glm::vec3(0.1, 0.1, 0.1)));

	cam = new Camera(0, 5.5, glm::vec3(0, 50.0f, 0));
	cull = cam->GetViewMatrix();
	wolf::SceneRenderer::getInstance().GenerateQuadtree(-10.0f, -10.0f, 20.0f, 20.0f);
	grid = new HexGrid(30, 30, 5.0f, 1.0f, 20.0f, wolf::ResourceLoader::Instance().getTexture("tiles/Tile_Texs_1.tga"));
	selector = new HexSelector(5.0f);
}

void BaseScene::Update()
{
	float delta = wolf::Time::Instance().deltaTime();
	cam->Update(delta);

	test->update(delta);
	
	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_B))
		test->setAnim("attack");
	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_N))
		test->setAnim("attack2");
	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_M))
		test->setAnim("attack3");
	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_V))
		test->setAnim("poo");

	int target = cam->CalculateIntersection(grid->GetHeights(), grid->GetPos(), 5.0f);
	std::vector<float> heights = grid->GetHeights();
	std::vector<glm::vec2> positions = grid->GetPos();
	if (!(target < 0))
	{
		selector->Update(target, positions.at(target), heights.at(target));
	}
	wolf::SceneRenderer::getInstance().Update(delta, cam->GetViewMatrix());
}

void BaseScene::Render()
{
	glDepthMask(true);
	glDisable(GL_BLEND);

	test->render(cam->GetViewMatrix(), glm::mat4(), false);
	wolf::SceneRenderer::getInstance().Render(cam->GetViewMatrix());
	grid->Render(cam->GetViewMatrix());
	selector->Render(cam->GetViewMatrix());

	//glDepthMask(false);
	//glEnable(GL_BLEND);

	//test->render(cam->GetViewMatrix(), glm::mat4(), true);
}


