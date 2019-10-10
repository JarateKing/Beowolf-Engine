#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>

#include <BMWLoader.h>


BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	wolf::BMWLoader::getInstance().loadFile("resources/models/hand.bmw");
}

void BaseScene::Update()
{
}

void BaseScene::Render()
{
}


