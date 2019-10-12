#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>

#include <BMWModel.h>


BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	wolf::BMWModel* test = new wolf::BMWModel("resources/models/hand.bmw", "", "");
}

void BaseScene::Update()
{
}

void BaseScene::Render()
{
}


