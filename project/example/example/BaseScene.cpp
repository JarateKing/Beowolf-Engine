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
#include "AIPathfinder.h"
#include <cmath>
#include "W_Hud.h"
#include "W_ProjectionMatrix.h"

const float DISTANCEFACTOR = 1.0f;
wolf::SoundEngine SE;
static Camera* cam;
static glm::mat4 cull;
static HexGrid* grid;
wolf::MousePos mouse;
static HexSelector* selector;
wolf::BMWModel* test;
wolf::BMWModel* test2;
week2::ComponentHexPos hexPos;
std::vector<int> testMove;
//week9::AIPathfinder* pathFinder;
wolf::Hud* testhud;
glm::mat4 hudProjMat;

BaseScene::BaseScene()
{
	//pathFinder->CreateInstance();
}

void BaseScene::Init()
{
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	auto shaders = wolf::ResourceLoader::Instance().getShaders("animatable_uv");
	test = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel("myskeleton.bmw"), shaders.first, shaders.second);
	test->setTransform(glm::translate(glm::vec3(0.0f, 20.0f, 20.0f)) * glm::rotate(180.0f, glm::vec3(0, 1.0f, 0)) * glm::scale(glm::vec3(0.025, 0.025, 0.025)));

	test2 = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel("myskeleton.bmw"), shaders.first, shaders.second);
	test2->setTransform(glm::translate(glm::vec3(0.0f, 20.0f, 20.0f)) * glm::rotate(180.0f, glm::vec3(0, 1.0f, 0)) * glm::scale(glm::vec3(0.1, 0.1, 0.1)));
  
	//auto shaders = wolf::ResourceLoader::Instance().getShaders("unlit_texture");
	//test = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel("Fir_Tree.bmw"), shaders.first, shaders.second);
	//test->setTransform(glm::translate(glm::vec3(0.0f, 20.0f, 0.0f)) * glm::rotate(180.0f, glm::vec3(0, 1.0f, 0)) * glm::scale(glm::vec3(0.01, 0.01, 0.01)));

	cam = new Camera(0, 5.5, glm::vec3(0, 50.0f, 0));
	cull = cam->GetViewMatrix();
	wolf::SceneRenderer::getInstance().GenerateQuadtree(-10.0f, -10.0f, 20.0f, 20.0f);
	grid = new HexGrid(15, 15, 5.0f, 1.0f, 20.0f, wolf::ResourceLoader::Instance().getTexture("tiles/Tile_Texs_1.tga"));
	selector = new HexSelector(5.0f);
	hexPos.SetGrid(grid);
	testMove.push_back(1);
	testMove.push_back(2);
	testMove.push_back(3);
	testMove.push_back(100);
	testMove.push_back(150);
	testMove.push_back(200);
	testMove.push_back(1);
	testMove.push_back(2);
	testMove.push_back(1);
	testMove.push_back(2);

	//pathFinder->Instance()->Load("resources/objects/AIPathfindingDataTest.json");

	testhud = new wolf::Hud();
	hudProjMat = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 1000.0f) * glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void BaseScene::Update()
{
	static bool wasJustAnimated = false;
	float delta = wolf::Time::Instance().deltaTime();
	cam->Update(delta);

	testhud->Update(delta);

	test->update(delta);
	test2->update(delta);
	
	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_J))
		test2->setAnim("attack");
	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_K))
		test2->setAnim("attack2");
	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_L))
		test2->setAnim("attack3");

	int target = cam->CalculateIntersection(grid->GetHeights(), grid->GetPos(), 5.0f);
	std::vector<float> heights = grid->GetHeights();
	std::vector<glm::vec2> positions = grid->GetPos();
	if (!(target < 0))
	{
		selector->Update(target, positions.at(target), heights.at(target));
		grid->Update(target, delta);
	}
	wolf::SceneRenderer::getInstance().Update(delta, cam->GetViewMatrix());
	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_M))
	{
		hexPos.Move(testMove, 20.0f);
		test->setAnim("walk");
	}

	glm::vec3 old = hexPos.GetPos();
	hexPos.Update(delta);
	glm::vec3 dif = hexPos.GetPos() - old;
	dif.y = 0;
	float dir = 180.0f;
	if (dif.x != 0 || dif.z != 0) {
		dif = glm::normalize(dif);
		dir = atan2(dif.x, dif.z) * RAD2DEG;
		wasJustAnimated = true;
	}
	else if (wasJustAnimated) {
		wasJustAnimated = false;
		test->setAnim("idle");
	}

	std::cout << dir << "\n";

	test->setTransform(glm::translate(hexPos.GetPos()) * glm::rotate(dir, glm::vec3(0, 1.0f, 0)) * glm::scale(glm::vec3(0.025, 0.025, 0.025)));
}

void BaseScene::Render()
{
	glDepthMask(true);
	glDisable(GL_BLEND);

	wolf::SceneRenderer::getInstance().Render(cam->GetViewMatrix());
	grid->Render(cam->GetViewMatrix());
	selector->Render(cam->GetViewMatrix());
	test->render(cam->GetViewMatrix(), glm::mat4(), false);
	test2->render(cam->GetViewMatrix(), glm::mat4(), false);

	glDepthMask(false);
	glEnable(GL_BLEND);

	testhud->Render(hudProjMat);

	glDepthMask(true);
	glDisable(GL_BLEND);
}


