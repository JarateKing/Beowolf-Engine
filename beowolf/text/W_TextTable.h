#ifndef W_TEXTTABLE_H
#define W_TEXTTABLE_H

#include "W_Common.h"
#include <string.h>
#include <map>

namespace wolf
{
	class TextTable
	{
	public:
		//Public Methods
		TextTable();
		~TextTable();
		bool Load(const std::string& file);
		bool SetLanguage(const std::string& lang);
		void SetVar(const std::string& id, const std::string& val);
		std::string GetString(const std::string& id);
		std::string GetVar(const std::string& id);

	private:
		//Private Variables
		std::string m_currentLanguage;
		std::map<std::string, std::map<std::string, std::string>> m_lookupMap;
		std::map<std::string, std::string> m_varMap;
	};
}

#endif