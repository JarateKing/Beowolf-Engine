#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"

#include "W_Time.h"
#include "W_ResourceLoader.h"
#include "W_Keybind.h"
#include "W_HudButton.h"
#include "SceneRenderer.h"
#include "StateManager.h"
#include "W_Math.h"

const float DISTANCEFACTOR = 1.0f;
const float NEARPLANE = 20.0f;
const float FARPLANE = 100.0f;

//Constructor
BaseScene::BaseScene()
{
}

//Deconstructor
BaseScene::~BaseScene()
{
	delete m_soundEngine;
	delete m_camera;
	delete m_hexgrid;
	delete m_selector;
	delete m_hud;
	delete m_characterManager;
	delete m_scoreTracker;
	delete m_pQuad;
	delete m_skybox;
	delete m_waterPlane;
	delete m_gameSaver;
}

//Initialization
void BaseScene::Init()
{
	//Initialize Systems and variables
	SetupLoader();
	SetupSoundEngine();

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	auto shaders = wolf::ResourceLoader::Instance().getShaders("animatable");

	float scale = 5.0;
	float scale2 = 0.05;

	//Set information for light projection
	m_lightProj = glm::ortho(-150.0f, 150.0f, -50.0f, 50.0f, NEARPLANE, FARPLANE);
	m_lightView = glm::lookAt(glm::vec3(-35.0f, 50.0f, -35.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	m_lightSpaceMatrix = m_lightProj * m_lightView;

	m_lightDir = glm::normalize(glm::vec3(35.0f, -50.0f, 35.0f) - glm::vec3(0.0f, 0.0f, 0.0f));
	m_pQuad = new PostProcessingQuad();
	m_hud = new wolf::Hud("resources/hud/hud.json");
	m_hudProjMat = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, 0.1f, 100.0f) * glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	m_camera = new Camera(0, 5.5, glm::vec3(0, 50.0f, -40.0));
	m_cullMatrix = m_camera->GetViewMatrix();
	wolf::SceneRenderer::getInstance().GenerateQuadtree(-10.0f, -10.0f, 20.0f, 20.0f);
	m_hexgrid = new HexGrid(15, 15, 5.0f, 1.0f, 20.0f, wolf::ResourceLoader::Instance().getTexture("tiles/Tile_Texs_1.tga"));
	m_hexgrid->SetAmbient(glm::vec4(0.999f, 0.999f, 0.899f, 1.0f));
	m_hexgrid->SetDiffuse(glm::vec4(0.988f, 1.0f, 0.788f, 1.0f));
	m_selector = new HexSelector(5.0f);
	m_characterManager = new CharacterManager(m_hexgrid, m_hud);
	m_characterManager->SetSoundEngine(m_soundEngine);
	m_characterManager->SetCamera(m_camera);
	m_hexpos.SetGrid(m_hexgrid);

	StateManager::getInstance().SetCharacterManager(m_characterManager);
	StateManager::getInstance().SetHud(m_hud);
	StateManager::getInstance().SetCamera(m_camera);
	StateManager::getInstance().SetSoundEngine(m_soundEngine);

	m_scoreTracker = new ScoreTracker(m_hud);
	m_characterManager->SetScoreTracker(m_scoreTracker);
	StateManager::getInstance().SetScoreTracker(m_scoreTracker);

	m_skybox = new Skybox();
	m_waterPlane = new Water();

	m_gameSaver = new GameSaver(m_hud);
	m_gameSaver->SetInfo(m_characterManager, m_scoreTracker, m_hexgrid);

	wolf::Keybind::Instance().addBinds("resources/cfg/keybinds.json");

	//Set Light Direction for hexgrid and charactermanager
	m_hexgrid->SetLightDir(m_lightDir);
	m_characterManager->SetLightDir(m_lightDir);
}

//Update
void BaseScene::Update()
{
	static bool wasJustAnimated = false;
	float delta = wolf::Time::Instance().deltaTime();

	//Update Camera and SoundEngine
	m_camera->Update(delta);
	m_soundEngine->SetListenerAttr(glm::vec3(-m_camera->GetPos().x, m_camera->GetPos().y, -m_camera->GetPos().z), glm::vec3(0.0f, 0.0f, 0.0f), m_camera->GetAim(), m_camera->GetUp());
	
	//Calculate the target hextile and update hexgrid with current target
	int target = m_camera->CalculateIntersection(m_hexgrid->GetHeights(), m_hexgrid->GetPos(), 5.0f);
	m_hexgrid->Update((target >= 0) ? target : -1, delta);

	wolf::SceneRenderer::getInstance().Update(delta, m_camera->GetViewMatrix());
	m_characterManager->Update(target, delta);
	m_characterManager->SetSoundEngine(m_soundEngine);
	m_characterManager->SetCamera(m_camera);

	// check for transition from player lost to player turn
	bool shouldSwap = StateManager::getInstance().GetState() == State::GamestatePlayerLost;
	StateManager::getInstance().Update(delta);
	if (shouldSwap)
		shouldSwap = StateManager::getInstance().GetState() == State::GamestatePlayerTurn;

	//Check if game is over to initiate grayscale change
	if (m_characterManager->IsGameOver() && m_grayTiming <= 2.5)
	{
		m_grayTiming += delta;
		m_grayLevel = wolf::Math::lerp(0.0, 1.0, m_grayTiming / 2.5);
		m_pQuad->SetPercentGray(m_grayLevel);	
	}
	else if (!m_characterManager->IsGameOver()) {
		m_grayTiming = 0;
		m_grayLevel = 0.0;
		m_pQuad->SetPercentGray(0);
	}

	//Restart Game if necessary
	if (shouldSwap)
		RestartGame();

	//Load Game if necessary
	bool shouldLoad = ((wolf::Keybind::Instance().getBind("loadgame") || ((wolf::HudButton*)m_hud->GetElement("MM_Load_Button"))->IsClicked()) && m_wasJustAtMainMenu);
	if (shouldLoad)
		LoadGame();

	//Set FPS Labels and update systems
	SetFPSLabels(delta);
	m_hud->Update(delta);
	m_soundEngine->UpdateSystem();
	m_waterPlane->Update(delta);
	m_gameSaver->Update(delta);

	//Set camera position for skybox
	m_skybox->SetPos(m_camera->GetPos());

	//Check if state is main menu
	m_wasJustAtMainMenu = StateManager::getInstance().GetState() == State::GamestateMainMenu;
}

//Render
void BaseScene::Render(wolf::RenderTarget target)
{
	//Render to a shadow Depth Map for shadow rendering
	if (target == wolf::RenderTarget::ShadowDepthmap)
	{
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		m_hexgrid->Render(m_camera->GetViewMatrix(), glm::mat4(), m_lightSpaceMatrix, wolf::RenderFilterOpaque, true, m_depthMapTexture, -1.0f, 100.0f);
		m_characterManager->Render(m_camera->GetViewMatrix(), glm::mat4(), m_lightSpaceMatrix, wolf::RenderFilterOpaque, true, m_depthMapTexture);
	}
	//Render water reflection
	else if (target == wolf::RenderTarget::WaterReflection)
	{
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::vec3 flippedPos = m_camera->GetPos();
		flippedPos.y = -flippedPos.y + 10;
		m_skybox->SetPos(flippedPos);

		m_skybox->Render(m_camera->GetVerticalInverse(5), wolf::RenderFilterOpaque);
		m_characterManager->Render(m_camera->GetVerticalInverse(5), glm::mat4(), m_lightSpaceMatrix, wolf::RenderFilterOpaque, false, m_depthMapTexture);
		m_hexgrid->Render(m_camera->GetVerticalInverse(5), glm::mat4(), m_lightSpaceMatrix, wolf::RenderFilterOpaque, false, m_depthMapTexture, 4.25f, 100.0f);
	}
	//Render water refraction
	else if (target == wolf::RenderTarget::WaterRefraction)
	{
		m_skybox->Render(m_camera->GetViewMatrix(), wolf::RenderFilterOpaque);
		m_hexgrid->Render(m_camera->GetViewMatrix(), glm::mat4(), m_lightSpaceMatrix, wolf::RenderFilterOpaque, false, m_depthMapTexture, -1.0f, 6.0f);
	}
	//Render water fog
	else if (target == wolf::RenderTarget::WaterFog)
	{
		m_hexgrid->Render(m_camera->GetViewMatrix(), glm::mat4(), m_lightSpaceMatrix, wolf::RenderFilterOpaque, false, m_depthMapTexture, -1.0f, 6.0f);
	}
	//Render full scene to post processing texture
	//Used as base for post processing effects
	else if (target == wolf::RenderTarget::PostProcessing)
	{
		// Opaque
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//tQuad->Render(m_camera->GetViewMatrix(), glm::mat4(), wolf::RenderFilterOpaque, false, m_depthMapTexture);
		m_hexgrid->Render(m_camera->GetViewMatrix(), glm::mat4(), m_lightSpaceMatrix, wolf::RenderFilterOpaque, false, m_depthMapTexture, -1.0f, 100.0f);
		m_selector->Render(m_camera->GetViewMatrix());
		m_characterManager->Render(m_camera->GetViewMatrix(), glm::mat4(), m_lightSpaceMatrix, wolf::RenderFilterOpaque, false, m_depthMapTexture);

		m_skybox->SetPos(m_camera->GetPos());
		m_skybox->Render(m_camera->GetViewMatrix(), wolf::RenderFilterOpaque);

		// Transparent
		glEnable(GL_BLEND);

		m_characterManager->Render(m_camera->GetViewMatrix(), glm::mat4(), m_lightSpaceMatrix, wolf::RenderFilterTransparent, false, m_depthMapTexture);

		m_waterPlane->Render(m_camera->GetViewMatrix(), wolf::RenderFilterTransparent, m_reflectionTexture, m_refractionTexture, m_fogTexture);

		// Depthless
		glDepthMask(false);

		m_hexgrid->Render(m_camera->GetViewMatrix(), glm::mat4(), m_lightSpaceMatrix, wolf::RenderFilterTransparent, false, m_depthMapTexture, -1.0f, 100.0f);

		// Additive
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		m_hexgrid->Render(m_camera->GetViewMatrix(), glm::mat4(), m_lightSpaceMatrix, wolf::RenderFilterAdditive, false, m_depthMapTexture, -1.0f, 100.0f);
		m_characterManager->Render(m_camera->GetViewMatrix(), glm::mat4(), m_lightSpaceMatrix, wolf::RenderFilterAdditive, false, m_depthMapTexture);

		// Done
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}
	//Render character depth map only
	//Used for character outlines through objects
	else if (target == wolf::RenderTarget::Characters)
	{
		m_characterManager->Render(m_camera->GetViewMatrix(), glm::mat4(), m_camera->GetViewMatrix(), wolf::RenderFilterOpaque, true, m_depthMapTexture2);
	}
	//Render depth map used for Depth of Field
	else if (target == wolf::RenderTarget::DepthFieldMap)
	{
		m_hexgrid->Render(m_camera->GetViewMatrix(), glm::mat4(), m_camera->GetViewMatrix(), wolf::RenderFilterOpaque, true, m_depthMapTexture, -1.0f, 100.0f);
		m_characterManager->Render(m_camera->GetViewMatrix(), glm::mat4(), m_camera->GetViewMatrix(), wolf::RenderFilterOpaque, true, m_depthMapTexture);
	}
	//Grayscale Post Processing Effect
	else if(target == wolf::RenderTarget::GrayScale)
	{
		m_pQuad->Render(m_camera->GetViewMatrix(), wolf::RenderFilterOpaque, m_postProcessTexture, m_postProcessBlurTexture, m_fogTexture, m_depthMapTexture2, "GrayScale");
	}
	//Blurring Post Processing Effect
	else if (target == wolf::RenderTarget::Blur)
	{
		m_pQuad->Render(m_camera->GetViewMatrix(), wolf::RenderFilterOpaque, m_postProcessTexture, m_postProcessBlurTexture, m_fogTexture, m_depthMapTexture2, "Blur");
	}
	//Depth of Field Post Processing Effect
	//Using both the main Post Processing Texture and Blurring
	else if (target == wolf::RenderTarget::DepthOfField)
	{
		m_pQuad->Render(m_camera->GetViewMatrix(), wolf::RenderFilterOpaque, m_postProcessTexture, m_postProcessBlurTexture, m_depthMapTexture, m_depthMapTexture2, "DepthOfField");
	}
	//Render HUD Separately so it doesnt get effected by Depth of Field
	else if (target == wolf::RenderTarget::HUD)
	{
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glDepthMask(false);

		m_hud->Render(m_hudProjMat);

		glDepthMask(true);
		glDisable(GL_BLEND);
	}
}
//Sets RenderTarget Textures to be used so they can be called on later
void BaseScene::SetTex(wolf::RenderTarget target, unsigned int tex)
{
	if (target == wolf::RenderTarget::ShadowDepthmap)
		m_depthMapTexture = tex;
	else if (target == wolf::RenderTarget::WaterReflection)
		m_reflectionTexture = tex;
	else if (target == wolf::RenderTarget::PostProcessing)
		m_postProcessTexture = tex;
	else if (target == wolf::RenderTarget::Blur)
		m_postProcessBlurTexture = tex;
	else if (target == wolf::RenderTarget::DepthFieldMap)
		m_postProcessDepthMap = tex;
	else if (target == wolf::RenderTarget::WaterRefraction)
		m_refractionTexture = tex;
	else if (target == wolf::RenderTarget::WaterFog)
		m_fogTexture = tex;
	else if (target == wolf::RenderTarget::Cutouts)
		m_depthMapTexture2 = tex;
}

//Sets up loading of models
void BaseScene::SetupLoader() {
	LoadingScreen loader;
	loader.AddModel("potion.bmw");
	loader.AddModel("sword1.bmw");
	loader.AddModel("shield.bmw");
	loader.AddModel("Fir_Tree.bmw");
	loader.AddModel("Oak_Tree.bmw");
	loader.AddModel("Palm_Tree.bmw");
	loader.AddModel("Poplar_Tree.bmw");
	loader.AddModel("units/mychamp.bmw");
	loader.AddModel("units/mygiant.bmw");
	loader.AddModel("units/mylich.bmw");
	loader.AddModel("units/myskeleton.bmw");
	loader.AddModel("units/myfleshlobber.bmw");
	loader.Load();
}

//Adds all sounds to sound engine
void BaseScene::SetupSoundEngine() {
	m_soundEngine = new wolf::SoundEngine();
	m_soundEngine->InitSystem();
	m_soundEngine->AddSound("resources/sound/Base_Music/old_city_theme.ogg", "base_theme", true);
	m_soundEngine->AddSound("resources/sound/Death/enemy_death.wav", "enemy_death", false);
	m_soundEngine->AddSound("resources/sound/Death/hero_death.flac", "hero_death", false);
	m_soundEngine->AddSound("resources/sound/Game_Over/Jingle_Lose_00.wav", "lose_jingle", true);
	m_soundEngine->AddSound("resources/sound/Game_Start/Jingle_Achievement_00.wav", "start_jingle", true);
	m_soundEngine->AddSound("resources/sound/Hits/Socapex-Swordsmall.wav", "hit1", true);
	m_soundEngine->AddSound("resources/sound/Hits/Socapex-Swordsmall_1.wav", "hit2", true);
	m_soundEngine->AddSound("resources/sound/Hits/Socapex-Swordsmall_2.wav", "hit3", true);
	m_soundEngine->AddSound("resources/sound/Hits/Socapex-Swordsmall_3.wav", "hit4", true);
	m_soundEngine->AddSound("resources/sound/Item_Pickup/Inventory_Open_00.wav", "item_pickup", false);
	m_soundEngine->AddSound("resources/sound/Movement/wooden-stairs-in-1.flac", "movement1", false);
	m_soundEngine->AddSound("resources/sound/Movement/wooden-stairs-out-1.flac", "movement2", false);

	m_soundEngine->Play3DSound("base_theme", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), true);
	m_soundEngine->UpdateSystem();
}

//Sets FPS Labels if necessary
void BaseScene::SetFPSLabels(float delta) {
	double fpsValue = round(wolf::Time::Instance().getFPS() * 10.0) / 10.0;
	std::string fpsString = std::to_string(fpsValue);
	m_hud->SetVar("deltaMS", std::to_string(delta * 1000));
	m_hud->SetVar("fps", fpsString.substr(0, fpsString.find('.') + 2));
}

//Method to Load Game from JSON File
void BaseScene::LoadGame() {
	if(m_hexgrid != NULL)
		delete m_hexgrid;

	m_hexgrid = new HexGrid(15, 15, 5.0f, 1.0f, 20.0f, wolf::ResourceLoader::Instance().getTexture("tiles/Tile_Texs_1.tga"), "savefile.json");
	m_hexgrid->SetAmbient(glm::vec4(0.999f, 0.999f, 0.899f, 1.0f));
	m_hexgrid->SetDiffuse(glm::vec4(0.988f, 1.0f, 0.788f, 1.0f));
	m_hexgrid->SetLightDir(m_lightDir);

	if (m_selector != NULL)
		delete m_selector;

	m_selector = new HexSelector(5.0f);

	if (m_characterManager != NULL)
		delete m_characterManager;

	m_characterManager = new CharacterManager(m_hexgrid, m_hud, "savefile.json");
	m_characterManager->SetLightDir(m_lightDir);

	StateManager::getInstance().SetCharacterManager(m_characterManager);

	m_characterManager->SetScoreTracker(m_scoreTracker);
	m_characterManager->SetSoundEngine(m_soundEngine);

	m_gameSaver->SetInfo(m_characterManager, m_scoreTracker, m_hexgrid);
}

//Method to Restart Game Upon a Game Loss
void BaseScene::RestartGame() {
	delete m_camera;
	m_camera = new Camera(0, 5.5, glm::vec3(0, 50.0f, -40.0));
	m_cullMatrix = m_camera->GetViewMatrix();

	delete m_hexgrid;
	m_hexgrid = new HexGrid(15, 15, 5.0f, 1.0f, 20.0f, wolf::ResourceLoader::Instance().getTexture("tiles/Tile_Texs_1.tga"));

	delete m_selector;
	m_selector = new HexSelector(5.0f);

	delete m_characterManager;
	m_characterManager = new CharacterManager(m_hexgrid, m_hud);
	m_hexpos.SetGrid(m_hexgrid);

	StateManager::getInstance().SetCharacterManager(m_characterManager);
	StateManager::getInstance().SetHud(m_hud);
	StateManager::getInstance().SetCamera(m_camera);

	m_scoreTracker->SetScore(0);
	m_characterManager->SetScoreTracker(m_scoreTracker);
	m_characterManager->SetSoundEngine(m_soundEngine);

	m_gameSaver->SetInfo(m_characterManager, m_scoreTracker, m_hexgrid);
}