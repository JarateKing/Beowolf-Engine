#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>
#include "W_Math.h"
#include "W_ProjectionMatrix.h"
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
#include "shadows/TestQuad.h"
#include "camera/Skybox.h"
#include "camera/Water.h"

const float DISTANCEFACTOR = 1.0f;
wolf::SoundEngine* SE;
static Camera* cam;
static glm::mat4 cull;
static HexGrid* grid;
wolf::MousePos mouse;
static HexSelector* selector;
week2::ComponentHexPos hexPos;
std::vector<int> testMove;
wolf::Hud* testhud;
glm::mat4 hudProjMat;
glm::vec3 lightDir;
static CharacterManager* cManager;
CharacterInfoHub cHub;
ScoreTracker* scoreTracker;
TestQuad* tQuad;
unsigned int depthMapTexture;
unsigned int reflectionTexture;
unsigned int refractionTexture;
Skybox* skybox;
Water* water;

wolf::BMWModel* test;
wolf::BMWModel* test2;
float distance = -1000.0f;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	SE = new wolf::SoundEngine();
	SE->InitSystem();
	SE->AddSound("resources/sound/Base_Music/old_city_theme.ogg", "base_theme", true);
	SE->AddSound("resources/sound/Death/enemy_death.wav", "enemy_death", false);
	SE->AddSound("resources/sound/Death/hero_death.flac", "hero_death", false);
	SE->AddSound("resources/sound/Game_Over/Jingle_Lose_00.wav", "lose_jingle", true);
	SE->AddSound("resources/sound/Game_Start/Jingle_Achievement_00.wav", "start_jingle", true);
	SE->AddSound("resources/sound/Hits/Socapex-Swordsmall.wav", "hit1", true);
	SE->AddSound("resources/sound/Hits/Socapex-Swordsmall_1.wav", "hit2", true);
	SE->AddSound("resources/sound/Hits/Socapex-Swordsmall_2.wav", "hit3", true);
	SE->AddSound("resources/sound/Hits/Socapex-Swordsmall_3.wav", "hit4", true);
	SE->AddSound("resources/sound/Item_Pickup/Inventory_Open_00.wav", "item_pickup", false);
	SE->AddSound("resources/sound/Movement/wooden-stairs-in-1.flac", "movement1", false);
	SE->AddSound("resources/sound/Movement/wooden-stairs-out-1.flac", "movement2", false);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	auto shaders = wolf::ResourceLoader::Instance().getShaders("animatable");

	float scale = 5.0;
	float scale2 = 0.05;
	
	lightDir = glm::normalize(glm::vec3(35.0f, -50.0f, 35.0f) - glm::vec3(0.0f, 0.0f, 0.0f));
	tQuad = new TestQuad();
	testhud = new wolf::Hud("resources/hud/hud.json");
	hudProjMat = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, 0.1f, 100.0f) * glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	cam = new Camera(0, 5.5, glm::vec3(0, 50.0f, -40.0));
	cull = cam->GetViewMatrix();
	wolf::SceneRenderer::getInstance().GenerateQuadtree(-10.0f, -10.0f, 20.0f, 20.0f);
	grid = new HexGrid(15, 15, 5.0f, 1.0f, 20.0f, wolf::ResourceLoader::Instance().getTexture("tiles/Tile_Texs_1.tga"));
	grid->SetAmbient(glm::vec4(0.999f, 0.999f, 0.899f, 1.0f));
	grid->SetDiffuse(glm::vec4(0.988f, 1.0f, 0.788f, 1.0f));
	selector = new HexSelector(5.0f);
	cManager = new CharacterManager(grid, testhud);
	cManager->SetSoundEngine(SE);
	cManager->SetCamera(cam);
	hexPos.SetGrid(grid);

	StateManager::getInstance().SetCharacterManager(cManager);
	StateManager::getInstance().SetHud(testhud);
	StateManager::getInstance().SetCamera(cam);
	StateManager::getInstance().SetSoundEngine(SE);

	scoreTracker = new ScoreTracker(testhud);
	cManager->SetScoreTracker(scoreTracker);
	SE->Play3DSound("base_theme", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), true);
	SE->UpdateSystem();

	skybox = new Skybox();
	water = new Water();
}

void BaseScene::Update()
{
	SE->SetListenerAttr(glm::vec3(-cam->GetPos().x, cam->GetPos().y, -cam->GetPos().z), glm::vec3(0.0f, 0.0f, 0.0f), cam->GetAim(), cam->GetUp());

	static bool wasJustAnimated = false;
	float delta = wolf::Time::Instance().deltaTime();
	cam->Update(delta);
	
	int target = cam->CalculateIntersection(grid->GetHeights(), grid->GetPos(), 5.0f);
	std::vector<float> heights = grid->GetHeights();
	std::vector<glm::vec2> positions = grid->GetPos();
	if (!(target < 0))
	{
		//selector->Update(target, positions.at(target), heights.at(target));
		grid->Update(target, delta);
	}
	wolf::SceneRenderer::getInstance().Update(delta, cam->GetViewMatrix());
	cManager->Update(target, delta);

	bool shouldSwap = StateManager::getInstance().GetState() == State::GamestatePlayerLost;
	StateManager::getInstance().Update(delta);
	if (shouldSwap)
		shouldSwap = StateManager::getInstance().GetState() == State::GamestatePlayerTurn;

	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_T))
		cManager->PrintCharacterTilePos();

	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_Y))
		shouldSwap = true;

	if (wolf::Input::Instance().isKeyHeld(INPUT_KB_1))
		lightDir.x += 0.1f;

	if (wolf::Input::Instance().isKeyHeld(INPUT_KB_2))
		lightDir.y += 0.1f;

	if (wolf::Input::Instance().isKeyHeld(INPUT_KB_3))
		lightDir.z += 0.1f;

	if (wolf::Input::Instance().isKeyHeld(INPUT_KB_4))
		lightDir.x -= 0.1f;

	if (wolf::Input::Instance().isKeyHeld(INPUT_KB_5))
		lightDir.y -= 0.1f;

	if (wolf::Input::Instance().isKeyHeld(INPUT_KB_6))
		lightDir.z -= 0.1f;

	if (shouldSwap) {

		delete cam;
		cam = new Camera(0, 5.5, glm::vec3(0, 50.0f, -40.0));
		cull = cam->GetViewMatrix();

		delete grid;
		grid = new HexGrid(15, 15, 5.0f, 1.0f, 20.0f, wolf::ResourceLoader::Instance().getTexture("tiles/Tile_Texs_1.tga"));

		delete selector;
		selector = new HexSelector(5.0f);

		delete cManager;
		cManager = new CharacterManager(grid, testhud);
		hexPos.SetGrid(grid);

		StateManager::getInstance().SetCharacterManager(cManager);
		StateManager::getInstance().SetHud(testhud);
		StateManager::getInstance().SetCamera(cam);

		scoreTracker->SetScore(0);
		cManager->SetScoreTracker(scoreTracker);
		cManager->SetSoundEngine(SE);
	}

	double fpsValue = round(wolf::Time::Instance().getFPS() * 10.0) / 10.0;
	std::string fpsString = std::to_string(fpsValue);
	testhud->SetVar("deltaMS", std::to_string(delta * 1000));
	testhud->SetVar("fps", fpsString.substr(0, fpsString.find('.') + 2));
	testhud->Update(delta);

	grid->SetLightDir(lightDir);
	cManager->SetLightDir(lightDir);

	SE->UpdateSystem();

	skybox->SetPos(cam->GetPos());

	water->Update(delta);
	//water->SetPos(cam->GetPos());
}

void BaseScene::Render(RenderTarget target)
{
	float near_plane = 20.0f, far_plane = 100.0f;
	glm::mat4 lightProj = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(glm::vec3(-35.0f, 50.0f, -35.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProj * lightView;

	if (target == RenderTarget::ShadowDepthmap)
	{
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		grid->Render(cam->GetViewMatrix(), lightSpaceMatrix, wolf::RenderFilterOpaque, true, depthMapTexture, -1.0f, 100.0f);
		cManager->Render(cam->GetViewMatrix(), glm::mat4(), lightSpaceMatrix, wolf::RenderFilterOpaque, true, depthMapTexture);
	}
	else if (target == RenderTarget::WaterReflection)
	{
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::vec3 flippedPos = cam->GetPos();
		flippedPos.y = -flippedPos.y + 10;
		skybox->SetPos(flippedPos);

		skybox->Render(cam->GetVerticalInverse(5), wolf::RenderFilterOpaque);
		cManager->Render(cam->GetVerticalInverse(5), glm::mat4(), lightSpaceMatrix, wolf::RenderFilterOpaque, false, depthMapTexture);
		grid->Render(cam->GetVerticalInverse(5), lightSpaceMatrix, wolf::RenderFilterOpaque, false, depthMapTexture, 4.25f, 100.0f);
	}
	else if (target == RenderTarget::WaterRefraction)
	{
		grid->Render(cam->GetViewMatrix(), lightSpaceMatrix, wolf::RenderFilterOpaque, false, depthMapTexture, -1.0f, 100.0f);
	}
	else
	{
		// Opaque
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//tQuad->Render(cam->GetViewMatrix(), glm::mat4(), wolf::RenderFilterOpaque, false, depthMapTexture);
		grid->Render(cam->GetViewMatrix(), lightSpaceMatrix, wolf::RenderFilterOpaque, false, depthMapTexture, -1.0f, 100.0f);
		selector->Render(cam->GetViewMatrix());
		cManager->Render(cam->GetViewMatrix(), glm::mat4(), lightSpaceMatrix, wolf::RenderFilterOpaque, false, depthMapTexture);

		skybox->SetPos(cam->GetPos());
		skybox->Render(cam->GetViewMatrix(), wolf::RenderFilterOpaque);

		// Transparent
		glEnable(GL_BLEND);

		cManager->Render(cam->GetViewMatrix(), glm::mat4(), lightSpaceMatrix, wolf::RenderFilterTransparent, false, depthMapTexture);

		water->Render(cam->GetViewMatrix(), wolf::RenderFilterTransparent, reflectionTexture);

		// Depthless
		glDepthMask(false);

		testhud->Render(hudProjMat);

		// Additive
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		cManager->Render(cam->GetViewMatrix(), glm::mat4(), lightSpaceMatrix, wolf::RenderFilterAdditive, false, depthMapTexture);

		// Done
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}
}

void BaseScene::SetTex(RenderTarget target, unsigned int tex)
{
	if (target == RenderTarget::ShadowDepthmap)
		depthMapTexture = tex;
	else if (target == RenderTarget::WaterReflection)
		reflectionTexture = tex;
	else if (target == RenderTarget::WaterRefraction)
		refractionTexture = tex;
}
