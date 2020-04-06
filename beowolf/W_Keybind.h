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
		// private constructor
		Keybind() {}

		void createKeymap();

		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		std::map<std::string, std::pair<int, int>> m_keymap;
		std::map<std::string, std::vector<std::string>> m_pressed;
		std::map<std::string, std::vector<std::string>> m_held;
		std::map<std::string, std::vector<std::string>> m_released;
		std::map<std::string, std::vector<std::string>> m_unheld;
	};
}

#endif