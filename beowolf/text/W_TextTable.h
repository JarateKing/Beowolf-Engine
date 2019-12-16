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
		TextTable();
		~TextTable();
		bool Load(const std::string& file);
		bool SetLanguage(const std::string& lang);
		std::string GetString(const std::string& id);
		void SetVar(const std::string& id, const std::string& val);
		std::string GetVar(const std::string& id);

	private:
		std::string m_currentLanguage;
		std::map<std::string, std::map<std::string, std::string>> m_lookupMap;
		std::map<std::string, std::string> m_varMap;
	};
}

#endif