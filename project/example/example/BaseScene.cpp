#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include "SceneRenderer.h"
#include "DebugCamera.h"
#include "DebugCube.h"
#include "W_Time.h"
#include "W_Input.h"
#include "Effect.h"

static wolf::DebugCamera* cam;
static glm::mat4 cull;

static Effect* effect;
static bool isFire;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	// view stuff
	cam = new wolf::DebugCamera(0, 0, glm::vec3(0, 0, -12));
	cull = cam->GetViewMatrix();

	// setup cubes
	effect = new Effect("../resources/particles/fire.xml");
	isFire = true;
}

void BaseScene::Update()
{
	float delta = wolf::Time::Instance().deltaTime();

	// update camera stuff
	cam->Update(delta);
	
	if (wolf::Input::Instance().isKeyPressed(INPUT_KB_C))
	{
		if (isFire)
		{
			isFire = false;
			delete effect;
			effect = new Effect("../resources/particles/glow.xml");
		}
		else
		{
			isFire = true;
			delete effect;
			effect = new Effect("../resources/particles/fire.xml");
		}
	}

	effect->Update(delta, glm::mat3(cam->GetViewMatrix()));
}

void BaseScene::Render()
{
	effect->Render(cam->GetViewMatrix());
}


