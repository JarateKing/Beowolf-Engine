#include "W_Hud.h"

#include "W_TextBox.h"
#include "W_HudImage.h"
#include "W_HudColorPanel.h"
#include "W_HudGradient.h"
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

		std::ifstream jsonIn(file);
		if (jsonIn) {
			std::stringstream jsonFileStream;
			jsonFileStream << jsonIn.rdbuf();
			std::string jsonFileData = jsonFileStream.str();
			nlohmann::json jsonData = nlohmann::json::parse(jsonFileData);

			if (jsonData.contains("localization")) {
				for (auto localization : jsonData["localization"]) {
					m_localization->Load(localization["file"]);
				}
			}

			if (jsonData.contains("defaultLanguage"))
				m_localization->SetLanguage(jsonData["defaultLanguage"]);
			else
				m_localization->SetLanguage("ENGLISH");

			if (jsonData.contains("fonts")) {
				for (auto font : jsonData["fonts"]) {
					std::string fontName = font["name"];
					std::string fontFolder = font["folder"];
					std::string fontFile = font["file"];
					m_fontlist[fontName] = new Font(fontFolder, fontFile);
				}
			}

			if (jsonData.contains("elements")) {
				for (auto element : jsonData["elements"]) {
					float x = 0, y = 0, z = 1, w = 0, h = 0;
					bool rx = false, ry = false, rw = false, rh = false;
					if (element.contains("xpos"))
						x = element["xpos"];
					if (element.contains("ypos"))
						y = element["ypos"];
					if (element.contains("zpos"))
						z = element["zpos"];
					if (element.contains("wide"))
						w = element["wide"];
					if (element.contains("tall"))
						h = element["tall"];

					if (element.contains("xposRelative"))
						rx = element["xposRelative"];
					if (element.contains("yposRelative"))
						ry = element["yposRelative"];
					if (element.contains("wideRelative"))
						rw = element["wideRelative"];
					if (element.contains("tallRelative"))
						rh = element["tallRelative"];

					std::string type = element["type"];
					if (type == "textbox") {
						std::string fontName = element["font"];
						std::string textLabel = element["label"];
						bool isTextLabelRaw = element["labelRaw"];
						float fontSize = element["fontSize"];

						std::string textAlignment = element["alignment"];
						float alignment = AL_Center;
						if (textAlignment == "left")
							alignment = AL_Left;
						if (textAlignment == "right")
							alignment = AL_Right;

						std::string textColor = element["color"];
						glm::vec4 actualColor = glm::vec4(1, 1, 1, 1);
						std::stringstream colorSS(textColor);
						colorSS >> actualColor[0] >> actualColor[1] >> actualColor[2] >> actualColor[3];

						TextBox* current = new TextBox(m_fontlist[fontName], m_localization);
						current->SetTextAlignment(alignment);
						current->SetTextColor(actualColor);
						current->SetSize(fontSize);
						current->SetX(x, rx);
						current->SetY(y, ry);
						current->SetW(w, rw);
						current->SetH(h, rh);
						current->SetZ(z);

						if (isTextLabelRaw)
							current->SetStringRaw(textLabel);
						else
							current->SetString(textLabel);

						m_elements.push_back(current);
					}
					else if (type == "image") {
						std::string imageFile = element["image"];

						HudImage* current = new HudImage(imageFile);
						current->SetX(x, rx);
						current->SetY(y, ry);
						current->SetW(w, rw);
						current->SetH(h, rh);
						current->SetZ(z);
						m_elements.push_back(current);
					}
					else if (type == "fillcolor") {
						std::string textColor = element["color"];
						glm::vec4 actualColor = glm::vec4(1, 1, 1, 1);
						std::stringstream colorSS(textColor);
						colorSS >> actualColor[0] >> actualColor[1] >> actualColor[2] >> actualColor[3];

						HudColorPanel* current = new HudColorPanel(actualColor);
						current->SetX(x, rx);
						current->SetY(y, ry);
						current->SetW(w, rw);
						current->SetH(h, rh);
						current->SetZ(z);
						m_elements.push_back(current);
					}
					else if (type == "gradient") {
						std::string textColor1 = element["topLeft"];
						glm::vec4 topleftColor = glm::vec4(1, 1, 1, 1);
						std::stringstream topleftSS(textColor1);
						topleftSS >> topleftColor[0] >> topleftColor[1] >> topleftColor[2] >> topleftColor[3];

						std::string textColor2 = element["topRight"];
						glm::vec4 toprightColor = glm::vec4(1, 1, 1, 1);
						std::stringstream toprightSS(textColor2);
						toprightSS >> toprightColor[0] >> toprightColor[1] >> toprightColor[2] >> toprightColor[3];

						std::string textColor3 = element["bottomRight"];
						glm::vec4 bottomrightColor = glm::vec4(1, 1, 1, 1);
						std::stringstream bottomrightSS(textColor3);
						bottomrightSS >> bottomrightColor[0] >> bottomrightColor[1] >> bottomrightColor[2] >> bottomrightColor[3];

						std::string textColor4 = element["bottomLeft"];
						glm::vec4 bottomleftColor = glm::vec4(1, 1, 1, 1);
						std::stringstream bottomleftSS(textColor4);
						bottomleftSS >> bottomleftColor[0] >> bottomleftColor[1] >> bottomleftColor[2] >> bottomleftColor[3];

						HudGradient* current = new HudGradient(topleftColor, toprightColor, bottomleftColor, bottomrightColor);
						current->SetX(x, rx);
						current->SetY(y, ry);
						current->SetW(w, rw);
						current->SetH(h, rh);
						current->SetZ(z);
						m_elements.push_back(current);
					}
				}
			}
		}
	}

	Hud::~Hud() {
	}

	void Hud::Update(float p_fDelta) {
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

	void Hud::SetVar(std::string id, std::string val) {
		m_localization->SetVar(id, val);
	}
}