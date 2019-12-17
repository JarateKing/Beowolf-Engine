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
		m_localization->SetLanguage("ENGLISH");


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