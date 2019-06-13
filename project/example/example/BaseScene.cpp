#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>
#include "W_TextTable.h"

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	wolf::TextTable* localization = new wolf::TextTable();
	localization->Load("../resources/localization/engine.txt");
	localization->SetLanguage("ENGLISH");
	std::cout << localization->GetString("engine_name");
}

void BaseScene::Update()
{
}

void BaseScene::Render()
{
}


