#include "W_Hud.h"

#include "W_TextBox.h"
#include "W_HudImage.h"
#include "W_HudColorPanel.h"
#include <algorithm>

namespace wolf {

	bool zSortCompare(HudElement* a, HudElement* b) {
		return a->GetZ() < b->GetZ();
	}

	Hud::Hud() {
		m_localization = new TextTable();
		m_localization->Load("resources/localization/engine.txt");
		m_localization->SetLanguage("ENGLISH");

		m_fontlist["opensans.fnt"] = new Font("resources/fonts/", "opensans.fnt");

		m_elements.push_back(new TextBox(m_fontlist["opensans.fnt"], m_localization));
		m_elements[0]->SetBounds(1240, 220);
		m_elements[0]->SetPos(20, 20);
		((TextBox*)m_elements[0])->SetTextAlignment(AL_Left);
		((TextBox*)m_elements[0])->SetString("Engine_FPS");
		((TextBox*)m_elements[0])->SetTextColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_elements[0]->SetZ(2.0f);

		m_elements.push_back(new HudImage("particles/particle.dds"));
		m_elements[1]->SetX(1920 - 80);
		m_elements[1]->SetY(0);
		m_elements[1]->SetW(80);
		m_elements[1]->SetH(80);
		m_elements[1]->SetZ(5.0f);

		m_elements.push_back(new HudColorPanel(glm::vec4(0.0f, 0.0f, 0.0f, 0.3f)));
		m_elements[2]->SetPos(0, 0);
		m_elements[2]->SetW(1, true);
		m_elements[2]->SetH(80);
		m_elements[2]->SetZ(10.0f);
	}

	Hud::~Hud() {
	}

	void Hud::Update(float p_fDelta) {
		m_localization->SetVar("delta", std::to_string(p_fDelta));
		m_localization->SetVar("fps", std::to_string(1.0 / p_fDelta));

		if (m_prevElementsSize != m_elements.size()) {
			std::stable_sort(m_elements.begin(), m_elements.end(), zSortCompare);
			m_prevElementsSize = m_elements.size();
		}

		for (int i = m_elements.size() - 1; i >= 0; i--)
			m_elements[i]->Update(p_fDelta);
	}

	void Hud::Render(glm::mat4 projection) {
		for (int i = m_elements.size() - 1; i >= 0; i--)
			m_elements[i]->Render(projection);
	}
}