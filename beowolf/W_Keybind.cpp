#include "W_Keybind.h"
#include "JSON/json.hpp"
#include <sstream>
#include <fstream>

std::string bindNames[] = { "pressed", "held", "released", "unheld" };

namespace wolf
{
	void Keybind::addBinds(std::string jsonfile) {
		std::ifstream jsonIn(jsonfile);
		if (jsonIn) {
			std::stringstream jsonFileStream;
			jsonFileStream << jsonIn.rdbuf();
			std::string jsonFileData = jsonFileStream.str();
			nlohmann::json jsonData = nlohmann::json::parse(jsonFileData);

			for (int i = 0; i < 4; i++)
				for (auto bind = jsonData[bindNames[i]].begin(); bind != jsonData[bindNames[i]].end(); bind++)
					for (std::string command : bind.value())
						m_bind[i][command].push_back(bind.key());
		}
	}

	bool Keybind::getBind(std::string bind) {
		for (int i = 0; i < 4; i++)
			for (auto key : m_bind[i][bind])
				if (functionSwitch((InputType)m_keymap[key].first, (BindType)i, m_keymap[key].second))
					return true;

		return false;
	}

	bool Keybind::functionSwitch(InputType input, BindType bind, int key) {
		wolf::Input* instance = &(wolf::Input::Instance());
		
		if (input == InputType::Keyboard) {
			if (bind == BindType::Pressed)
				return instance->isKeyPressed(key);
			if (bind == BindType::Held)
				return instance->isKeyHeld(key);
			if (bind == BindType::Released)
				return instance->isKeyReleased(key);
			if (bind == BindType::Unheld)
				return instance->isKeyUnheld(key);
		}
		else if (input == InputType::Mouse) {
			if (bind == BindType::Pressed)
				return instance->isMousePressed(key);
			if (bind == BindType::Held)
				return instance->isMouseHeld(key);
			if (bind == BindType::Released)
				return instance->isMouseReleased(key);
			if (bind == BindType::Unheld)
				return instance->isMouseUnheld(key);
		}
		else if (input == InputType::Controller) {
			if (bind == BindType::Pressed)
				return instance->isControllerButtonPressed(key);
			if (bind == BindType::Held)
				return instance->isControllerButtonHeld(key);
			if (bind == BindType::Released)
				return instance->isControllerButtonReleased(key);
			if (bind == BindType::Unheld)
				return instance->isControllerButtonUnheld(key);
		}
	}

	void Keybind::createKeymap() {
		// alphanumeric
		for (char i = 'a'; i <= 'z'; i++) {
			m_keymap[std::string(1, i)] = { InputType::Keyboard, INPUT_KB_A + (i - 'a')};
		}
		for (char i = '0'; i <= '9'; i++) {
			m_keymap[std::string(1, i)] = { InputType::Keyboard, INPUT_KB_0 + (i - '0') };
		}
		m_keymap["space"] = { InputType::Keyboard, INPUT_KB_SPACE };

		// mouse
		m_keymap["leftclick"] = { InputType::Mouse, INPUT_LMB };
		m_keymap["rightclick"] = { InputType::Mouse, INPUT_RMB };

		// controller
		m_keymap["controller_a"] = { InputType::Controller, INPUT_CONTROLLER_A };
		m_keymap["controller_b"] = { InputType::Controller, INPUT_CONTROLLER_B };
		m_keymap["controller_x"] = { InputType::Controller, INPUT_CONTROLLER_X };
		m_keymap["controller_y"] = { InputType::Controller, INPUT_CONTROLLER_Y };
		m_keymap["controller_lb"] = { InputType::Controller, INPUT_CONTROLLER_LB };
		m_keymap["controller_rb"] = { InputType::Controller, INPUT_CONTROLLER_RB };
	}
}