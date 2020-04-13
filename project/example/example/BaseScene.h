//-----------------------------------------------------------------------------
// File:			examples.h
// Original Author:	Gordon Wood
//
// Prototypes for all the example functions
//-----------------------------------------------------------------------------
#ifndef BASESCENE_H
#define BASESCENE_H

#include "Scene.h"

#include "W_Common.h"
#include "W_Input.h"
#include "W_Hud.h"
#include "sound/W_SoundEngine.h"
#include "beowolf/hexGrid/HexGrid.h"
#include "camera/HexSelector.h"
#include "ComponentHexPos.h"
#include "camera/Camera.h"
#include "camera/Skybox.h"
#include "camera/Water.h"
#include "LoadingScreen.h"
#include "post/PostProcessingQuad.h"
#include "GameSaver.h"
#include "characterUnits/CharacterManager.h"
#include "characterUnits/CharacterInfoHub.h"
#include "characterUnits/ScoreTracker.h"

class BaseScene : public Scene
{
public:
	//Public Methods
	BaseScene();
	void Init();
	void Update();
	void Render(wolf::RenderTarget target);
	void SetTex(wolf::RenderTarget target, unsigned int tex);

private:
	//Private Methods
	void SetupLoader();
	void SetupSoundEngine();
	void SetFPSLabels(float delta);
	void LoadGame();
	void RestartGame();

	//Private Objects
	wolf::SoundEngine* m_soundEngine;
	Camera* m_camera;
	glm::mat4 m_cullMatrix;
	HexGrid* m_hexgrid;
	HexSelector* m_selector;
	wolf::MousePos m_mouse;
	week2::ComponentHexPos m_hexpos;
	wolf::Hud* m_hud;

	//Private Variables
	bool m_wasJustAtMainMenu = true;
	float m_grayLevel = 0.0f;
	float m_grayTiming = 0.0f;
	glm::vec3 m_lightDir;
	glm::mat4 m_hudProjMat;
	glm::mat4 m_lightProj;
	glm::mat4 m_lightView;
	glm::mat4 m_lightSpaceMatrix;
	CharacterManager* m_characterManager;
	CharacterInfoHub m_characterHub;
	ScoreTracker* m_scoreTracker;
	PostProcessingQuad* m_pQuad;
	unsigned int m_depthMapTexture;
	unsigned int m_depthMapTexture2;
	unsigned int m_reflectionTexture;
	unsigned int m_postProcessTexture;
	unsigned int m_postProcessBlurTexture;
	unsigned int m_refractionTexture;
	unsigned int m_fogTexture;
	unsigned int m_postProcessDepthMap;
	Skybox* m_skybox;
	Water* m_waterPlane;
	GameSaver* m_gameSaver;
};

#endif
