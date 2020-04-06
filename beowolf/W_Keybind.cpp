#include "W_Keybind.h"

namespace wolf
{
	void Keybind::addBinds(std::string jsonfile) {


		createKeymap();
	}

	bool Keybind::getBind(std::string bind) {
		for (auto keys : m_pressed[bind]) {

		}
		for (auto keys : m_held[bind]) {

		}
		for (auto keys : m_released[bind]) {

		}
		for (auto keys : m_unheld[bind]) {

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