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
#include "W_Input_Keys.h"
#include "W_ResourceLoader.h"
#include "camera/HexSelector.h"
#include "ComponentHexPos.h"
#include "AIPathfinder.h"
#include <cmath>
#include "W_Hud.h"
#include "W_ProjectionMatrix.h"
#include "characterUnits/CharacterManager.h"

const float DISTANCEFACTOR = 1.0f;
wolf::SoundEngine SE;
static Camera* cam;
static glm::mat4 cull;
static HexGrid* grid;
wolf::MousePos mouse;
static HexSelector* selector;
week2::ComponentHexPos hexPos;
std::vector<int> testMove;
wolf::Hud* testhud;
glm::mat4 hudProjMat;
static CharacterManager* cManager;

wolf::BMWModel* test;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	auto shaders = wolf::ResourceLoader::Instance().getShaders("animatable");
	float scale = 15.0;
	test = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel("lich/FreeLich.bmw"), shaders.first, shaders.second);
	test->setTransform(glm::translate(glm::vec3(0, 0, 50)) * glm::scale(glm::vec3(scale, scale, scale)));

	cam = new Camera(0, 5.5, glm::vec3(0, 50.0f, 0));
	cull = cam->GetViewMatrix();
	wolf::SceneRenderer::getInstance().GenerateQuadtree(-10.0f, -10.0f, 20.0f, 20.0f);
	grid = new HexGrid(15, 15, 5.0f, 1.0f, 20.0f, wolf::ResourceLoader::Instance().getTexture("tiles/Tile_Texs_1.tga"));
	selector = new HexSelector(5.0f);
	cManager = new CharacterManager(grid);
	hexPos.SetGrid(grid);

	for (int i = 0; i < 102; i++)
		cManager->SpawnItem(i);

	testhud = new wolf::Hud("resources/hud/hud.json");
	hudProjMat = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, 0.1f, 100.0f) * glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void BaseScene::Update()
{
	static bool wasJustAnimated = false;
	float delta = wolf::Time::Instance().deltaTime();
	cam->Update(delta);

	test->update(delta);

	double fpsValue = round(wolf::Time::Instance().getFPS() * 10.0) / 10.0;
	std::string fpsString = std::to_string(fpsValue);
	testhud->SetVar("deltaMS", std::to_string(delta * 1000));
	testhud->SetVar("fps", fpsString.substr(0, fpsString.find('.') + 2));
	testhud->Update(delta);

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
	}

	cManager->Update(delta);
}

void BaseScene::Render()
{
	glDepthMask(true);
	glDisable(GL_BLEND);

	wolf::SceneRenderer::getInstance().Render(cam->GetViewMatrix());
	grid->Render(cam->GetViewMatrix());
	selector->Render(cam->GetViewMatrix());
	
	cManager->Render(cam->GetViewMatrix(), glm::mat4(), true);

	glDepthMask(false);
	glEnable(GL_BLEND);

	testhud->Render(hudProjMat);
	cManager->Render(cam->GetViewMatrix(), glm::mat4(), false);

	glDepthMask(true);
	glDisable(GL_BLEND);
}


