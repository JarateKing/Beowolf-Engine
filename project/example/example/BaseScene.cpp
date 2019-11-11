#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>
#include "W_Math.h"
#include "W_ProjectionMatrix.h"
#include <iomanip>
#include "sound/W_SoundEngine.h"
#include "beowolf/hexGrid/HexGrid.h"
#include "camera/Camera.h"
#include "DebugCube.h"
#include "SceneRenderer.h"
#include "W_Time.h"

const float DISTANCEFACTOR = 1.0f;
wolf::SoundEngine SE;
static Camera* cam;
static glm::mat4 cull;
static HexGrid* grid;

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

	//HexGrid grid(5, 5, 10.0f);
	//grid.PrintOutLoc();

	glEnable(GL_DEPTH_TEST);
	cam = new Camera(0, 5.5, glm::vec3(0, 50.0f, 0));
	cull = cam->GetViewMatrix();

	/*for (int i = 0; i < 30; i++)
	{
		wolf::SceneRenderer::getInstance().AddNode((wolf::Node*)new wolf::DebugCube());
	}*/
	wolf::SceneRenderer::getInstance().GenerateQuadtree(-10.0f, -10.0f, 20.0f, 20.0f);
	grid = new HexGrid(50 , 50, 5.0f, 1.0f, 20.0f, "resources/textures/tiles/Tile_Texs_1.tga");
}

void BaseScene::Update()
{
	//Updates the Position of listener of 3D sound, useless for stereo sound
	//FMOD_VECTOR pos = SE.GetListenerPos();
	//glm::vec3 listPos{ pos.x, pos.y, pos.z };
	//listPos.x = listPos.x + 0.1f;
	//SE.SetListenerAttr(listPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//SE.UpdateSystem();
	float delta = wolf::Time::Instance().deltaTime();
	cam->Update(delta);
	wolf::SceneRenderer::getInstance().Update(delta, cam->GetViewMatrix());
}

void BaseScene::Render()
{
	wolf::SceneRenderer::getInstance().Render(cam->GetViewMatrix());
	grid->Render(cam->GetViewMatrix());
}


