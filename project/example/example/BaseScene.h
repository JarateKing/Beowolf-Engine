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
	BaseScene();
	void Init();
	void Update();
	void Render(RenderTarget target);
	void SetTex(RenderTarget target, unsigned int tex);
private:
	void SetupLoader();
	void SetupSoundEngine();
	void SetFPSLabels(float delta);
	void LoadGame();
	void RestartGame();

	wolf::SoundEngine* m_soundEngine;
	Camera* m_camera;
	glm::mat4 m_cullMatrix;
	HexGrid* m_hexgrid;
	wolf::MousePos m_mouse;
	HexSelector* m_selector;
	week2::ComponentHexPos m_hexpos;
	wolf::Hud* m_hud;
	glm::mat4 m_hudProjMat;
	glm::vec3 m_lightDir;
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
	bool m_wasJustAtMainMenu = true;
};

#endif
