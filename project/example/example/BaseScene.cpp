#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>
#include "W_Math.h"
#include "W_ProjectionMatrix.h"
#include <iomanip>
#include "sound/W_SoundEngine.h"
#include "beowolf/hexGrid/HexGrid.h"

const float DISTANCEFACTOR = 1.0f;
wolf::SoundEngine SE;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	/////////////////////////
	//Demo for Audio System//
	/////////////////////////

	//Listener and sound info
	//glm::vec3 listFor{ 0.0f, 0.0f, 1.0f };
	//glm::vec3 listUp{ 0.0f, 1.0f, 0.0f };
	//glm::vec3 listVel{ 10.0f, 0.0f, 0.0f };
	//glm::vec3 listPos{ -50.0f, 0.0f, 0.0f };
	//glm::vec3 soundPos{ 0.0f, 0.0f, 0.0f };
	//glm::vec3 soundVel{ 100.0f, 0.0f, 0.0f };

	//Initialize System
	//SE.InitSystem();

	//Creates loop sound, sets listener pos
	//SE.AddSound("media/drumloop.wav", "drumloop", false);
	//SE.PlayLoopSound("drumloop");
	//SE.Change3DSoundPos("drumloop", soundPos, soundVel);
	//SE.SetListenerAttr(listPos, listVel, listFor, listUp);

	//Use this example for standard base sound
	//SE.AddSound("media/drumloop.wav", "drumloop", true);
	//SE.PlayBasicSound("drumloop");

	HexGrid grid(5, 5, 10.0f);
	//grid.PrintOutLoc();
}

void BaseScene::Update()
{
	//Updates the Position of listener of 3D sound, useless for stereo sound
	FMOD_VECTOR pos = SE.GetListenerPos();
	glm::vec3 listPos{ pos.x, pos.y, pos.z };
	listPos.x = listPos.x + 0.1f;
	SE.SetListenerAttr(listPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	SE.UpdateSystem();
}

void BaseScene::Render()
{
}


