#include "W_Hud.h"

#include "W_TextBox.h"
#include "W_HudImage.h"

namespace wolf {
	Hud::Hud() {
		m_localization = new TextTable();
		m_localization->Load("resources/localization/engine.txt");
		m_localization->SetLanguage("ENGLISH");

		m_fontlist.push_back(new Font("resources/fonts/", "opensans.fnt"));

		m_elements.push_back(new TextBox(m_fontlist[0], m_localization));
		m_elements[0]->SetBounds(1240, 220);
		m_elements[0]->SetPos(20, 20);
		((TextBox*)m_elements[0])->SetTextAlignment(AL_Left);
		((TextBox*)m_elements[0])->SetString("Engine_Name");
		((TextBox*)m_elements[0])->SetTextColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		m_elements.push_back(new HudImage("particles/particle.dds"));
		m_elements[1]->SetBounds(100, 100);
		m_elements[1]->SetPos(20, 20);
	}

	Hud::~Hud() {

	}

	void Hud::Update(float p_fDelta) {
		((TextBox*)m_elements[0])->SetStringRaw("FPS: " + std::to_string(1.0f / p_fDelta));
	}

	void Hud::Render(glm::mat4 projection) {
		for (int i = 0; i < m_elements.size(); i++)
			m_elements[i]->Render(projection);
	}
}