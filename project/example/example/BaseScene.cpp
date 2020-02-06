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
#include "W_RNG.h"
#include "StateManager.h"
#include "characterUnits/CharacterInfoHub.h"
#include "characterUnits/ScoreTracker.h"

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
CharacterInfoHub cHub;
ScoreTracker* scoreTracker;

wolf::BMWModel* test;
wolf::BMWModel* test2;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	auto shaders = wolf::ResourceLoader::Instance().getShaders("animatable");

	//Hub Testing
	//cHub.AddCharacter("Characters/hero1.json", "WingedKnight");
	//cHub.AddEnemyType("Characters/enemyLight.json", "Enemy1");
	//cHub.AddItemType("Items/potion.json");
	//cHub.DamageCharacter("WingedKnight", 50);
	//cHub.DamageEnemy("Enemy1", 50);
	//cHub.GivePlayerItem("WingedKnight", "Potion");

	float scale = 5.0;
	float scale2 = 0.05;
	//test = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel("Knights/RedKnightAlternative.bmw"), shaders.first, shaders.second);
	//test2 = new wolf::BMWModel(wolf::ResourceLoader::Instance().getModel("lich/FreeLich.bmw"), shaders.first, shaders.second);
	//test->setTransform(glm::translate(glm::vec3(0, -100, 50)) * glm::rotate(90.0f, 0.0f, 1.0f, 0.0f) * glm::rotate(180.0f, 0.0f, 0.0f, 1.0f) * glm::scale(glm::vec3(scale2, scale2, scale2)));
	//test2->setTransform(glm::translate(glm::vec3(0, -100, 60)) * glm::scale(glm::vec3(scale, scale, scale)));

	testhud = new wolf::Hud("resources/hud/hud.json");
	hudProjMat = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, 0.1f, 100.0f) * glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	cam = new Camera(0, 5.5, glm::vec3(0, 50.0f, -40.0));
	cull = cam->GetViewMatrix();
	wolf::SceneRenderer::getInstance().GenerateQuadtree(-10.0f, -10.0f, 20.0f, 20.0f);
	grid = new HexGrid(15, 15, 5.0f, 1.0f, 20.0f, wolf::ResourceLoader::Instance().getTexture("tiles/Tile_Texs_1.tga"));
	selector = new HexSelector(5.0f);
	cManager = new CharacterManager(grid, testhud);
	hexPos.SetGrid(grid);

	StateManager::getInstance().SetCharacterManager(cManager);
	StateManager::getInstance().SetHud(testhud);
	StateManager::getInstance().SetCamera(cam);

	scoreTracker = new ScoreTracker(testhud);
	cManager->SetScoreTracker(scoreTracker);
}

void BaseScene::Update()
{
	static bool wasJustAnimated = false;
	float delta = wolf::Time::Instance().deltaTime();
	cam->Update(delta);

	//test->update(delta);
	
	int target = cam->CalculateIntersection(grid->GetHeights(), grid->GetPos(), 5.0f);
	std::vector<float> heights = grid->GetHeights();
	std::vector<glm::vec2> positions = grid->GetPos();
	if (!(target < 0))
	{
		selector->Update(target, positions.at(target), heights.at(target));
		grid->Update(target, delta);
	}
	wolf::SceneRenderer::getInstance().Update(delta, cam->GetViewMatrix());
	cManager->Update(target, delta);

	bool shouldSwap = StateManager::getInstance().GetState() == State::GamestatePlayerLost;
	StateManager::getInstance().Update(delta);
	if (shouldSwap)
		shouldSwap = StateManager::getInstance().GetState() == State::GamestatePlayerTurn;

	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_Y))
		shouldSwap = true;

	if (shouldSwap) {
		std::cout << "YEAH!!!\n";
	}

	double fpsValue = round(wolf::Time::Instance().getFPS() * 10.0) / 10.0;
	std::string fpsString = std::to_string(fpsValue);
	testhud->SetVar("deltaMS", std::to_string(delta * 1000));
	testhud->SetVar("fps", fpsString.substr(0, fpsString.find('.') + 2));
	testhud->Update(delta);
}

void BaseScene::Render()
{
	// Opaque
	glDepthMask(true);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	grid->Render(cam->GetViewMatrix(), wolf::RenderFilterOpaque);
	selector->Render(cam->GetViewMatrix());
	cManager->Render(cam->GetViewMatrix(), glm::mat4(), wolf::RenderFilterOpaque);
	
	//test->render(cam->GetViewMatrix(), glm::mat4(), wolf::RenderFilterOpaque);
	//test2->render(cam->GetViewMatrix(), glm::mat4(), wolf::RenderFilterOpaque);

	// Transparent
	glEnable(GL_BLEND);

	cManager->Render(cam->GetViewMatrix(), glm::mat4(), wolf::RenderFilterTransparent);
	
	//test->render(cam->GetViewMatrix(), glm::mat4(), wolf::RenderFilterTransparent);
	//test2->render(cam->GetViewMatrix(), glm::mat4(), wolf::RenderFilterTransparent);

	// Depthless
	glDepthMask(false);

	testhud->Render(hudProjMat);

	// Additive
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	cManager->Render(cam->GetViewMatrix(), glm::mat4(), wolf::RenderFilterAdditive);

	// Done
	glDepthMask(true);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}
