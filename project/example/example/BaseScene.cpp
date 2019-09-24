#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"

#include "W_Font.h"
#include "W_TextTable.h"
#include "W_TextBox.h"

static glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 1000.0f) * glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
wolf::Font* opensans;
wolf::Font* impact_small;
wolf::Font* monoton_large;
wolf::TextTable* localization;
wolf::TextBox* textfield1;
wolf::TextBox* textfield2;
wolf::TextBox* textfield3;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	opensans = new wolf::Font("../resources/fonts/", "opensans_semibold.fnt");
	impact_small = new wolf::Font("../resources/fonts/", "impact_small.fnt");
	monoton_large = new wolf::Font("../resources/fonts/", "monoton_large.fnt");

	localization = new wolf::TextTable();
	localization->Load("../resources/localization/engine.txt");
	localization->SetLanguage("ENGLISH");

	textfield1 = new wolf::TextBox(opensans, localization);
	textfield1->SetBounds(1240, 220);
	textfield1->SetPos(20, 20);
	textfield1->SetTextAlignment(wolf::AL_Left);
	textfield1->SetString("Engine_Lang");
	textfield1->SetTextColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

	textfield2 = new wolf::TextBox(impact_small, localization);
	textfield2->SetBounds(1240, 220);
	textfield2->SetPos(20, 250);
	textfield2->SetTextAlignment(wolf::AL_Center);
	textfield2->SetString("Engine_Lang");
	textfield2->SetTextColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	textfield3 = new wolf::TextBox(monoton_large, localization);
	textfield3->SetBounds(1240, 220);
	textfield3->SetPos(20, 480);
	textfield3->SetTextAlignment(wolf::AL_Right);
	textfield3->SetString("Engine_Lang");
	textfield3->SetTextColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
}

void BaseScene::Update()
{
}

void BaseScene::Render()
{
	textfield1->Render(projection);
	textfield2->Render(projection);
	textfield3->Render(projection);
}


