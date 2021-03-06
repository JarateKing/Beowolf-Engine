#include "W_TextTable.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace wolf
{
	//Constructor
	TextTable::TextTable()
	{
		m_varMap[""] = "$";
	}

	//Destructor
	TextTable::~TextTable()
	{
		// @TODO: implement destructor
	}

	//Loads in a text table from file
	bool TextTable::Load(const std::string& file)
	{
		std::ifstream fileReader(file);

		if (!fileReader.is_open())
			return false;

		//get first line
		std::string line;
		if (!std::getline(fileReader, line))
			return false;

		std::stringstream headerStream(line);
		std::string current;
		std::getline(headerStream, current, '\t'); // skip the first element
		std::vector<std::string> languages;
		while (std::getline(headerStream, current, '\t'))
		{
			languages.push_back(current);
			std::map<std::string, std::string> nestedMap;
			m_lookupMap[current] = nestedMap;
		}

		while (std::getline(fileReader, line))
		{
			std::stringstream lineStream(line);
			std::string id;

			std::getline(lineStream, id, '\t');
			for (int i = 0; i < languages.size(); i++)
			{
				std::string localizedString;
				std::getline(lineStream, localizedString, '\t');
				m_lookupMap[languages[i]][id] = localizedString;
			}
		}
	}

	//Sets current language
	bool TextTable::SetLanguage(const std::string& lang)
	{
		for (std::map<std::string, std::map<std::string, std::string>>::iterator iter = m_lookupMap.begin(); iter != m_lookupMap.end(); iter++)
		{
			if (iter->first == lang)
			{
				m_currentLanguage = lang;
				return true;
			}
		}

		return false;
	}

	//Looks up a string based on a string given
	std::string TextTable::GetString(const std::string& id)
	{
		return m_lookupMap[m_currentLanguage][id];
	}

	//Sets variable in map based on id and value input
	void TextTable::SetVar(const std::string& id, const std::string& val) {
		m_varMap[id] = val;
	}

	//Gets a variable from map based on id
	std::string TextTable::GetVar(const std::string& id) {
		if (m_varMap.count(id) == 0)
			return "$" + id + "$";
		
		return m_varMap[id];
	}
}
