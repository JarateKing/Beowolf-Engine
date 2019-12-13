#include "W_Hud.h"

namespace wolf {
	Hud::Hud() {
		m_localization = new TextTable();
		m_localization->Load("resources/localization/engine.txt");
		m_localization->SetLanguage("ENGLISH");

		m_fontlist.push_back(new Font("resources/fonts/", "opensans.fnt"));

		m_textboxes.push_back(new TextBox(m_fontlist[0], m_localization));
		m_textboxes[0]->SetBounds(1240, 220);
		m_textboxes[0]->SetPos(20, 20);
		m_textboxes[0]->SetTextAlignment(AL_Left);
		m_textboxes[0]->SetString("Engine_Name");
		m_textboxes[0]->SetTextColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	Hud::~Hud() {

	}

	void Hud::Update(float p_fDelta) {

	}

	void Hud::Render(glm::mat4 projection) {
		for (int i = 0; i < m_textboxes.size(); i++)
			m_textboxes[i]->Render(projection);
	}
}