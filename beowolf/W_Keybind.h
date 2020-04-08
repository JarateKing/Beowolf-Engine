#ifndef W_KEYBINDS_H
#define W_KEYBINDS_H

#include "W_Input.h"
#include "W_Common.h"
#include <map>
#include <vector>
#include <utility>

namespace wolf
{
	class Keybind
	{
	private:
		enum InputType {
			Keyboard = 0,
			Mouse = 1,
			Controller = 2
		};

		enum BindType {
			Pressed = 0,
			Held = 1,
			Released = 2,
			Unheld = 3
		};

	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		// singleton pattern get instance
		static Keybind& Instance()
		{
			static Keybind m_instance;
			return m_instance;
		}
		// remove methods that break the singleton pattern
		Keybind(Keybind const&) = delete;
		void operator=(Keybind const&) = delete;

		void addBinds(std::string jsonfile);
		bool getBind(std::string bind);

	private:
		//-------------------------------------------------------------------------
		// PRIVATE METHODS
		//-------------------------------------------------------------------------
		void createKeymap();
		bool functionSwitch(InputType input, BindType bind, int key);
		Keybind() {
			createKeymap();
		}

		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		std::map<std::string, std::pair<int, int>> m_keymap;
		std::map<std::string, std::vector<std::string>> m_pressed;
		std::map<std::string, std::vector<std::string>> m_held;
		std::map<std::string, std::vector<std::string>> m_released;
		std::map<std::string, std::vector<std::string>> m_unheld;

		std::map<std::string, std::vector<std::string>> m_bind[4];
	};
}

#endif