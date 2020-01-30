#include "W_HudButton.h"
#include "W_Input.h"
#include "W_ProjectionMatrix.h"

#include <iostream>

namespace wolf {
	HudButton::HudButton() {
		
	}

	HudButton::~HudButton() {

	}

	void HudButton::Update(float p_fDelta) {
		if (Input::Instance().isMouseReleased(INPUT_LMB)) {
			float x = Input::Instance().getMousePos().x / ProjMatrix::GetScreenSize().x * 1920 - m_xPos;
			float y = Input::Instance().getMousePos().y / ProjMatrix::GetScreenSize().y * 1080 - m_yPos;
			if (x >= 0 && x <= m_xBound && y >= 0 && y <= m_yBound)
				std::cout << "clicked\n";
		}
	}

	void HudButton::Render(glm::mat4 proj) {
		
	}
}