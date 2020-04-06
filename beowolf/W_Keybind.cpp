#include "W_Keybind.h"

namespace wolf
{
	void Keybind::addBinds(std::string jsonfile) {

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
}