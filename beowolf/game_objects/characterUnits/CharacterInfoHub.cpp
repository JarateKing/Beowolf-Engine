#include "CharacterInfoHub.h"
#include "JSON/json.hpp"
#include "W_ResourceLoader.h"
#include <sstream>
#include <fstream>
#include <iostream>

const std::vector<std::string> INFO_NAMES = { "HP", "MaxMovement", "MaxAttack", "MinAttack", "Health", "Damage", "Defense" };

//Class used to load and store information about characters including their stats

CharacterInfoHub::CharacterInfoHub()
{

}

CharacterInfoHub::~CharacterInfoHub()
{

}

//Method to add character to hub
//Reads through a JSON file and stores characters info
void CharacterInfoHub::AddCharacter(std::string p_characterJson, std::string p_characterName)
{
	InfoBit temp;
	temp.m_name = p_characterName;

	//Create stream from JSON File
	std::ifstream jsonIn(wolf::ResourceLoader::Instance().getJSONObject(p_characterJson));
	nlohmann::json jsonData;

	//If JSON File loads, read through file storing character information
	if (jsonIn) 
	{
		std::stringstream jsonFileStream;
		jsonFileStream << jsonIn.rdbuf();
		std::string jsonFileData = jsonFileStream.str();
		jsonData = nlohmann::json::parse(jsonFileData);

		if (jsonData.contains("Character")) 
		{
			std::string tempDesc = jsonData["Character"]["BasicDescription"];
			temp.m_desc = tempDesc;
			temp.m_type = "Character";
			for (int i = 0; i < INFO_NAMES.size(); i++)
			{
				if (jsonData["Character"].contains(INFO_NAMES[i]))
				{
					temp.m_info[INFO_NAMES[i]] = jsonData["Character"][INFO_NAMES[i]];

					if (INFO_NAMES[i] == "HP")
						temp.m_info["Health"] = temp.m_info["HP"];
				}
			}
		}
	}

	//Push back created character onto vector
	m_infoBits.push_back(temp);
}

//Method to add an enemey type from a JSON File
void CharacterInfoHub::AddEnemyType(std::string p_enemyJson, std::string p_enemyName)
{
	InfoBit temp;
	temp.m_name = p_enemyName;
	
	//Create stream from JSON File
	std::ifstream jsonIn(wolf::ResourceLoader::Instance().getJSONObject(p_enemyJson));
	nlohmann::json jsonData;

	//If JSON File loads, read through file storing enemy information
	if (jsonIn)
	{
		std::stringstream jsonFileStream;
		jsonFileStream << jsonIn.rdbuf();
		std::string jsonFileData = jsonFileStream.str();
		jsonData = nlohmann::json::parse(jsonFileData);

		if (jsonData.contains("Enemy"))
		{
			std::string tempDesc = jsonData["Enemy"]["BasicDescription"];
			temp.m_desc = tempDesc;
			temp.m_type = "Enemy";
			for (int i = 0; i < INFO_NAMES.size(); i++)
			{
				if (jsonData["Enemy"].contains(INFO_NAMES[i]))
				{
					temp.m_info[INFO_NAMES[i]] = jsonData["Enemy"][INFO_NAMES[i]];

					if (INFO_NAMES[i] == "HP")
						temp.m_info["Health"] = temp.m_info["HP"];
				}
			}
		}
	}

	//Push back enemy onto vector
	m_infoBits.push_back(temp);
}

//Method to add an item type to game
void CharacterInfoHub::AddItemType(std::string p_itemJson)
{
	InfoBit temp;

	//Create stream from JSON File
	std::ifstream jsonIn(wolf::ResourceLoader::Instance().getJSONObject(p_itemJson));
	nlohmann::json jsonData;

	//If JSON File loads, read through file storing item information
	if (jsonIn)
	{
		std::stringstream jsonFileStream;
		jsonFileStream << jsonIn.rdbuf();
		std::string jsonFileData = jsonFileStream.str();
		jsonData = nlohmann::json::parse(jsonFileData);

		if (jsonData.contains("Name"))
		{
			std::string tempName = jsonData["Name"];
			std::string tempDesc = jsonData["BasicDescription"];
			temp.m_name = tempName;
			temp.m_desc = tempDesc;
			temp.m_type = "Item";
			for (int i = 0; i < INFO_NAMES.size(); i++)
			{
				if (jsonData["Stats"].contains(INFO_NAMES[i]))
				{
					temp.m_info[INFO_NAMES[i]] = jsonData["Stats"][INFO_NAMES[i]];
				}
			}
		}
	}

	//Push back item onto vector
	m_infoBits.push_back(temp);
}

//Damage an enemy based on character's stored MaxAttack stat and affected by defense stat
void CharacterInfoHub::DamageEnemy(std::string p_enemyName, std::string p_characterName, float mult)
{
	for (int i = 0; i < m_infoBits.size(); i++)
	{
		if (m_infoBits.at(i).m_name.compare(p_enemyName) == 0)
		{
			for (int j = 0; j < m_infoBits.size(); j++)
			{
				if (m_infoBits.at(j).m_name.compare(p_characterName) == 0)
				{
					m_infoBits.at(i).m_info["HP"] -= m_infoBits.at(j).m_info["MaxAttack"] * mult * (100 / m_infoBits.at(i).m_info["Defense"]);

					if (m_infoBits.at(i).m_info["HP"] < 0)
						m_infoBits.at(i).m_info["HP"] = 0;
				}
			}
		}
	}
}

//Damage a Player Controlled Character based on character's stored MaxAttack stat and affected by defense stat
void CharacterInfoHub::DamageCharacter(std::string p_characterName, std::string p_enemyName, float mult)
{
	for (int i = 0; i < m_infoBits.size(); i++)
	{
		if (m_infoBits.at(i).m_name.compare(p_characterName) == 0)
		{
			for (int j = 0; j < m_infoBits.size(); j++)
			{
				if (m_infoBits.at(j).m_name.compare(p_enemyName) == 0)
				{
					m_infoBits.at(i).m_info["HP"] -= m_infoBits.at(j).m_info["MaxAttack"] * mult * (100 / m_infoBits.at(i).m_info["Defense"]);

					if (m_infoBits.at(i).m_info["HP"] < 0)
						m_infoBits.at(i).m_info["HP"] = 0;
				}
			}
		}
	}
}

//Add an items stats to a character and store changed stats
void CharacterInfoHub::GivePlayerItem(std::string p_characterName, std::string p_itemName)
{
	for (int i = 0; i < m_infoBits.size(); i++)
	{
		if (m_infoBits.at(i).m_name.compare(p_itemName) == 0)
		{
			for (int j = 0; j < m_infoBits.size(); j++)
			{
				if (m_infoBits.at(j).m_name.compare(p_characterName) == 0)
				{
					std::map<std::string, float>::iterator it = m_infoBits.at(i).m_info.begin();
					while (it != m_infoBits.at(i).m_info.end())
					{
						m_infoBits.at(j).m_info[it->first] += it->second;

						if (it->first == "HP" && m_infoBits.at(j).m_info["HP"] > m_infoBits.at(j).m_info["Health"])
							m_infoBits.at(j).m_info["HP"] = m_infoBits.at(j).m_info["Health"];

						it++;
					}
				}
			}
		}
	}
}

//Lookup for a stat by stat ID
float CharacterInfoHub::GetStat(std::string p_characterName, std::string p_statID)
{
	for (int i = 0; i < m_infoBits.size(); i++)
	{
		if (m_infoBits.at(i).m_name.compare(p_characterName) == 0)
		{
			if (m_infoBits.at(i).m_info.count(p_statID))
				return m_infoBits.at(i).m_info[p_statID];
		}
	}
	return -1;
}

//Lookup for full stats of a character by character name
std::vector<std::pair<std::string, float>> CharacterInfoHub::GetStats(std::string p_characterName)
{
	for (int i = 0; i < m_infoBits.size(); i++)
	{
		if (m_infoBits.at(i).m_name.compare(p_characterName) == 0)
		{
			std::vector<std::pair<std::string, float>> toret;
			for (auto value : m_infoBits.at(i).m_info)
				toret.push_back({ value.first, value.second });
			return toret;
		}
	}
	return std::vector<std::pair<std::string, float>>();
}

//Pull description of character, lookup by character name
std::string CharacterInfoHub::GetDescription(std::string p_characterName)
{
	for (int i = 0; i < m_infoBits.size(); i++)
	{
		if (m_infoBits.at(i).m_name.compare(p_characterName) == 0)
			return m_infoBits.at(i).m_desc;
	}
}

//Update a stat if needed by lookup of character name and stat ID
void CharacterInfoHub::UpdateStat(std::string p_characterName, std::string p_statID, float p_updatedValue)
{
	for (int i = 0; i < m_infoBits.size(); i++)
	{
		if (m_infoBits.at(i).m_name.compare(p_characterName) == 0)
		{
			if (m_infoBits.at(i).m_info.count(p_statID))
				m_infoBits.at(i).m_info[p_statID] = p_updatedValue;
		}
	}
}

//Debug Method to print out all information stored currently
void CharacterInfoHub::PrintOutInfo()
{
	std::cout << "Test Info Character" << std::endl << "====================" << std::endl;
	for (int i = 0; i < m_infoBits.size(); i++)
	{
		std::cout << "Name: " << m_infoBits.at(i).m_name << std::endl;
		std::cout << "Desc: " << m_infoBits.at(i).m_desc << std::endl;
		std::cout << "Type: " << m_infoBits.at(i).m_type << std::endl;

		std::map<std::string, float>::iterator it = m_infoBits.at(i).m_info.begin();
		while (it != m_infoBits.at(i).m_info.end())
		{
			std::cout << it->first << " | ";
			std::cout << it->second << std::endl;
			it++;
		}
	}
	std::cout << "====================" << std::endl;
}