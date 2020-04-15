#include "W_HudButton.h"
#include "W_Input.h"
#include "W_ProjectionMatrix.h"
#include "StateManager.h"

#include <iostream>

namespace wolf {
	// methods implemented as stubs, since there is nothing they need to do
	HudButton::HudButton() {}
	HudButton::~HudButton() {}
	void HudButton::Update(const float& p_fDelta) {}
	void HudButton::Render(const glm::mat4& proj) {}

	bool HudButton::IsClicked() {
		if (Input::Instance().isMouseReleased(INPUT_LMB)) {
			float x = Input::Instance().getMousePos().x / ProjMatrix::GetScreenSize().x * 1920 - m_xPos;
			float y = Input::Instance().getMousePos().y / ProjMatrix::GetScreenSize().y * 1080 - m_yPos;

			return (x >= 0 && x <= m_xBound && y >= 0 && y <= m_yBound);
		}
		return false;
	}
}