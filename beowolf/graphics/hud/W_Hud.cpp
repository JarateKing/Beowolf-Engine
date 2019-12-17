#include "W_Hud.h"

#include "W_TextBox.h"
#include "W_HudImage.h"
#include "W_HudColorPanel.h"
#include <algorithm>
#include "JSON/json.hpp"
#include <sstream>
#include <fstream>
#include <iostream>

namespace wolf {

	bool zSortCompare(HudElement* a, HudElement* b) {
		return a->GetZ() < b->GetZ();
	}

	Hud::Hud(std::string file) {
		m_localization = new TextTable();
		m_localization->SetLanguage("ENGLISH");

		std::ifstream jsonIn(file);
		if (jsonIn) {
			std::stringstream jsonFileStream;
			jsonFileStream << jsonIn.rdbuf();
			std::string jsonFileData = jsonFileStream.str();
			nlohmann::json jsonData = nlohmann::json::parse(jsonFileData);
			
			if (jsonData.contains("defaultLanguage")) {
				m_localization->SetLanguage(jsonData["defaultLanguage"]);
			}

			if (jsonData.contains("localization")) {
				for (auto localization : jsonData["localization"]) {
				}
			}

			if (jsonData.contains("fonts")) {
				for (auto font : jsonData["fonts"]) {
				}
			}

			if (jsonData.contains("elements")) {
				for (auto element : jsonData["elements"]) {
				}
			}
		}
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