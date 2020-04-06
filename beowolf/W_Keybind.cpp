#include "W_Keybind.h"

namespace wolf
{
	void Keybind::addBinds(std::string jsonfile) {


		createKeymap();
	}

	bool Keybind::getBind(std::string bind) {
		for (auto key : m_pressed[bind]) {
			std::pair<int, int> keyval = m_keymap[key];
			if (keyval.first == 0 && Input::Instance().isKeyPressed(keyval.second))
				return true;
			else if (keyval.first == 1 && Input::Instance().isMousePressed(keyval.second))
				return true;
			else if (keyval.first == 2 && Input::Instance().isControllerButtonPressed(keyval.second))
				return true;
		}
		for (auto key : m_held[bind]) {
			std::pair<int, int> keyval = m_keymap[key];
			if (keyval.first == 0 && Input::Instance().isKeyHeld(keyval.second))
				return true;
			else if (keyval.first == 1 && Input::Instance().isMouseHeld(keyval.second))
				return true;
			else if (keyval.first == 2 && Input::Instance().isControllerButtonHeld(keyval.second))
				return true;
		}
		for (auto key : m_released[bind]) {
			std::pair<int, int> keyval = m_keymap[key];
			if (keyval.first == 0 && Input::Instance().isKeyReleased(keyval.second))
				return true;
			else if (keyval.first == 1 && Input::Instance().isMouseReleased(keyval.second))
				return true;
			else if (keyval.first == 2 && Input::Instance().isControllerButtonReleased(keyval.second))
				return true;
		}
		for (auto key : m_unheld[bind]) {
			std::pair<int, int> keyval = m_keymap[key];
			if (keyval.first == 0 && Input::Instance().isKeyUnheld(keyval.second))
				return true;
			else if (keyval.first == 1 && Input::Instance().isMouseUnheld(keyval.second))
				return true;
			else if (keyval.first == 2 && Input::Instance().isControllerButtonUnheld(keyval.second))
				return true;
		}

		return false;
	}

	void Keybind::createKeymap() {
		// alphanumeric
		for (char i = 'a'; i <= 'z'; i++) {
			m_keymap[std::string(1, i)] = { 0, INPUT_KB_A };
		}
		for (char i = '0'; i <= '9'; i++) {
			m_keymap[std::string(1, i)] = { 0, INPUT_KB_0 };
		}
		m_keymap["space"] = { 0, INPUT_KB_SPACE };

		// mouse
		m_keymap["leftclick"] = { 1, INPUT_LMB };
		m_keymap["rightclick"] = { 1, INPUT_RMB };

		// controller
		m_keymap["controller_a"] = { 2, INPUT_CONTROLLER_A };
		m_keymap["controller_b"] = { 2, INPUT_CONTROLLER_B };
		m_keymap["controller_x"] = { 2, INPUT_CONTROLLER_X };
		m_keymap["controller_y"] = { 2, INPUT_CONTROLLER_Y };
	}
}