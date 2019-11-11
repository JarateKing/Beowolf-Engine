#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>
#include <iomanip>
#include "sound/W_SoundEngine.h"
#include "beowolf/hexGrid/HexGrid.h"
#include "camera/Camera.h"
#include "DebugCube.h"
#include "SceneRenderer.h"
#include "W_Time.h"
#include "W_Math.h"

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
	//for (float i = 0.0f; i <= 1.000001f; i += 0.1f) {
		//std::cout << std::fixed << std::setprecision(1) << i;
		//std::cout << " = in: " << std::fixed << std::setprecision(6) << wolf::Math::easeIn(i);
		//std::cout << " - out: " << std::fixed << std::setprecision(6) << wolf::Math::easeOut(i);
		//std::cout << " - both: " << std::fixed << std::setprecision(6) << wolf::Math::ease(i) << "\n";
	//}
}

void BaseScene::Update()
{
}

void BaseScene::Render()
{
}


